/*
-----------------------------------------------------------------------------
This source file is part of EOS Multimedia Scripting System
For the latest info, see https://github.com/ultimoistante/eos-multimedia-scripting

The MIT License (MIT)

Copyright (c) 2014-2019 of Salvatore Carotenuto aka ultimoistante of StartupSolutions
                           (mailto: ultimoistante@gmail.com | www.startupsolutions.it)

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

#ifndef EOSOGRE_EOSODECOLLISIONWORLD_H
#define EOSOGRE_EOSODECOLLISIONWORLD_H

#include "ode/ode.h"
#include "EOSOgreObject.h"
#include "EOSOdeCollisionObject.h"
#include "EOSLuaRuntime.h"

#define MAX_CONTACTS_PER_STEP	100

class EOSOdeCollisionWorld
	{
	public:
		EOSOdeCollisionWorld();
		~EOSOdeCollisionWorld();
		//CollisionObject* createSphereCollisionObject(EOSOgreObject* ogreObject);
		//void collisionCallback(void *data, dGeomID geom_a, dGeomID geom_b);
		//
		void checkCollisions();
		//
		EOSOdeCollisionObject* createBoxGeometry(float xsize, float ysize, float zsize);
		EOSOdeCollisionObject* createSphereGeometry(float radius);
		EOSOdeCollisionObject* createRayGeometry(float length);
		EOSOdeCollisionObject* createSphereGeometryFromEntity(EOSOgreObject* ogreObject);
		EOSOdeCollisionObject* createTrimeshGeometryFromEntity(Ogre::MeshPtr meshPtr);
		//
	private:
		//
		void extractMeshData(Ogre::MeshPtr mesh);
		//
		static void collisionCallback(void *data, dGeomID g1, dGeomID g2);
		//
		dWorldID		   odeWorld;
		dJointGroupID	contacts;
		dSpaceID		   defaultSpace;
		//
		Ogre::Vector3*	trimeshVertices;
		unsigned int*  trimeshIndices;
		size_t         trimeshVertexCount;
		size_t         trimeshIndexCount;
		//
	};


#endif
