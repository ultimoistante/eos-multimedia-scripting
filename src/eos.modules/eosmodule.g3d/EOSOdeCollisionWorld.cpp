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


#include "EOSOdeCollisionWorld.h"

// -----------------------------------------------------------------------------

EOSOdeCollisionWorld::EOSOdeCollisionWorld()
	{
	this->odeWorld = dWorldCreate();
	dWorldSetERP(this->odeWorld, 0.8);
	dWorldSetCFM(this->odeWorld, 1e-5);
	//dWorldSetGravity(this->odeWorld, 0, 0, 0); // default: no gravity
	//dWorldSetCFM(this->odeWorld, 10e-5);
	dWorldSetAutoDisableFlag(this->odeWorld, 1);
	//contactGroupId = dJointGroupCreate(0);
	this->defaultSpace = dHashSpaceCreate(0);
	//defaultSpaceId = dSimpleSpaceCreate(0);
	dSpaceSetCleanup(this->defaultSpace, 0); // disables auto destruction of contained geometries on space destruction
	//
	this->trimeshVertices    = NULL;
	this->trimeshIndices     = NULL;
	this->trimeshVertexCount = 0;
	this->trimeshIndexCount  = 0;
	}

// -----------------------------------------------------------------------------

EOSOdeCollisionWorld::~EOSOdeCollisionWorld()
	{
	//# closes ODE
	dCloseODE();
	//
	if(this->trimeshVertices)
		delete[] this->trimeshVertices;
	if(this->trimeshIndices)
		delete[] this->trimeshIndices;
	}

// -----------------------------------------------------------------------------

void EOSOdeCollisionWorld::collisionCallback(void *data, dGeomID g1, dGeomID g2)
	{
	static char msgBuffer[2048];
	//printf("!!! - EOSOdeWorld::collisionCallback() - >>> \n");
	// Get a pointer back to the frame listener that called us
	EOSOdeCollisionWorld* odeCollisionWorld = (EOSOdeCollisionWorld*)data;

	dContact contact[MAX_CONTACTS_PER_STEP];

	for(int i=0; i<MAX_CONTACTS_PER_STEP; i++)
		{
		contact[i].surface.mode = dContactSoftERP;
		contact[i].surface.soft_erp = 0.002;
		}

	// Get all the contacts
	//dContactGeom contact[MAX_CONTACTS_PER_STEP];
	int n = dCollide(g1, g2, MAX_CONTACTS_PER_STEP, &contact[0].geom, sizeof(dContactGeom));

	// If we had some contacts...
	for(int i=0; i<n; i++)
		{
		EOSOgreObject* ogreObject = (EOSOgreObject*)dGeomGetData(contact[i].geom.g1);
		const char* geometryOneName = (ogreObject != NULL) ? ogreObject->objectName : "";
		ogreObject = (EOSOgreObject*)dGeomGetData(contact[i].geom.g2);
		const char* geometryTwoName = (ogreObject != NULL) ? ogreObject->objectName : "";
		// enqueue message
		snprintf(msgBuffer, 2046, "EOSKernel.messageInit(\"EOSOgre\") EOSKernel.messagePushData(\"messageType\", \"3d_collision\") EOSKernel.messagePushData(\"objectOne\", \"%s\") EOSKernel.messagePushData(\"objectTwo\", \"%s\") EOSKernel.messagePushData(\"contactPosX\", %f) EOSKernel.messagePushData(\"contactPosY\", %f) EOSKernel.messagePushData(\"contactPosZ\", %f) EOSKernel.messagePushData(\"contactNormalX\", %f) EOSKernel.messagePushData(\"contactNormalY\", %f) EOSKernel.messagePushData(\"contactNormalZ\", %f) EOSKernel.messagePushData(\"contactDepth\", %f) EOSKernel.messageSend(\"\")", geometryOneName, geometryTwoName, contact[i].geom.pos[0], contact[i].geom.pos[1], contact[i].geom.pos[2], contact[i].geom.normal[0], contact[i].geom.normal[1], contact[i].geom.normal[2], contact[i].geom.depth);
		if(strchr(msgBuffer, '#') == NULL)
			EOSLuaRuntime::getInstance()->executeLuaChunk(msgBuffer);
		}

	}

// -----------------------------------------------------------------------------

