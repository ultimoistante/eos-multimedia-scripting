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

/* 
 * File:   RigidBody.h
 * Author: ultimoistante
 *
 * Created on September 26, 2009, 5:33 PM
 */

#ifndef _RIGIDBODY_H
#define	_RIGIDBODY_H

#include <stdio.h>
#include "btBulletDynamicsCommon.h"
#include "EOSObject.h"
#include "eos_data_containers.h"


class RigidBody
    {
    public:
        RigidBody(btDiscreteDynamicsWorld* dynamicsWorld, btCollisionShape* collisionShape, btScalar mass);
        ~RigidBody();
        //
        void          updateSessionObjectData();
        EOSObject*    getSessionObject();
        //
        bool          isDynamic() { return this->mass != 0.f; }
        void          setMass(btScalar mass);
        btScalar      getMass() const    { return mass; }
        btRigidBody*  getBulletBody()      { return this->body; }
        btVector3*    getPosition();
        void          setPosition(btScalar x, btScalar y, btScalar z);
        btQuaternion* getOrientation();
        void          setOrientation(btScalar x, btScalar y, btScalar z, btScalar w);
        //
        //
    private:
        btDiscreteDynamicsWorld* dynamicsWorld;
        btRigidBody*             body;
        btCollisionShape*        shape;
        btDefaultMotionState*    motionState;
        btTransform              bodyTransform;
        btScalar                 mass;
        btVector3                localInertia;
        //
        EOSObject*               sessionObject;
        eos_3dobject             sessionObjectData;
    };

#endif	/* _RIGIDBODY_H */

