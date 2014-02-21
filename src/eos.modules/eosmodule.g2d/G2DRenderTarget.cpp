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


#include "G2DRenderTarget.h"

// -----------------------------------------------------------------------------

G2DRenderTarget::G2DRenderTarget(uint width, uint height)
    {
    this->width = width;
    this->height = height;

    // create texture
    glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // create fbo
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);
    }

// -----------------------------------------------------------------------------

G2DRenderTarget::~G2DRenderTarget()
    {
    if(glIsFramebuffer(fboId))
        glDeleteFramebuffers(1, &fboId);

    if(glIsTexture(textureId))
        glDeleteTextures(1, &textureId);
    }

// -----------------------------------------------------------------------------

void G2DRenderTarget::update()
    {  
    glClearDepth(0);
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glShadeModel(GL_SMOOTH);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, (GLdouble)width, (GLdouble)height, 0.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);    
    
    // ------------------

    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    sort(elements.begin(), elements.end(), G2DElementCompareAscending());

    std::map<std::string, G2DElement*>::iterator it;
    for(it=elements.begin(); it!=elements.end(); it++)
        it->second->paint();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    G2DUtility::checkGL("G2DRenderTarget::update");
    G2DUtility::checkGLFramebuffer(GL_FRAMEBUFFER);

    // ------------------

    glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();
    }

// -----------------------------------------------------------------------------

eos_imagedata* G2DRenderTarget::getSharedGLTexture()
    {
    shared_imagedata.width = width;
    shared_imagedata.height = height;
    shared_imagedata.glTextureHandle = textureId;
    return &shared_imagedata;
    }

// -----------------------------------------------------------------------------

bool G2DRenderTarget::addElement(const char* name, G2DElement* element)
    {
    std::string elementName = name;
    if(!element)
        return false;
    elements.insert(std::pair<std::string, G2DElement*>(elementName, element));
    return true;
    }

// -----------------------------------------------------------------------------

bool G2DRenderTarget::removeElement(const char* name)
    {
    std::string elementName = name;
    std::map<std::string, G2DElement*>::iterator it;
    it = elements.find(elementName);
    if(it != elements.end())
        {
        delete it->second;
        elements.erase(it);
        return true;
        }

    return false;
    }

// -----------------------------------------------------------------------------

G2DElement *G2DRenderTarget::getElement(const char* name)
    {
    std::string elementName = name;
    std::map<std::string, G2DElement*>::iterator it;
    it = elements.find(elementName);
    if(it != elements.end())
        return it->second;

    return NULL;
    }

// -----------------------------------------------------------------------------