void EOSOdeCollisionWorld::checkCollisions()
	{
	//dSpaceCollide(this->defaultSpace, NULL, collisionCallback);
	//printf(".\n");
	dSpaceCollide(this->defaultSpace, (void*)this, &EOSOdeCollisionWorld::collisionCallback);
	}

// -----------------------------------------------------------------------------

/*
void collisionCallback(void *data, dGeomID geom_a, dGeomID geom_b)
	{
	// default ode nearCallback:
	int i, j, n;
	const int N = 100;
	dContactGeom contact[N];

	if(dGeomGetClass (geom_b) == dRayClass)
		{
		n = dCollide (geom_b, geom_a, N, &contact[0], sizeof(dContactGeom));
		}
	else
		{
		n = dCollide (geom_a, geom_b, N, &contact[0], sizeof(dContactGeom));
		}
	printf("geom_a/geom_b: %d | %d\n", geom_a, geom_b);
	n = dCollide (geom_a, geom_b, N, &contact[0], sizeof(dContactGeom));
	if (n > 0)
		{
		printf("Contact!!!");
		dMatrix3 RI;
		dRSetIdentity (RI);
		const dReal ss[3] = {0.01,0.01,0.01};
		for (i=0; i<n; i++)
			{
			contact[i].pos[2] += Z_OFFSET;
			dsDrawBox (contact[i].pos,RI,ss);
			dVector3 n;
			for (j=0; j<3; j++) n[j] = contact[i].pos[j] + 0.1*contact[i].normal[j];
			dsDrawLine (contact[i].pos,n);
			}
		}

	/* from ogreode:
	bool a_space = (dGeomIsSpace(geom_a))?true:false;
	bool b_space = (dGeomIsSpace(geom_b))?true:false;

	void* ptr_a = dGeomGetData(geom_a);
	void* ptr_b = dGeomGetData(geom_b);

	if(a_space  || b_space )
		{
		// Collide a space with a space
		if(a_space && b_space)
			static_cast<Space*>(ptr_a)->collide(static_cast<Space*>(ptr_b),data);
		else if(a_space)
			static_cast<Space*>(ptr_a)->collide(static_cast<Geometry*>(ptr_b),data);
		else
			static_cast<Space*>(ptr_b)->collide(static_cast<Geometry*>(ptr_a),data);

		// Collide geoms internal to the spaces
		if(a_space) static_cast<Space*>(ptr_a)->collide(data);
		if(b_space) static_cast<Space*>(ptr_b)->collide(data);
		}
    else
		{
		// Collide a geom with a geom, i.e. generate contacts
		static_cast<Geometry*>(ptr_a)->collide(static_cast<Geometry*>(ptr_b),_collision_listener);
		}
	}*/

// -----------------------------------------------------------------------------

EOSOdeCollisionObject* EOSOdeCollisionWorld::createBoxGeometry(float xsize, float ysize, float zsize)
	{
	EOSOdeCollisionObject* newCollisionObject = new EOSOdeCollisionObject();
	newCollisionObject->collisionGeomType = EOSOdeCollisionObject::BoxGeomType;
	newCollisionObject->collisionGeometry = dCreateBox(this->defaultSpace, xsize, ysize, zsize);
	newCollisionObject->size = Ogre::Vector3(xsize, ysize, zsize);
	return newCollisionObject;
	}

// -----------------------------------------------------------------------------

EOSOdeCollisionObject* EOSOdeCollisionWorld::createSphereGeometry(float radius)
	{
	EOSOdeCollisionObject* newCollisionObject = new EOSOdeCollisionObject();
	newCollisionObject->collisionGeomType = EOSOdeCollisionObject::SphereGeomType;
	newCollisionObject->collisionGeometry = dCreateSphere(this->defaultSpace, radius);
	newCollisionObject->size = Ogre::Vector3(radius, radius, radius);
	return newCollisionObject;
	}

// -----------------------------------------------------------------------------

EOSOdeCollisionObject* EOSOdeCollisionWorld::createRayGeometry(float length)
	{
	EOSOdeCollisionObject* newCollisionObject = new EOSOdeCollisionObject();
	newCollisionObject->collisionGeomType = EOSOdeCollisionObject::RayGeomType;
	newCollisionObject->collisionGeometry = dCreateRay(this->defaultSpace, length);
	newCollisionObject->size = Ogre::Vector3(0, 0, length);
	return newCollisionObject;
	}

// -----------------------------------------------------------------------------

