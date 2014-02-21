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
 * File:   BulletPhysics.cpp
 * Author: ultimoistante
 * 
 * Created on September 26, 2009, 5:10 PM
 */

#include "BulletPhysics.h"
#include "EOSLuaRuntime.h"

// -----------------------------------------------------------------------------

// static members
const int BulletPhysics::SHAPE_BOX      = 1;
const int BulletPhysics::SHAPE_SPHERE   = 2;

// -----------------------------------------------------------------------------

BulletPhysics::BulletPhysics()
	{
	this->collisionConfiguration = NULL;
	this->dispatcher             = NULL;
	this->overlappingPairCache   = NULL;
	this->solver                 = NULL;
	this->dynamicsWorld          = NULL;
	//
	this->init();
	}

// -----------------------------------------------------------------------------

BulletPhysics::~BulletPhysics()
	{
	}

// -----------------------------------------------------------------------------

void BulletPhysics::init()
	{
	// default code taken directly from HelloWorld bullet demo
	//
	// collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	this->collisionConfiguration = new btDefaultCollisionConfiguration();

	// use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	this->dispatcher = new btCollisionDispatcher(this->collisionConfiguration);

	// btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	this->overlappingPairCache = new btDbvtBroadphase();

	// the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	this->solver = new btSequentialImpulseConstraintSolver;

	// *the* dynamics world
	this->dynamicsWorld = new btDiscreteDynamicsWorld(this->dispatcher, this->overlappingPairCache, this->solver, this->collisionConfiguration);

	// default gravity vector: earth's 1g, approximated from 9.8 to 10.0
	this->dynamicsWorld->setGravity(btVector3(0, -10, 0));
	}

// -----------------------------------------------------------------------------

void BulletPhysics::update(float timeStep)
	{
	this->dynamicsWorld->stepSimulation(timeStep, 10);
	}

// -----------------------------------------------------------------------------

void BulletPhysics::cleanup()
	{
	// deletes all instanced bodies
	std::map<std::string, RigidBody*>::iterator it;
	for(it = bodies.begin(); it != bodies.end(); it++)
		delete it->second;
	bodies.clear();
	EOSLuaRuntime::getInstance()->logInfoMessage("[eos.bullet] All bodies deleted.");
	//
	// deletes all dynamics world related objects
	delete this->dynamicsWorld;
	delete this->solver;
	delete this->overlappingPairCache;
	delete this->dispatcher;
	delete this->collisionConfiguration;
	}

	// -----------------------------------------------------------------------------

btCollisionShape* BulletPhysics::primitiveShape(int shapeType, btScalar xSizeOrRadius, btScalar ySize, btScalar zSize)
	{
	btCollisionShape* newShape = NULL;
	//
	switch(shapeType)
		{
		case BulletPhysics::SHAPE_BOX:
			newShape = new btBoxShape(btVector3(xSizeOrRadius, ySize, zSize));
			break;
		case BulletPhysics::SHAPE_SPHERE:
			newShape = new btSphereShape(xSizeOrRadius);
			break;
		}
	//
	return newShape;
	}

// -----------------------------------------------------------------------------

RigidBody* BulletPhysics::bodyByName(const char* bodyName)
	{
	RigidBody* body = NULL;
	if(bodyName != NULL)
		{
		std::map<std::string, RigidBody*>::iterator it;
		it = bodies.find(bodyName);
		if(it != bodies.end()) // name found
			body = it->second;
		}
	return body;
	}

// -----------------------------------------------------------------------------

bool BulletPhysics::createBody(const char* bodyName, int shapeType, btScalar xSizeOrRadius, btScalar ySize, btScalar zSize, btScalar mass)
	{
	if(bodyName == NULL)
		return false;
	//
	RigidBody* newBody = new RigidBody(this->dynamicsWorld, this->primitiveShape(shapeType, xSizeOrRadius, ySize, zSize), mass);
	//newBody->addShape();
	//
	if(this->bodyByName(bodyName) == NULL)
		{
		// pushes the new object in the map
		this->bodies.insert(std::pair<std::string, RigidBody*>(bodyName, newBody));
		return true;
		}
	else
		delete newBody;
	//
	return false;
	}

// -----------------------------------------------------------------------------


