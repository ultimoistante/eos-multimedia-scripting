
#include "ViewSurface.h"

// -----------------------------------------------------------------------------

ViewSurface::ViewSurface()
    {
    this->width = 0;
    this->height = 0;
    this->connected = VIEW_SURFACE_SRC_NONE;
    this->visible = true;
    this->id = -1;
    //
    this->fboId = 0;
    this->fboIdCopy = 0;
    this->textureId = 0;
    this->textureIdCopy = 0;
    this->glTextureType = GL_TEXTURE_2D;
    this->pboMode = 0;
    this->index = 0;
    //
    setPosition(0, 0, 0);
    setOrientation(0, 0, 0, 1);
    setScale(1, 1, 1);
    setColor(1, 1, 1, 1);
    }

// -----------------------------------------------------------------------------

ViewSurface::~ViewSurface()
    {
    if(hadSubSurfaces())
        {
        glUnloadTexture(&textureIdCopy);
        subSurfaces.clear();
        }

    disconnectSource();
    glUnloadFBO();
    }

// -----------------------------------------------------------------------------

bool ViewSurface::create(uint width, uint height)
    {
    // store size
    this->width = width;
    this->height = height;

    // check npots (non power of two size)
    if(glPow2(width) > width || glPow2(height) > height)
        {
        //glTextureType = GL_TEXTURE_RECTANGLE_ARB;
        }

    // create fbo
    glLoadFBO();

    return true;
    }

// -----------------------------------------------------------------------------

bool ViewSurface::connectSource(eos_imagedata *data)
    {
    if(connected != VIEW_SURFACE_SRC_NONE)
        {
        LOGWARNING("[eos.view] surface already connected");
        return false;
        }

    switch(data->imageType)
        {
        case EOS_IMAGETYPE_GLTEXTURE:
            return connectSourceTexture(data);
        case EOS_IMAGETYPE_RGB_BUFFER:
        case EOS_IMAGETYPE_RGBA_BUFFER:
            return connectSourceBuffer(data);
        case EOS_IMAGETYPE_CAIROSURFACE:
            
        default:
            LOGERROR("[eos.view] imagetype unknown (%d)", data->imageType);
            return false;
        }
    }

// -----------------------------------------------------------------------------

bool ViewSurface::connectSourceTexture(eos_imagedata *data)
    {
    if(!glIsTexture(data->glTextureHandle))
        {
        LOGWARNING("[eos.view] invalid texture");
        return false;
        }
    
    // memorize object
    this->image = data;

    // resize surface
    this->width = data->width;
    this->height = data->height;

    textureId = data->glTextureHandle;
    connected = VIEW_SURFACE_SRC_TEXTURE;
    return true;
    }

// -----------------------------------------------------------------------------

bool ViewSurface::connectSourceBuffer(eos_imagedata* data)
    {
    // memorize object
    this->image = data;

    // copy buffer properties
    srfData.buffer = data->buffer;
    srfData.bufferSize = data->bufferSize;
    srfData.staticBuffer = data->bufferStatic;

    // resize surface
    this->width = data->width;
    this->height = data->height;

    if(!srfData.staticBuffer)
        {
        // dynamic buffer
        connected = VIEW_SURFACE_SRC_DYNAMICBUFFER;

        // load gl texture
        glLoadTexture(&textureId, false);

        // create pbo buffers il buffer is dynamic
        if(ViewPlatformSupport::makePBO(&srfData))
            pboMode = 2;
        else
            pboMode = 0;
        }
    else
        {
        // static buffer
        connected = VIEW_SURFACE_SRC_STATICBUFFER;

        // load gl texture
        glLoadTexture(&textureId, true);
        }

    return true;
    }

// -----------------------------------------------------------------------------

bool ViewSurface::connectSourceCairoSurface(eos_imagedata *data)
    {
    // memorize object
    this->image = data;
    
    // copy buffer properties
    srfData.buffer = data->buffer;
    
    // resize surface
    this->width = data->width;
    this->height = data->height;
    
    connected = VIEW_SURFACE_SRC_CAIROSURFACE;
    
    // load gl texture
    glLoadTexture(&textureId, true);
    
    return true;
    }

// -----------------------------------------------------------------------------

bool ViewSurface::disconnectSource()
    {
    switch(connected)
    {
        case VIEW_SURFACE_SRC_NONE:
            LOGWARNING("[eos.view] surface already disconnected");
            return false;
        case VIEW_SURFACE_SRC_TEXTURE:
            textureId = 0;
            break;
        case VIEW_SURFACE_SRC_DYNAMICBUFFER:
            ViewPlatformSupport::destroyPBO(&srfData);
            // intentionally NO-BREAK
        case VIEW_SURFACE_SRC_STATICBUFFER:
            glUnloadTexture(&textureId);
            srfData.buffer = NULL;
            srfData.bufferSize = 0;
            break;
    }

    connected = VIEW_SURFACE_SRC_NONE;
    return true;
    }