EOSOdeCollisionObject* EOSOdeCollisionWorld::createSphereGeometryFromEntity(EOSOgreObject* ogreObject)
	{
	EOSOdeCollisionObject* newCollisionObject = NULL;
	if(ogreObject != NULL && ogreObject->getType() == EOSOgreObject::EntityObjectType)
		{
		printf(" *** ogreObject->update() OK...\n");
		// gets AABB of the Ogre object
		Ogre::Vector3 aabbMax = ogreObject->getEntity()->getBoundingBox().getMaximum();
		Ogre::Vector3 nodeScale = ogreObject->cachedSceneNode->sceneNode->getScale();
		aabbMax *= nodeScale;
		float radius = aabbMax.x;
		if(aabbMax.y > aabbMax.x) radius = aabbMax.y;
		if(aabbMax.z > aabbMax.y) radius = aabbMax.z;
		//radius /= 2.0;
		//radius *= 10.0;
		printf(" *** radius calculation OK (%f)...\n", radius);
		//
		newCollisionObject = createSphereGeometry(radius);
		printf(" *** collisionObject OK (%d)...\n", newCollisionObject->collisionGeometry);
		//
		}
	//
	return newCollisionObject;
	}

// -----------------------------------------------------------------------------

EOSOdeCollisionObject* EOSOdeCollisionWorld::createTrimeshGeometryFromEntity(Ogre::MeshPtr meshPtr)
	{
	printf("* createTrimeshGeometryFromEntity - >>>\n");
	//
	EOSOdeCollisionObject* newCollisionObject = new EOSOdeCollisionObject();
	newCollisionObject->collisionGeomType = EOSOdeCollisionObject::TrimeshGeomType;
	printf("  * newCollisionObject OK...\n");
	//newCollisionObject->collisionGeometry = dCreateSphere(this->defaultSpace, radius);
	//
	// extracts trimesh data from Ogre Mesh
	extractMeshData(meshPtr); //, Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY, Ogre::Vector3::UNIT_SCALE);
	printf("  * extractMeshData OK...\n");
	//
	// and loads it into the new collision object created
	newCollisionObject->vertexCount = this->trimeshVertexCount;
	newCollisionObject->indexCount = this->trimeshIndexCount;
	newCollisionObject->vertices = new dVector3[this->trimeshVertexCount];
	newCollisionObject->indices = new unsigned int[this->trimeshIndexCount];
	for(unsigned int i = 0; i < this->trimeshVertexCount; i++)
		{
		newCollisionObject->vertices[i][0] = (dReal)trimeshVertices[i].x;
		newCollisionObject->vertices[i][1] = (dReal)trimeshVertices[i].y;
		newCollisionObject->vertices[i][2] = (dReal)trimeshVertices[i].z;
		}
	memcpy(newCollisionObject->indices, this->trimeshIndices, sizeof(unsigned int) * this->trimeshIndexCount);
	printf("  * memcpy OK...\n");

	newCollisionObject->triMeshData = dGeomTriMeshDataCreate();
	//dGeomTriMeshDataBuildSimple(newCollisionObject->triMeshData, (const dReal*)newCollisionObject->vertices, (int)newCollisionObject->vertexCount, (int*)newCollisionObject->indices, (int)newCollisionObject->indexCount);
	dGeomTriMeshDataBuildSimple(newCollisionObject->triMeshData, (const dReal*)newCollisionObject->vertices, (int)newCollisionObject->vertexCount, (const dTriIndex*)newCollisionObject->indices, (int)newCollisionObject->indexCount);
	newCollisionObject->collisionGeometry = dCreateTriMesh(this->defaultSpace, newCollisionObject->triMeshData, 0, 0, 0);
	printf("  * dCreateTriMesh OK...\n");
	//
	// cleanup...
	if(this->trimeshVertices)
		delete[] this->trimeshVertices;
	if(this->trimeshIndices)
		delete[] this->trimeshIndices;
	this->trimeshVertices = NULL;
	this->trimeshIndices = NULL;
	this->trimeshVertexCount = 0;
	this->trimeshIndexCount = 0;
	//
	//
	//newCollisionObject->size = Ogre::Vector3(radius, radius, radius);
	return newCollisionObject;
	}

// -----------------------------------------------------------------------------

