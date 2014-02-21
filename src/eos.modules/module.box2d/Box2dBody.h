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

#ifndef BOX2DBODY_H
#define	BOX2DBODY_H

#include <Box2D.h>
#include "../modulesdk/eos_data_containers.h"

class Box2dBody
    {
    public:
        Box2dBody(float friction, float restitution, float density);
        virtual ~Box2dBody();

        void addCircleShape(float x, float y, float radius);
        void addBoxShape(float x, float y, float w, float h);

        void setPosition(float x, float y);

        void applyForce(float forceX, float forceY, float pointX, float pointY);
        void applyTorque(float torque);
        void applyImpulse(float impulseX, float impulseY, float pointX, float pointY);

        eos_2d_object* getSharedObject() { return &shared_object; }
        void fillSharedObject();

    private:
        b2Body*         body;

        float           friction,
                        restitution,
                        density;

        eos_2d_object   shared_object;
    };

#endif

