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

#ifndef EOSOGRE_EOSODECOLLISIONOBJECT_H
#define EOSOGRE_EOSODECOLLISIONOBJECT_H


#include "Ogre.h"
#include "ode/ode.h"
#include "ode/collision_trimesh.h"
#include "EOSOdeDebugObjects.h"


class EOSOdeCollisionObject
	{
	public:
		EOSOdeCollisionObject();
		~EOSOdeCollisionObject();
		//
		enum OdeGeomType
			{
			NullGeomType,
			BoxGeomType,
			SphereGeomType,
			RayGeomType,
			TrimeshGeomType
			};
		//
		//void loadTrimeshData(const Ogre::Vector3* vertices, unsigned int vertex_count, const unsigned int* indices, unsigned int index_count);
		//
		void showDebugObject(Ogre::SceneNode* parentSceneNode, bool visible);
		//
		void applyTransform(Ogre::Vector3& position, Ogre::Quaternion& rotation, Ogre::Vector3& scale);
		//
		void setParentObject(void* parentObject);
		//
		void setActive(bool activityStatus);
		bool isActive();
		//
		dGeomID             collisionGeometry;
		EOSOdeDebugObject*  debugObject;
		//
		Ogre::Vector3 size;
		//
		EOSOdeCollisionObject::OdeGeomType collisionGeomType;
	//private:
		dTriMeshDataID  triMeshData;
		dVector3*       vertices;
		dVector3*       verticesBackup;
		unsigned int*   indices;
		unsigned int    vertexCount;
		unsigned int    indexCount;
		//
	};



#endif