// -----------------------------------------------------------------------------

void ViewSurface::paint()
    {
    // if not visible or not connected don't update
    //
    if(!visible)
        return;

    // if connected to a buffer update PBO
    //
    if(connected == VIEW_SURFACE_SRC_DYNAMICBUFFER && image->bufferUpdated)
        {
        updatePBO();
        }

    // if subsurfaces are present do sub-paint
    //
    if(hadSubSurfaces())
        {
        paintSubSurfaces();
        }

    // draw surface
    //
    float w = (float) width;
    float h = (float) height;
    float w2 = w / 2;
    float h2 = h / 2;

    glLoadIdentity();
    glTranslatef(position.x(), position.y(), position.z());
    glTranslatef(0, 0, -725.0f);
    //glRotatef(rotation., 0.0f, 1.0f, 0.0f);
    glScalef(scale.x(), scale.y(), scale.z());

    if(connected == VIEW_SURFACE_SRC_NONE)
        {
        glBindTexture(GL_TEXTURE_2D, 0);
        glBegin(GL_QUADS);
        glColor4f(1.0, 1.0, 1.0, 1.0);
        glVertex3f(-w2, -h2, 0);
        glVertex3f(w2, -h2, 0);
        glColor4f(1.0, 0.5, 0.5, 1.0);
        glVertex3f(w2, h2, 0);
        glVertex3f(-w2, h2, 0);
        glEnd();
        }
    else
        {
        if(!hadSubSurfaces())
            glBindTexture(glTextureType, textureId);
        else
            glBindTexture(glTextureType, textureIdCopy);

        glEnable(glTextureType);
        glBegin(GL_QUADS);
        glColor4f(color[0], color[1], color[2], color[3]);
        //
        switch(glTextureType)
        {
            case GL_TEXTURE_2D:
                glTexCoord2f(0, 1);     glVertex3f(-w2, -h2, 0);
                glTexCoord2f(1, 1);     glVertex3f( w2, -h2, 0);
                glTexCoord2f(1, 0);     glVertex3f( w2,  h2, 0);
                glTexCoord2f(0, 0);     glVertex3f(-w2,  h2, 0);
                break;

            case GL_TEXTURE_RECTANGLE_ARB:
                glTexCoord2f(0, h);     glVertex3f(-w2, -h2, 0);
                glTexCoord2f(w, h);     glVertex3f( w2, -h2, 0);
                glTexCoord2f(w, 0);     glVertex3f( w2,  h2, 0);
                glTexCoord2f(0, 0);     glVertex3f(-w2,  h2, 0);
                break;
        }
        //
        glEnd();
        }

    ViewUtility::checkGL("surface.update");
    }

// -----------------------------------------------------------------------------

bool ViewSurface::addSubSurface(ViewSurface *surface)
    {
    if(surface == NULL || surface->getGLTextureId() == 0)
        return false;

    this->subSurfaces.push_back(surface);

    // create a "working texture" to paint subsurfaces on
    if(textureIdCopy == 0)
        glLoadTexture(&textureIdCopy, false);

    return true;
    }

// -----------------------------------------------------------------------------

bool ViewSurface::removeSubSurface(ViewSurface *surface)
    {
    if(surface == NULL)
        return false;

    std::vector<ViewSurface*>::iterator it;

    for(it = subSurfaces.begin(); it != subSurfaces.end(); it++)
        {
        if(*it == surface)
            {
            subSurfaces.erase(it);
            return true;
            }
        }

    return false;
    }

// -----------------------------------------------------------------------------

void ViewSurface::setPosition(float x, float y, float z)
    {
    position.set(x, y, z);
    }

// -----------------------------------------------------------------------------

void ViewSurface::setOrientation(float x, float y, float z, float w)
    {
    rotation.set(w, x, y, z);
    }

// -----------------------------------------------------------------------------

void ViewSurface::setScale(float x, float y, float z)
    {
    scale.set(x, y, z);
    }

// -----------------------------------------------------------------------------

void ViewSurface::setColor(float r, float g, float b, float a)
    {
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
    }

// -----------------------------------------------------------------------------

void ViewSurface::rotateByAxisAngle(float x, float y, float z, float a)
    {
    //rotation.rotate(0, Vector3f(x, y, z), a);
    }

