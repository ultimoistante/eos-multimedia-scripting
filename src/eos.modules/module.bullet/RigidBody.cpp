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
 * File:   RigidBody.cpp
 * Author: ultimoistante
 * 
 * Created on September 26, 2009, 5:33 PM
 */

#include "RigidBody.h"

// -----------------------------------------------------------------------------

RigidBody::RigidBody(btDiscreteDynamicsWorld* dynamicsWorld, btCollisionShape* collisionShape, btScalar mass)
	{
	this->sessionObject = NULL;
	//
	this->dynamicsWorld = dynamicsWorld;
	this->shape = collisionShape;
	this->mass = mass;
	//
	//this->bodyTransform.setIdentity();
	//this->bodyTransform.setOrigin(btVector3(0, 0, 0));
	//this->motionState = new btDefaultMotionState(this->bodyTransform);

	// initial body placement - orientation: identity, position: 0, 0, 0
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(0, 0, 0));
	this->motionState = new btDefaultMotionState(trans);
	//
	this->shape->calculateLocalInertia(this->mass, this->localInertia);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(this->mass, this->motionState, this->shape, this->localInertia);
	this->body = new btRigidBody(rbInfo);
	this->dynamicsWorld->addRigidBody(this->body);
	//
	}

// -----------------------------------------------------------------------------

RigidBody::~RigidBody()
	{
	if(this->motionState != NULL)
		delete this->motionState;
	//
	if(this->body != NULL)
		{
		this->dynamicsWorld->removeCollisionObject(this->body);
		delete this->body;
		}
	//
	if(this->shape != NULL)
		delete this->shape;
	//
	if(this->sessionObject != NULL)
		delete this->sessionObject;
	}

// -----------------------------------------------------------------------------

void RigidBody::updateSessionObjectData()
	{
	if(this->body && this->body->getMotionState())
		{
		body->getMotionState()->getWorldTransform(this->bodyTransform);
		this->sessionObjectData.position.x = (float)this->bodyTransform.getOrigin().x();
		this->sessionObjectData.position.y = (float)this->bodyTransform.getOrigin().y();
		this->sessionObjectData.position.z = (float)this->bodyTransform.getOrigin().z();
		//
		this->sessionObjectData.orientation.x = (float)this->bodyTransform.getRotation().x();
		this->sessionObjectData.orientation.y = (float)this->bodyTransform.getRotation().y();
		this->sessionObjectData.orientation.z = (float)this->bodyTransform.getRotation().z();
		this->sessionObjectData.orientation.w = (float)this->bodyTransform.getRotation().w();
		// todo: update size in session object
		}
	}

// -----------------------------------------------------------------------------

EOSObject* RigidBody::getSessionObject()
	{
	if(this->sessionObject == NULL) // first time we request the session object data
		{
		this->sessionObject = new EOSObject();
		this->sessionObject->setType("3D_OBJECT");
		this->sessionObject->setUserData((void*)&(this->sessionObjectData));
		this->sessionObject->setUserDataSize(sizeof(eos_3dobject));
		}
	return this->sessionObject;
	}

// -----------------------------------------------------------------------------

void RigidBody::setMass(btScalar mass)
	{
	this->mass = mass;
	if(/*this->isDynamic() &&*/ this->shape != NULL)
		{
		this->shape->calculateLocalInertia(this->mass, this->localInertia);
		// updates body
		if(this->body != NULL)
			{
			this->dynamicsWorld->removeCollisionObject(this->body);
			delete this->body;
			}
		btRigidBody::btRigidBodyConstructionInfo rbInfo(this->mass, this->motionState, this->shape, this->localInertia);
		this->body = new btRigidBody(rbInfo);
		printf("body recreated\n");
		}
	}

// -----------------------------------------------------------------------------

btVector3* RigidBody::getPosition()
	{
	btVector3* pos = NULL;
	if(this->body && this->body->getMotionState())
		{
		body->getMotionState()->getWorldTransform(this->bodyTransform);
		pos = &(this->bodyTransform.getOrigin());
		}
	return pos;
	}

// -----------------------------------------------------------------------------

void RigidBody::setPosition(btScalar x, btScalar y, btScalar z)
	{
	if(this->body && this->body->getMotionState())
		{
		body->getMotionState()->getWorldTransform(this->bodyTransform);
		this->bodyTransform.setOrigin(btVector3(x, y, z));
		body->getMotionState()->setWorldTransform(this->bodyTransform);
		body->setCenterOfMassTransform(this->bodyTransform);
		body->activate();
		}
	}

// -----------------------------------------------------------------------------

btQuaternion* RigidBody::getOrientation()
	{
	btQuaternion* orientation = NULL;
	if(this->body && this->body->getMotionState())
		{
		body->getMotionState()->getWorldTransform(this->bodyTransform);
		orientation = &(this->bodyTransform.getRotation());
		}
	return orientation;
	}

// -----------------------------------------------------------------------------

void RigidBody::setOrientation(btScalar x, btScalar y, btScalar z, btScalar w)
	{
	if(this->body && this->body->getMotionState())
		{
		body->getMotionState()->getWorldTransform(this->bodyTransform);
		this->bodyTransform.setRotation(btQuaternion(x, y, z, w));
		body->getMotionState()->setWorldTransform(this->bodyTransform);
		body->setCenterOfMassTransform(this->bodyTransform);
		body->activate();
		}
	}

// -----------------------------------------------------------------------------
