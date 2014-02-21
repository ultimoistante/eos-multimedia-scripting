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

#ifndef PICASSO_H
#define	PICASSO_H

#include "../modulesdk/EOSPrecisionTimer.h"
#include "../modulesdk/EOSObject.h"
#include "G2DRenderTarget.h"
#include "G2DElement.h"
#include "G2DSprite.h"
#include <stdio.h>
#include <map>
#include <vector>
#include <GLee.h>

#include <cairo/cairo.h>

class G2D
    {
    public:
        G2D();
        virtual ~G2D();
        static G2D *getInstance();
        void shutdown();
        void update();

        bool createRenderTarget(const char* name, uint width, uint height);
        //bool updateRenderTarget(const char* name);
        void setCurrentRenderTarget(const char* name);
        G2DRenderTarget* getRenderTarget(const char* name);
        G2DRenderTarget* getCurrentRenderTarget();
        EOSObject* createSessionObject(const char* name);

        bool createSprite(const char* name);
        G2DElement* getElement(const char* name);

        float getTimeSeconds()      { return timer->getSeconds(); }
        ulong getTimeMilliseconds() { return timer->getMilliseconds(); }

    private:
        static bool instanceFlag;
        static G2D *instance;

        EOSPrecisionTimer *timer;

        G2DRenderTarget* currentRenderTarget;
        std::map<std::string, G2DRenderTarget*> renderTargets;
        std::vector<EOSObject*> sessionObjects;
    };

#endif