void EOSOdeCollisionWorld::extractMeshData(Ogre::MeshPtr mesh) //, const Ogre::Vector3& position, const Ogre::Quaternion& orient, const Ogre::Vector3& scale)
	{
	printf("  * extractMeshData - >>>\n");
	bool added_shared     = false;
	size_t current_offset = this->trimeshVertexCount,
	shared_offset         = this->trimeshVertexCount,
	next_offset           = this->trimeshVertexCount,
	index_offset          = this->trimeshIndexCount;
	size_t prev_vert      = this->trimeshVertexCount,
	prev_ind              = this->trimeshIndexCount;

	bool got_size = false;
	bool got_radius = false;

	// Calculate how many vertices and indices we're going to need
	for(int i = 0; i < mesh->getNumSubMeshes(); ++i)
		{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		// We only need to add the shared vertices once
		if(submesh->useSharedVertices)
			{
			if(!added_shared)
				{
				Ogre::VertexData* vertex_data = mesh->sharedVertexData;
				this->trimeshVertexCount += vertex_data->vertexCount;
				added_shared = true;
				}
			}
		else
			{
			Ogre::VertexData* vertex_data = submesh->vertexData;
			this->trimeshVertexCount += vertex_data->vertexCount;
			}

		// Add the indices
		Ogre::IndexData* index_data = submesh->indexData;
		this->trimeshIndexCount += index_data->indexCount;
		}
	printf("    * SubMeshes ok\n");

	// (re)allocate space for the vertices and indices
	Ogre::Vector3* tmp_vert = new Ogre::Vector3[this->trimeshVertexCount];
	if(prev_vert)
		{
		memcpy(tmp_vert, this->trimeshVertices, sizeof(Ogre::Vector3) * prev_vert);
		delete[] this->trimeshVertices;
		}
	this->trimeshVertices = tmp_vert;
	printf("    * allocate space for the vertices and indices ok\n");

	unsigned int* tmp_ind = new unsigned int[this->trimeshIndexCount];
	if(prev_ind)
		{
		memcpy(tmp_ind, this->trimeshIndices, sizeof(unsigned int) * prev_ind);
		delete[] this->trimeshIndices;
		}
	this->trimeshIndices = tmp_ind;

	added_shared = false;

	// Run through the submeshes again, adding the data into the arrays
	for(int i = 0; i < mesh->getNumSubMeshes(); ++i)
		{
		Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
		if((!submesh->useSharedVertices) || (submesh->useSharedVertices && !added_shared))
			{
			if(submesh->useSharedVertices)
				{
				added_shared = true;
				shared_offset = current_offset;
				}

			const Ogre::VertexElement* posElem = vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
			Ogre::HardwareVertexBufferSharedPtr vbuf = vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());
			unsigned char* vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			Ogre::Real* pReal;

			for(size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
				{
				posElem->baseVertexPointerToElement(vertex, &pReal);

				Ogre::Vector3 pt;

				pt.x = (*pReal++);
				pt.y = (*pReal++);
				pt.z = (*pReal++);

				// applies rotation, traslation and scale
				//pt = (orient * (pt * scale)) + position;

				this->trimeshVertices[current_offset + j].x = pt.x;
				this->trimeshVertices[current_offset + j].y = pt.y;
				this->trimeshVertices[current_offset + j].z = pt.z;
				}
			vbuf->unlock();
			next_offset += vertex_data->vertexCount;
			}

		Ogre::IndexData* index_data = submesh->indexData;

		size_t numTris = index_data->indexCount / 3;
		unsigned short* pShort;
		unsigned int* pInt;
		Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;
		bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);
		if (use32bitindexes) pInt = static_cast<unsigned int*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		else pShort = static_cast<unsigned short*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

		for(size_t k = 0; k < numTris; ++k)
			{
			size_t offset = (submesh->useSharedVertices)?shared_offset:current_offset;

			unsigned int vindex = use32bitindexes? *pInt++ : *pShort++;
			this->trimeshIndices[index_offset + 0] = vindex + offset;
			vindex = use32bitindexes? *pInt++ : *pShort++;
			this->trimeshIndices[index_offset + 1] = vindex + offset;
			vindex = use32bitindexes? *pInt++ : *pShort++;
			this->trimeshIndices[index_offset + 2] = vindex + offset;

			index_offset += 3;
			}
		ibuf->unlock();
		current_offset = next_offset;
		}
	}

// -----------------------------------------------------------------------------
