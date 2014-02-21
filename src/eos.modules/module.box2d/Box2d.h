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

#ifndef BOX2D_H_
#define	BOX2D_H_

#include "../modulesdk/EOSPrecisionTimer.h"
#include "../modulesdk/EOSObject.h"
#include "Box2dDefines.h"
#include "Box2dBody.h"
#include <Box2D.h>
#include <stdio.h>
#include <map>
#include <vector>

class Box2d
    {
    public:
        Box2d();
        virtual ~Box2d();
        static Box2d *getInstance();
        void shutdown();
        void update(float frameTime);
        
        b2World* getWorld() { return world; }

        EOSObject* createBody(const char* name, float friction, float restitution, float density);
        Box2dBody* getBody(const char* name);

    private:
        static bool instanceFlag;
        static Box2d *instance;

        b2World*    world;

        b2AABB      worldAABB;
        b2Vec2      gravity;
        bool        doSleep;

        std::map<std::string, Box2dBody*> bodies;
    };

#endif

