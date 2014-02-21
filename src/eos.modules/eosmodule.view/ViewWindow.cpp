
#include "ViewWindow.h"
#include "View.h"

// -----------------------------------------------------------------------------

ViewWindow::ViewWindow(uint screen, uint width, uint height, uint bpp, uint antialiasing, bool fullscreen)
    {
    this->screen = screen;
    this->width = width;
    this->height = height;
    this->bpp = bpp;
    this->antialiasing = antialiasing;
    this->fullscreen = fullscreen;
    //
    this->timer = new EOSPrecisionTimer();
    this->framesUpdated = 0;
    this->fpsCounter = 0;
    //
    this->prof = new EOSProfiler("view");
    }

// -----------------------------------------------------------------------------

ViewWindow::~ViewWindow()
    {
    if(prof)
        delete prof;

    // delete registered surfaces
    surfaces.clear();

    // destroy window
    ViewPlatformSupport::destroyWindow(&winData);

    // destroy timer
    if(this->timer)
        delete timer;
    }

// -----------------------------------------------------------------------------

bool ViewWindow::open()
    {
    // make window
    if(!ViewPlatformSupport::makeWindow(screen, "EOS Window", 0, 0, width, height, fullscreen, &winData))
        return false;

    //Initialize OpenGL
    if(!this->initGL())
        return false;

    // resize the initial window
    this->resize(this->width, this->height);

    // release context to other modules
    releaseContext();

    return true;
    }

// -----------------------------------------------------------------------------

void ViewWindow::resize(uint width, uint height)
    {
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 10000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    this->width = width;
    this->height = height;

    ViewUtility::checkGL("window.resize");
    }

// -----------------------------------------------------------------------------

bool ViewWindow::update()
    {
    prof->beginProfiling("window.update");

    // process wm events
    ViewPlatformSupport::processWindowEvents(&winData);

    // resize window if requested
    if(winData.toBeResized)
        {
        resize(winData.width, winData.height);
        winData.toBeResized = false;
        }

    // set primary context
    takeContext();

    // clear the window and the depth buffer
    glClearColor(0.4f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // updates all registered and active surfaces
    std::vector<uint>::iterator it;
    for(it = surfaces.begin(); it != surfaces.end(); it++)
        {
        ViewSurface* surface = View::getInstance()->getSurfaceFromId(*it);
        if(surface != NULL)
            surface->paint();
        }

    // swaps window buffers (true for wait VSync)
    ViewPlatformSupport::swapBuffers(&winData, false);

    // set secondary context
    releaseContext();

    // fps
    if(timer->getSeconds() >= 1.0f)
        {
        fpsCounter = framesUpdated;
        framesUpdated = 0;
        timer->reset();
        }
    framesUpdated++;

    prof->endProfiling("window.update");

    if(winData.toBeClosed)
        return false;

    return true;
    }

// -----------------------------------------------------------------------------

void ViewWindow::setCaption(const char* text)
    {
    if(text != NULL)
        {
        this->caption = text;
        winData.windowTitle = (char*) this->caption.c_str();
        ViewPlatformSupport::setWindowCaption(&winData);
        }
    }

// -----------------------------------------------------------------------------

void ViewWindow::takeContext()
    {
    ViewPlatformSupport::setContext(&winData, 1);
    }

// -----------------------------------------------------------------------------

void ViewWindow::releaseContext()
    {
    ViewPlatformSupport::setContext(&winData, 2);
    }

// -----------------------------------------------------------------------------

bool ViewWindow::registerSurface(uint surfaceId)
    {
    if(surfaceId >= 0)
        {
        surfaces.push_back(surfaceId);
        return true;
        }
    else
        return false;
    }

// -----------------------------------------------------------------------------

bool ViewWindow::unregisterSurface(uint surfaceId)
    {
    std::vector<uint>::iterator it = surfaces.begin();
    while(it != surfaces.end())
        {
        if(*it == surfaceId)
            {
            surfaces.erase(it);
            return true;
            }
        else
            ++it;
        }
    return false;
    }

// -----------------------------------------------------------------------------

bool ViewWindow::initGL()
    {
    GLfloat lightAmbient[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat lightPosition[] = {0.0f, 0.0f, 2.0f, 1.0f};

    glShadeModel(GL_SMOOTH);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);

    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 10000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glAlphaFunc(GL_GREATER, 0.1);
    glEnable(GL_ALPHA_TEST);

    /* glEnable(GL_LIGHTING);
     glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
     glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
     glLightfv(GL_LIGHT1, GL_POSITION,lightPosition);
     glEnable(GL_LIGHT1);*/

    return ViewUtility::checkGL("window.initGL");
    }

// -----------------------------------------------------------------------------
