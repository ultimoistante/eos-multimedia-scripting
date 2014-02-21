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

#include "Box2d.h"

// -----------------------------------------------------------------------------

bool Box2d::instanceFlag = false;
Box2d* Box2d::instance = NULL;

// -----------------------------------------------------------------------------

Box2d::Box2d()
    {
    // define world bounding box
    worldAABB.lowerBound.Set(-5000.0f, -5000.0f);
    worldAABB.upperBound.Set( 5000.0f,  5000.0f);

    // default gravity
    gravity.Set(0.0f, -9.81f);

    // do sleep
    doSleep = true;

    // create world
    world = new b2World(worldAABB, gravity, doSleep);
    }

// -----------------------------------------------------------------------------

Box2d::~Box2d()
{
    // delete world
    if(world)
        delete world;

    // delete bodies
    std::map<std::string, Box2dBody*>::iterator it;
    for(it=bodies.begin(); it!=bodies.end(); it++)
        delete it->second;
}

// -----------------------------------------------------------------------------

Box2d* Box2d::getInstance()
	{
	if(!instanceFlag)
		{
		instance = new Box2d();
		instanceFlag = true;
		}
	return instance;
	}

// -----------------------------------------------------------------------------

void Box2d::shutdown()
    {
    if(instance)
        delete instance;

    instanceFlag = false;
    }

// -----------------------------------------------------------------------------

void Box2d::update(float frameTime)
    {
    world->Step(frameTime, 10, 10);
    //
    std::map<std::string, Box2dBody*>::iterator it;
    for(it=bodies.begin(); it!=bodies.end(); it++)
        it->second->fillSharedObject();
    }

// -----------------------------------------------------------------------------

EOSObject* Box2d::createBody(const char* name, float friction, float restitution, float density)
    {
    Box2dBody* body = new Box2dBody(friction, restitution, density);
    if(body != NULL)
        {
        std::string bName = name;
        bodies.insert(std::pair<std::string, Box2dBody*>(bName, body));
        //
        EOSObject *obj = new EOSObject();
        obj->setType("2D_OBJECT");
        obj->setUserData(body->getSharedObject());
        obj->setUserDataSize(sizeof(eos_2d_object));
        return obj;
        }

    return NULL;
    }

// -----------------------------------------------------------------------------

Box2dBody* Box2d::getBody(const char* name)
    {
    std::string bName = name;
    std::map<std::string, Box2dBody*>::iterator it;
    it = bodies.find(bName);
    if(it != bodies.end())
        return it->second;

    return NULL;
    }

// -----------------------------------------------------------------------------

