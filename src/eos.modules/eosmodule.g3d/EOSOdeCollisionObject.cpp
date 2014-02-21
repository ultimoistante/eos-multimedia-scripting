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


#include "EOSOdeCollisionObject.h"

// -----------------------------------------------------------------------------

EOSOdeCollisionObject::EOSOdeCollisionObject()
	{
	this->collisionGeometry = 0;
	this->debugObject       = NULL;
	this->size = Ogre::Vector3::ZERO;
	//
	this->vertices = NULL;
	this->verticesBackup = NULL;
	this->indices = NULL;
	this->vertexCount = 0;
	this->indexCount = 0;
	}

// -----------------------------------------------------------------------------

EOSOdeCollisionObject::~EOSOdeCollisionObject()
	{
	if(this->collisionGeometry > 0)
		dGeomDestroy(this->collisionGeometry);
	if(this->debugObject != NULL)
		delete this->debugObject;
	if(this->vertices != NULL)
		delete[] this->vertices;
	if(this->verticesBackup != NULL)
		delete[] this->verticesBackup;
	if(this->indices != NULL)
		delete this->indices;
	}

// -----------------------------------------------------------------------------

/*void EOSOdeCollisionObject::loadTrimeshData(const Ogre::Vector3* vertices, unsigned int vertex_count, const unsigned int* indices, unsigned int index_count)
	{
	this->vertexCount = vertex_count;
	this->indexCount = index_count;
	this->vertices = new dVector3[vertex_count];
	this->indices = new unsigned int[index_count];

	for(unsigned int i = 0;i < vertex_count;i++)
		{
		this->vertices[i][0] = (dReal)vertices[i].x;
		this->vertices[i][1] = (dReal)vertices[i].y;
		this->vertices[i][2] = (dReal)vertices[i].z;
		}

	memcpy(this->indices, indices, sizeof(unsigned int) * index_count);

	this->triMeshData = dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSimple(this->triMeshData, (const dReal*)this->vertices, (int)vertex_count, (int*)this->indices, (int)index_count);
	this->collisionGeometry = dCreateTriMesh(getSpaceID(space), this->triMeshData, 0, 0, 0);
	registerGeometry();
	}*/

// -----------------------------------------------------------------------------

void EOSOdeCollisionObject::applyTransform(Ogre::Vector3& position, Ogre::Quaternion& rotation, Ogre::Vector3& scale)
	{
	dQuaternion odeQuaternion;
	odeQuaternion[0] = (dReal)rotation.w; odeQuaternion[1] = (dReal)rotation.x; odeQuaternion[2] = (dReal)rotation.y; odeQuaternion[3] = (dReal)rotation.z;
	//
	dGeomSetPosition(this->collisionGeometry, position.x, position.y, position.z);
	dGeomSetQuaternion(this->collisionGeometry, odeQuaternion);
	//
	/*switch(this->collisionGeomType)
		{
		case EOSOdeCollisionObject::BoxGeomType:
		case EOSOdeCollisionObject::SphereGeomType:
			dGeomSetPosition(this->collisionGeometry, position.x, position.y, position.z);
			dGeomSetQuaternion(this->collisionGeometry, odeQuaternion);
			break;
		case EOSOdeCollisionObject::TrimeshGeomType:
			// makes a backup of the initial status of the vertices
			if(this->verticesBackup == NULL)
				{
				this->verticesBackup = new dVector3[this->vertexCount];
				memcpy(this->verticesBackup, this->vertices, sizeof(unsigned int) * this->vertexCount);
				}
			//
			for(unsigned int i = 0; i < this->vertexCount; i++)
				{
				Ogre::Vector3 pt;
				pt.x = this->verticesBackup[i][0];
				pt.y = this->verticesBackup[i][1];
				pt.z = this->verticesBackup[i][2];
				// applies rotation, traslation and scale
				pt = (rotation * (pt * scale)) + position;
				// stores back the vertex
				this->vertices[i][0] = (dReal)pt.x;
				this->vertices[i][1] = (dReal)pt.y;
				this->vertices[i][2] = (dReal)pt.z;
				}
			dGeomTriMeshDataBuildSimple(this->triMeshData, (const dReal*)this->vertices, (int)this->vertexCount, (int*)this->indices, (int)this->indexCount);
			if(this->collisionGeometry)
				dGeomDestroy(this->collisionGeometry);
			this->collisionGeometry = dCreateTriMesh(0, this->triMeshData, 0, 0, 0);
			break;
		}*/
	}