// -----------------------------------------------------------------------------

void ViewSurface::rotateByAngles(float x, float y, float z)
    {
    //rotation.rotate_x(x);
    //rotation.rotate_y(y);
    //rotation.rotate_z(z);
    }

// -----------------------------------------------------------------------------

vmml::vec3f ViewSurface::getPosition()
    {
    return position;
    }

// -----------------------------------------------------------------------------

vmml::quaternionf ViewSurface::getOrientation()
    {
    return rotation;
    }

// -----------------------------------------------------------------------------

bool ViewSurface::load(const char* filename, bool resizeImage)
    {
    // check the file signature and deduce its format
    FREE_IMAGE_FORMAT imgFormat = FreeImage_GetFileType(filename, 0);
    if(imgFormat == FIF_UNKNOWN)
        imgFormat = FreeImage_GetFIFFromFilename(filename);
    if(imgFormat == FIF_UNKNOWN)
        return false;

    // load image
    FIBITMAP *img = FreeImage_Load(imgFormat, filename, BMP_DEFAULT);
    if(img == NULL) return false;
    uint imgW = FreeImage_GetWidth(img);
    uint imgH = FreeImage_GetHeight(img);
    //FREE_IMAGE_COLOR_TYPE imgColorType = FreeImage_GetColorType(img);

    // convert image to 32 bpp if not
    if(FreeImage_GetBPP(img) != 32)
        {
        FIBITMAP *newImg = FreeImage_ConvertTo32Bits(img);
        if(img == NULL) return false;
        FreeImage_Unload(img);
        img = newImg;
        }

    // resize if requested
    if((imgW != width || imgH != height) && resizeImage)
        {
        FIBITMAP *newImg = FreeImage_Rescale(img, width, height, FILTER_CATMULLROM);
        if(img == NULL) return false;
        FreeImage_Unload(img);
        img = newImg;
        imgW = width;
        imgH = height;
        }
    else
        {
        width = imgW;
        height = imgH;
        // re-create buffer
        if(srfData.buffer)
            delete srfData.buffer;
        srfData.buffer = new uchar[imgW * imgH * 4];
        }

    // load buffer
    uchar *bp = srfData.buffer;
    uint x, y;

    for(y = 0; y < imgH; y++)
        {
        uchar* bits = FreeImage_GetScanLine(img, y);
        for(x = 0; x < imgW; x++)
            {
            bp[0] = bits[FI_RGBA_RED];
            bp[1] = bits[FI_RGBA_GREEN];
            bp[2] = bits[FI_RGBA_BLUE];
            bp[3] = bits[FI_RGBA_ALPHA];

            bp += 4;
            bits += 4;
            }
        }

    // unload image
    FreeImage_Unload(img);

    return true;
    }

// -----------------------------------------------------------------------------

bool ViewSurface::save(const char* filename)
    {
    FIBITMAP *img = FreeImage_Allocate(width, height, 24);

    // load buffer
    uchar *bp = srfData.buffer;
    uint x, y;

    for(y = 0; y < height; y++)
        {
        uchar* bits = FreeImage_GetScanLine(img, y);
        for(x = 0; x < width; x++)
            {
            bits[FI_RGBA_RED] = bp[0];
            bits[FI_RGBA_GREEN] = bp[1];
            bits[FI_RGBA_BLUE] = bp[2];

            bp += 4;
            bits += 3;
            }
        }

    if(FreeImage_Save(FIF_JPEG, img, filename) == FALSE)
        return false;

    FreeImage_Unload(img);
    return true;
    }

// -----------------------------------------------------------------------------

void ViewSurface::updatePBO()
    {
    uchar *mappedBuffer = NULL;
    int nextIndex = 0;

    if(pboMode > 0)
        {
        // increment current index first then get the next index
        // "index" is used to copy pixels from a PBO to a texture object
        // "nextIndex" is used to update pixels in a PBO
        index = (index + 1) % 2;
        if(pboMode == 1) // with 1 PBO
            nextIndex = index;
        else if(pboMode == 2) // with 2 PBO
            nextIndex = (index + 1) % 2;

        // bind the texture and PBO
        glBindTexture(glTextureType, textureId);
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, srfData.pbo_ids[index]);

        // copy pixels from PBO to texture object - use offset instead of pointer
        glTexSubImage2D(glTextureType, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        // bind PBO to update texture source
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, srfData.pbo_ids[nextIndex]);

        // discard the previous data in PBO
        glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, srfData.bufferSize, 0, GL_STREAM_DRAW_ARB);

        // map the buffer object into client's memory
        mappedBuffer = (uchar*) glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
        if(mappedBuffer)
            {
            // update data directly on the mapped buffer
            memcpy(mappedBuffer, srfData.buffer, srfData.bufferSize);

            // release pointer to mapping buffer
            glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB);
            }

        // release PBOs with ID 0 after use
        glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
        }
    else
        {
        // start to copy pixels from system memory to texture object
        glBindTexture(glTextureType, textureId);
        glTexSubImage2D(glTextureType, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, srfData.buffer);
        }
    }

