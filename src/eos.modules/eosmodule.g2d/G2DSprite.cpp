/*
-----------------------------------------------------------------------------
This source file is part of EOS Multimedia Scripting System
For the latest info, see https://github.com/ultimoistante/eos-multimedia-scripting

The MIT License (MIT)

Copyright (c) 2014-2019 of Salvatore Carotenuto aka ultimoistante of StartupSolutions
                           (mailto: ultimoistante@gmail.com | www.startupsolutions.it)
                       and Alessandro Rasulo (mailto: alex9978@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/


#include "G2DSprite.h"
#include "G2D.h"

// -----------------------------------------------------------------------------

G2DSprite::G2DSprite()
    {
    currentAnim = NULL;
    animTime = 0;
    pos.set(0, 0);
    zoom.set(1, 1);
    hotSpot.set(0, 0);
    flipX = false;
    flipY = false;
    alpha = 1.0f;
    rotationAngle = 0;
    hotSpotId = HOTSPOT_LEFTTOP;
    controller = NULL;
    }

// -----------------------------------------------------------------------------

G2DSprite::~G2DSprite()
    {
    // delete anims
    std::map<std::string, G2DSpriteAnim*>::iterator it;
    for(it=anims.begin(); it!=anims.end(); it++)
        delete it->second;
    anims.clear();
    }

// -----------------------------------------------------------------------------

void G2DSprite::paint()
    {
    // return if no current animation
    if(currentAnim == NULL)
        return;

    // update position
    if(controller != NULL)
        {
        pos.set(controller->x, -controller->y);
        rotationAngle = controller->angle;
        }

    // get the current frame
    g2d_sprite_frame *sf = currentAnim->frames[currentAnim->currentFrame];

    // if the time to get the next frame has come, do it
    if(sf->timeDelay > 0 && G2D::getInstance()->getTimeMilliseconds()-animTime > sf->timeDelay)
        {
        // update animation time
        animTime = G2D::getInstance()->getTimeMilliseconds();

        // if anim is not terminated get next frame
        if(!currentAnim->terminated)
            currentAnim->currentFrame++;

        // if the animation frames was ended do loop if requested
        if(currentAnim->currentFrame >= currentAnim->frames.size())
            {
            if(currentAnim->loopStart >= 0)
                currentAnim->currentFrame = currentAnim->loopStart;
            else
                {
                currentAnim->terminated = true;
                currentAnim->currentFrame = currentAnim->frames.size()-1;
				}
				// send message to me!
				char cmdBuffer[2048];
				sprintf(cmdBuffer, "eos.message.init(\"g2d\") \
                                    eos.message.pushData(\"spriteName\", \"%s\") \
                                    eos.message.pushData(\"animName\", \"%s\") \
                                    eos.message.pushData(\"animStatus\", \"%s\") \
                                    eos.message.send(\"%s\")",
                                    getProperty("Name"),
                                    currentAnim->name.c_str(),
                                    "end",
                                    getProperty("ProcessToNotify"));
				EOSLuaRuntime::getInstance()->executeLuaChunk(cmdBuffer);
				}
    }

    // paint
    int u1=0, v1=0, u2=0, v2=0;
    if(!flipX) { u1=0; u2=2; } else { u1=2; u2=0; }
    if(!flipY) { v1=1; v2=3; } else { v1=3; v1=1; }

    calculateHotSpot();

    glLoadIdentity();
    glTranslatef(pos.x() - (hotSpot.x() * zoom.x()), pos.y() - (hotSpot.y() * zoom.y()), 0);
    glRotatef(rotationAngle, 0, 0, 1);    
    glBindTexture(GL_TEXTURE_2D, currentAnim->glTexture);
    glBegin(GL_QUADS);
        glColor4f(1, 1, 1, alpha);
        glTexCoord2f(sf->texcoord[u1], sf->texcoord[v2]); glVertex2f(0, 0);
        glTexCoord2f(sf->texcoord[u2], sf->texcoord[v2]); glVertex2f(currentAnim->frameWidth*zoom.x(), 0);
        glTexCoord2f(sf->texcoord[u2], sf->texcoord[v1]); glVertex2f(currentAnim->frameWidth*zoom.x(), currentAnim->frameHeight*zoom.y());
        glTexCoord2f(sf->texcoord[u1], sf->texcoord[v1]); glVertex2f(0, currentAnim->frameHeight*zoom.y());
    glEnd();    
    }

// -----------------------------------------------------------------------------

bool G2DSprite::addAnim(const char *name, eos_imagedata *data, uint frames, uint frameWidth, uint frameHeight, float timeDelay, uint loopStart)
{
    // create new spriteanim
    G2DSpriteAnim *sa = new G2DSpriteAnim();
    if(!sa)
        return false;

    if(data == NULL)
        return false;    

    if(data->glTextureHandle == 0 && data->buffer != NULL)
        glLoadTextureFromBuffer(&data->glTextureHandle, data->width, data->height, data->buffer);

    sa->name = name;
    sa->image = data;
    sa->imageWidth = data->width;
    sa->imageHeight = data->height;
    sa->glTexture = data->glTextureHandle;
    sa->frameWidth = frameWidth;
    sa->frameHeight = frameHeight;
    sa->currentFrame = 0;
    sa->loopStart = loopStart;
    sa->terminated = false;    

    // u-v calculation
    int onrow = sa->image->width / frameWidth,
        ir = 0,
        ic = 0;

    float ustep = (float)frameWidth / sa->imageWidth,
          vstep = (float)frameHeight / sa->imageHeight;

//    LOGINFO("frames:%i w:%i h:%i onrow:%i ustep:%f vstep:%f", frames, sa->image->w, sa->image->h, onrow, ustep, vstep);

    // create frames
    for(uint i=0; i<frames; i++)
        {
        // check number of frames!
        if(ir * vstep >= 1)
            {
            LOGERROR("[eos.g2d] sprite.addAnim() - Wrong number of frames in '%s'!", name);
            break;
            }

        g2d_sprite_frame *sf = new g2d_sprite_frame;
        sf->texcoord[0] = ic * ustep;           // u1
        sf->texcoord[1] = ir * vstep;           // v1
        sf->texcoord[2] = (ic * ustep) + ustep; // u2
        sf->texcoord[3] = (ir * vstep) + vstep; // v2
        sf->timeDelay = timeDelay;
        sa->frames.push_back(sf);

        //printf("u1:%f v1:%f u2:%f v2:%f\n", sf->texcoord[0], sf->texcoord[1], sf->texcoord[2], sf->texcoord[3]);

        ic++;
        if(ic >= onrow)
            {
            ic = 0;
            ir++;
            }
        }  

    std::string animname = name;
    anims.insert(std::pair<std::string, G2DSpriteAnim*>(animname, sa));

    // check current anim
    if(getCurrentAnim() == NULL)
        setCurrentAnim(name);

    return true;
    }

// -----------------------------------------------------------------------------

bool G2DSprite::setFixedFrame(eos_imagedata *data)
{
    // add the default anim
    if(!addAnim("default", data, 1, data->width, data->height, 0, 0))
        return false;

    // set anim
    if(!setCurrentAnim("default"))
        return false;

    return true;
}

// -----------------------------------------------------------------------------

bool G2DSprite::animGoToFrame(uint frameNumber)
	{
	if(currentAnim == NULL)
		return false;
	uint frameCount = currentAnim->frames.size();
	if(frameNumber >= frameCount && frameCount > 0)
		currentAnim->currentFrame = frameCount-1;
	else
		currentAnim->currentFrame = frameNumber;
	//
	return true;
	}

// -----------------------------------------------------------------------------

bool G2DSprite::setCurrentAnim(const char *name)
{
    std::map<std::string, G2DSpriteAnim*>::iterator it;
    it = anims.find(name);
    if(it != anims.end())
    {
        if(currentAnim != it->second)
        {
            currentAnim = it->second;
            animTime = G2D::getInstance()->getTimeMilliseconds();
            currentAnim->currentFrame = 0;
            currentAnim->terminated = false;
        }
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------

char *G2DSprite::getCurrentAnim()
{
    if(currentAnim == NULL)
        return NULL;

    return (char *)currentAnim->name.c_str();
}

// -----------------------------------------------------------------------------

void G2DSprite::setHotSpot(int hotspot)
    {
    this->hotSpot = hotspot;
    }

// -----------------------------------------------------------------------------

void G2DSprite::setPosition(int x, int y)
    {
    pos.set(x, y);
    }

// -----------------------------------------------------------------------------

void G2DSprite::setRotation(float angle)
    {
    rotationAngle = angle;
    }

// -----------------------------------------------------------------------------

void G2DSprite::bindToObject(EOSObject *obj)
    {
    if(strcmp(obj->getType(), "2D_OBJECT") == 0)
        controller = (eos_2d_object*)obj->getUserData();
    }

// -----------------------------------------------------------------------------

Vector2f G2DSprite::getPosition()
    {
    return pos;
    }

// -----------------------------------------------------------------------------

float G2DSprite::getRotation()
    {
    return rotationAngle;
    }

// -----------------------------------------------------------------------------

uint G2DSprite::getWidth()
    {
    if(currentAnim != NULL)
        return currentAnim->frameWidth * zoom.x();

    return 0;
    }

// -----------------------------------------------------------------------------

uint G2DSprite::getHeight()
    {
    if(currentAnim != NULL)
        return currentAnim->frameHeight * zoom.y();

    return 0;
    }

// -----------------------------------------------------------------------------

void G2DSprite::calculateHotSpot()
{
    if(currentAnim == NULL)
        return;

    switch(hotSpotId)
    {
    case HOTSPOT_LEFTTOP:       hotSpot.set(0, 0); break;
    case HOTSPOT_CENTERTOP:     hotSpot.set(currentAnim->frameWidth/2, 0); break;
    case HOTSPOT_RIGHTTOP:      hotSpot.set(currentAnim->frameWidth, 0); break;
    case HOTSPOT_CENTER:        hotSpot.set(currentAnim->frameWidth/2, currentAnim->frameHeight/2); break;
    case HOTSPOT_LEFTBOTTOM:    hotSpot.set(0, currentAnim->frameHeight); break;
    case HOTSPOT_CENTERBOTTOM:  hotSpot.set(currentAnim->frameWidth/2, currentAnim->frameHeight); break;
    case HOTSPOT_RIGHTBOTTOM:   hotSpot.set(currentAnim->frameWidth, currentAnim->frameHeight); break;
    default:                    hotSpot.set(0, 0); break;
    }
}

// -----------------------------------------------------------------------------

bool G2DSprite::glLoadTextureFromBuffer(uint *id, uint width, uint height, void *buffer)
    {
 	glGenTextures(1, id);
 	glBindTexture(GL_TEXTURE_2D, *id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return G2DUtility::checkGL("sprite.glLoadTextureFromBuffer");
    }

// -----------------------------------------------------------------------------
