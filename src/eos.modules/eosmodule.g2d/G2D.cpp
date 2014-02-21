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


#include "G2D.h"

// -----------------------------------------------------------------------------

bool G2D::instanceFlag = false;
G2D* G2D::instance = NULL;

// -----------------------------------------------------------------------------

G2D::G2D()
    {
    timer = new EOSPrecisionTimer();
    currentRenderTarget = NULL;
    }

// -----------------------------------------------------------------------------

G2D::~G2D()
    {
    // delete render targets
    std::map<std::string, G2DRenderTarget*>::iterator it;
    for(it=renderTargets.begin(); it!=renderTargets.end(); it++)
        delete it->second;
    renderTargets.clear();

    // delete session objects
    for(uint i=0; i<sessionObjects.size(); i++)
		delete sessionObjects[i];
	sessionObjects.clear();
    }

// -----------------------------------------------------------------------------

G2D* G2D::getInstance()
	{
	if(!instanceFlag)
		{
		instance = new G2D();
		instanceFlag = true;
		}
	return instance;
	}

// -----------------------------------------------------------------------------

void G2D::shutdown()
    {
    if(instance)
        delete instance;

    instanceFlag = false;
    }

// -----------------------------------------------------------------------------

void G2D::update()
    {
    // update render targets
    std::map<std::string, G2DRenderTarget*>::iterator it;
    for(it=renderTargets.begin(); it!=renderTargets.end(); it++)
        it->second->update();
    }

// -----------------------------------------------------------------------------

bool G2D::createRenderTarget(const char* name, uint width, uint height)
    {
    std::string targetName = name;
    G2DRenderTarget *target = new G2DRenderTarget(width, height);
    if(!target)
        return false;
    renderTargets.insert(std::pair<std::string, G2DRenderTarget*>(targetName, target));
    // set the current render target if none is present
    if(getCurrentRenderTarget() == NULL)
        setCurrentRenderTarget(targetName.c_str());
    return true;
    }

// -----------------------------------------------------------------------------

/*bool G2D::updateRenderTarget(const char* name)
    {
    G2DRenderTarget *rt = getRenderTarget(name);
    if(rt)
        rt->update();
    }*/

// -----------------------------------------------------------------------------

void G2D::setCurrentRenderTarget(const char* name)
    {
    currentRenderTarget = getRenderTarget(name);
    }

// -----------------------------------------------------------------------------

G2DRenderTarget* G2D::getRenderTarget(const char* name)
    {
    std::string rname = name;
    std::map<std::string, G2DRenderTarget*>::iterator it;
    it = renderTargets.find(rname);
    if(it != renderTargets.end())
        return it->second;
    else
        return NULL;
    }

// -----------------------------------------------------------------------------

G2DRenderTarget* G2D::getCurrentRenderTarget()
    {
    return currentRenderTarget;
    }

// -----------------------------------------------------------------------------

EOSObject* G2D::createSessionObject(const char* name)
    {
    EOSObject *obj = NULL;
    G2DRenderTarget *rt = getRenderTarget(name);
    if(rt)
        {
        obj = new EOSObject();
        obj->setType("GL_TEXTURE_HANDLE");
        obj->setUserData(rt->getSharedGLTexture());
        obj->setUserDataSize(sizeof(eos_imagedata));
        }
    return obj;
    }

// -----------------------------------------------------------------------------

 bool G2D::createSprite(const char* name)
    {
    G2DSprite *spr = new G2DSprite();
    if(spr != NULL)
        {
        G2DRenderTarget *ct = getCurrentRenderTarget();
        if(ct != NULL)
            {
            ct->addElement(name, spr);
            return true;
            }
        }

    return false;
    }

// -----------------------------------------------------------------------------

G2DElement* G2D::getElement(const char* name)
    {
    if(currentRenderTarget == NULL)
        return NULL;

    return currentRenderTarget->getElement(name);
    }