// -----------------------------------------------------------------------------

void ViewSurface::paintSubSurfaces()
    {
    ViewSurface *surface = NULL;
    vmml::vec3f pos;
    uint w, h;

    if(GLEE_ARB_framebuffer_object)
        {
        // set the draw buffer
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fboIdCopy);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->textureIdCopy, 0);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        // draw first a copy of main surface
        glBindFramebuffer(GL_READ_FRAMEBUFFER, this->fboId);
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->textureId, 0);
        glReadBuffer(GL_COLOR_ATTACHMENT1);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);

        // now draw subsurfaces
        std::vector<ViewSurface*>::iterator it;
        for(it = subSurfaces.begin(); it != subSurfaces.end(); ++it)
            {
            surface = *it;
            pos = surface->getPosition();
            w = surface->getWidth();
            h = surface->getHeight();
            //
            glBindFramebuffer(GL_READ_FRAMEBUFFER, surface->getGLFboId());
            glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, surface->getGLTextureId(), 0);
            glReadBuffer(GL_COLOR_ATTACHMENT1);
            glBlitFramebuffer(0, 0, w, h, pos.x(), pos.y(), pos.x() + w, pos.y() + h, GL_COLOR_BUFFER_BIT, GL_LINEAR);
            }
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    else
        {
        printf("FBO NOT SUPPORTED!!!\n");
        }

    ViewUtility::checkGL("surface.paintSubSurfaces");
    }

// -----------------------------------------------------------------------------

bool ViewSurface::glLoadTexture(uint *id, bool loadBuffer)
    {
    glGenTextures(1, id);
    glBindTexture(glTextureType, *id);
    
    if(this->image->imageType == EOS_IMAGETYPE_RGBA_BUFFER)
        {
        if(!loadBuffer)
            glTexImage2D(glTextureType, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        else
            glTexImage2D(glTextureType, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, srfData.buffer);
        }
    else if(this->image->imageType == EOS_IMAGETYPE_RGB_BUFFER)
        {
        if(!loadBuffer)
            glTexImage2D(glTextureType, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
        else
            glTexImage2D(glTextureType, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, srfData.buffer);
        }
    else if(this->image->imageType == EOS_IMAGETYPE_CAIROSURFACE)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, 4, this->width, this->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, srfData.buffer);
        }
    
    glTexParameteri(glTextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(glTextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return ViewUtility::checkGL("surface.glLoadTexture");
    }

// -----------------------------------------------------------------------------

void ViewSurface::glUnloadTexture(uint *id)
    {
    if(glIsTexture(*id))
        glDeleteTextures(1, id);

    ViewUtility::checkGL("surface.glUnloadTexture");
    }

// -----------------------------------------------------------------------------

bool ViewSurface::glLoadFBO()
    {
    glGenFramebuffers(1, &fboId);
    glGenFramebuffers(1, &fboIdCopy);

    return ViewUtility::checkGL("surface.glLoadFBO");
    }

// -----------------------------------------------------------------------------

void ViewSurface::glUnloadFBO()
    {
    if(glIsFramebuffer(fboId))
        glDeleteFramebuffers(1, &fboId);
    if(glIsFramebuffer(fboIdCopy))
        glDeleteFramebuffers(1, &fboIdCopy);

    ViewUtility::checkGL("surface.glUnloadFBO");
    }

// -----------------------------------------------------------------------------

void ViewSurface::glCopyTexture(uint inId, uint outId)
    {
    uint nFbo = 0;

    // bind destination
    glBindTexture(glTextureType, outId);

    // create and bind frame buffer
    glGenFramebuffers(1, &nFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, nFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, glTextureType, inId, 0);

    // copy texture from frame buffer
    glCopyTexSubImage2D(glTextureType, 0, 0, 0, 0, 0, width, height);

    // cleanup
    glBindTexture(glTextureType, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &nFbo);
    }

// -----------------------------------------------------------------------------

uint ViewSurface::glPow2(int value)
    {
    int n = 1;
    while(n < value)
        n <<= 1;
    return n;
    }