// -----------------------------------------------------------------------------

void EOSOdeCollisionObject::setParentObject(void* parentObject)
	{
	if(parentObject != NULL && this->collisionGeometry != 0)
		dGeomSetData(this->collisionGeometry, parentObject);
	}

// -----------------------------------------------------------------------------

void EOSOdeCollisionObject::setActive(bool activityStatus)
	{
	if(this->collisionGeometry != 0)
		{
		activityStatus ? dGeomEnable(this->collisionGeometry) : dGeomDisable(this->collisionGeometry);
		}
	}

// -----------------------------------------------------------------------------

bool EOSOdeCollisionObject::isActive()
	{
	if(this->collisionGeometry != 0)
		{
		return dGeomIsEnabled(this->collisionGeometry);
		}
	return false;
	}

// -----------------------------------------------------------------------------

void EOSOdeCollisionObject::showDebugObject(Ogre::SceneNode* parentSceneNode, bool visible)
	{
	EOSOdeTrimeshDebugObject* debugObj = NULL;
	switch(this->collisionGeomType)
		{
		case EOSOdeCollisionObject::BoxGeomType:
			this->debugObject = new EOSOdeBoxDebugObject(this->size);
			break;
		case EOSOdeCollisionObject::SphereGeomType:
			this->debugObject = new EOSOdeSphereDebugObject(this->size.x);
			break;
		case EOSOdeCollisionObject::TrimeshGeomType:
			debugObj = new EOSOdeTrimeshDebugObject((this->indexCount / 3) * 6);
			debugObj->beginDefinition();
			for(unsigned int i = 0, j = 0; i < this->indexCount; i+=3, j+=6)
				{
				debugObj->setVertex(j,   Ogre::Vector3((Ogre::Real)this->vertices[this->indices[i]][0],  (Ogre::Real)this->vertices[this->indices[i]][1],  (Ogre::Real)this->vertices[this->indices[i]][2]));
				debugObj->setVertex(j+1, Ogre::Vector3((Ogre::Real)this->vertices[this->indices[i+1]][0],(Ogre::Real)this->vertices[this->indices[i+1]][1],(Ogre::Real)this->vertices[this->indices[i+1]][2]));

				debugObj->setVertex(j+2, Ogre::Vector3((Ogre::Real)this->vertices[this->indices[i+1]][0],(Ogre::Real)this->vertices[this->indices[i+1]][1],(Ogre::Real)this->vertices[this->indices[i+1]][2]));
				debugObj->setVertex(j+3, Ogre::Vector3((Ogre::Real)this->vertices[this->indices[i+2]][0],(Ogre::Real)this->vertices[this->indices[i+2]][1],(Ogre::Real)this->vertices[this->indices[i+2]][2]));

				debugObj->setVertex(j+4, Ogre::Vector3((Ogre::Real)this->vertices[this->indices[i+2]][0],(Ogre::Real)this->vertices[this->indices[i+2]][1],(Ogre::Real)this->vertices[this->indices[i+2]][2]));
				debugObj->setVertex(j+5, Ogre::Vector3((Ogre::Real)this->vertices[this->indices[i]][0],  (Ogre::Real)this->vertices[this->indices[i]][1],  (Ogre::Real)this->vertices[this->indices[i]][2]));
				}
			debugObj->endDefinition();
			this->debugObject = debugObj;
			break;
		case EOSOdeCollisionObject::RayGeomType:
			dVector3 start, direction;
			dGeomRayGet(this->collisionGeometry, start, direction);
			Ogre::Real length = dGeomRayGetLength(this->collisionGeometry);
			this->debugObject = new EOSOdeRayDebugObject(Ogre::Vector3(start[0], start[1], start[2]), Ogre::Vector3(direction[0], direction[1], direction[2]), length);
			break;
		}
	parentSceneNode->attachObject(this->debugObject);
	}

// -----------------------------------------------------------------------------
