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


#include "EOSOgreObject.h"

// -----------------------------------------------------------------------------

EOSOgreObject::EOSOgreObject(const char* objectName, Ogre::SceneManager* sceneMgr)
	{
	this->objectName      = strdup(objectName);
	this->sceneManager    = sceneMgr;
	this->cachedSceneNode = NULL;
	this->entity          = NULL;
	this->particleSystem  = NULL;
	this->billboardSet    = NULL;
	this->collisionObject = NULL;
	this->ribbonTrail     = NULL;
	this->objectType      = EOSOgreObject::VoidObjectType;
	//this->currentAnimationState = NULL;
	//this->currentAnimationSpeed = 1.0;
	//
	//this->notifyOnCollisions = false;
	}

// -----------------------------------------------------------------------------

EOSOgreObject::~EOSOgreObject()
	{
	/*if(this->sceneNode != NULL)
		this->sceneNode->detachAllObjects();
	if(this->entity != NULL)
		sceneManager->removeEntity(entity);
		//delete this->ogreEntity;
	if(this->sceneNode != NULL)
		{
		this->sceneNode->removeAndDestroyAllChildren();
		if(objectName != NULL)
			this->sceneManager->destroySceneNode(this->sceneNode->getName().c_str());
		}
	if(particleSystem != NULL)
		Ogre::ParticleSystemManager::getSingleton().destroySystem(particleSystem);*/

	if(objectName != NULL)
		delete objectName;

	//if(this->collisionObject != NULL)
	//	delete this->collisionObject;

	}

// -----------------------------------------------------------------------------

void EOSOgreObject::destroy()
	{
	//printf("EOSOgreObject::destroy() - >>>\n");

	if(this->cachedSceneNode != NULL)
		{
		this->cachedSceneNode->sceneNode->detachAllObjects();
		//printf("\tdetachAllObjects ok...\n");
		//printf("attached nodes - before: %d\n", this->cachedSceneNode->sceneNode->numChildren() );
		this->cachedSceneNode->sceneNode->removeAllChildren();
		//printf("\tremoveAllChildren ok...\n");
		//printf("attached nodes - after : %d\n", this->cachedSceneNode->sceneNode->numChildren() );
		// don't destroys the sceneNode, but sets it as "not used"
		this->cachedSceneNode->inUse = false;

		if(this->cachedSceneNode->sceneNode->getParent() != NULL)
			this->cachedSceneNode->sceneNode->getParent()->removeChild(this->cachedSceneNode->sceneNode);
		//else
		//	printf("\tORPHANED CHILD!!!\n");

		//printf("\tdetach from parent ok...\n");
		this->sceneManager->getRootSceneNode()->addChild(this->cachedSceneNode->sceneNode);
		//printf("\treattach to root ok...\n");
		//this->sceneManager->destroySceneNode(this->cachedSceneNode->sceneNode->getName().c_str());
		}

  	// object destroy
  	//
	if(this->entity != NULL)
		this->sceneManager->destroyEntity(entity);

	if(this->particleSystem != NULL)
		this->sceneManager->destroyParticleSystem(particleSystem);

	if(this->billboardSet != NULL)
		this->sceneManager->destroyBillboardSet(this->billboardSet);

	if(this->collisionObject != NULL)
		{
		delete this->collisionObject;
		this->collisionObject = NULL;
		}

	if(this->ribbonTrail != NULL)
		this->sceneManager->destroyRibbonTrail(this->ribbonTrail);

	//printf("EOSOgreObject::destroy() - <<<\n");

	delete this;
	}

// -----------------------------------------------------------------------------

/*void EOSOgreObject::requestSceneNode(const char* requestedNodeName)
	{
	if(this->cachedSceneNode == NULL)
		{
		this->cachedSceneNode = EOSOgre3D::getInstance()->requestCachedSceneNode(requestedNodeName);
		//if(requestedNodeName != NULL)
		//	this->sceneNode = (this->sceneManager->getRootSceneNode()->createChildSceneNode(requestedNodeName));
		//else
		//	this->sceneNode = (this->sceneManager->getRootSceneNode()->createChildSceneNode());
		}
	}*/

// -----------------------------------------------------------------------------

void EOSOgreObject::attachEntity(Ogre::Entity* entityPtr)
	{
	if(entityPtr != NULL)
		{
		this->entity     = entityPtr;
		this->objectType = EOSOgreObject::EntityObjectType;
		}
	}

// -----------------------------------------------------------------------------

void EOSOgreObject::attachParticleSystem(Ogre::ParticleSystem* particleSystemPtr)
	{
	if(particleSystemPtr != NULL)
		{
		this->particleSystem = particleSystemPtr;
		this->objectType     = EOSOgreObject::ParticleSystemObjectType;
		}
	}

// -----------------------------------------------------------------------------

void EOSOgreObject::attachBillboardSet(Ogre::BillboardSet* bbSetPtr)
	{
	if(bbSetPtr != NULL)
		{
		this->billboardSet = bbSetPtr;
		this->objectType   = EOSOgreObject::BillboardSetObjectType;
		}
	}

// -----------------------------------------------------------------------------

void EOSOgreObject::attachCollisionObject(EOSOdeCollisionObject* collisionObject)
	{
	if(this->collisionObject != NULL)
		delete this->collisionObject;
	//
	if(collisionObject != NULL)
		{
		if(this->objectType == EOSOgreObject::VoidObjectType)
			this->objectType = EOSOgreObject::CollisionObjectType;
		this->collisionObject = collisionObject;
		collisionObject->setParentObject(this);
		this->update(0);
		}
	}

// -----------------------------------------------------------------------------

void EOSOgreObject::attachRibbonTrail(Ogre::RibbonTrail* ribbonTrailPtr)
	{
	if(ribbonTrailPtr != NULL)
		{
		this->ribbonTrail = ribbonTrailPtr;
		this->objectType  = EOSOgreObject::RibbonTrailObjectType;
		}
	}

// -----------------------------------------------------------------------------

EntityAnimation* EOSOgreObject::animByName(const char* animationName)
	{
	EntityAnimation* anim = NULL;
	if(animationName != NULL)
		{
		std::map<std::string, EntityAnimation*>::iterator it;
		it = entityAnimations.find(animationName);
		if(it != entityAnimations.end()) // name found
			anim = (*it).second;
		}
	return anim;
	}

// -----------------------------------------------------------------------------

EntityAnimation* EOSOgreObject::addEntityAnim(const char* animName, Ogre::AnimationState* animState)
	{
	EntityAnimation* entityAnimation = NULL;
	if(animState != NULL)
		{
		entityAnimation = new EntityAnimation(animState);
		entityAnimation->parentObjectName = this->objectName;
		entityAnimation->animationName = animName;
		entityAnimations.insert(std::pair<std::string, EntityAnimation*>(animName, entityAnimation));
		}
	return entityAnimation;
	}

// -----------------------------------------------------------------------------

void EOSOgreObject::setVisible(bool status)
	{
	switch(this->objectType)
		{
		case EOSOgreObject::EntityObjectType:
			this->entity->setVisible(status);
			break;
		case EOSOgreObject::ParticleSystemObjectType:
			this->particleSystem->setVisible(status);
			break;
		case EOSOgreObject::BillboardSetObjectType:
			this->billboardSet->setVisible(status);
			break;
		case EOSOgreObject::CollisionObjectType:
			this->collisionObject->showDebugObject(this->cachedSceneNode->sceneNode, status);
			break;
		case EOSOgreObject::RibbonTrailObjectType:
			this->ribbonTrail->setVisible(status);
			break;
		}
	}

// -----------------------------------------------------------------------------

void EOSOgreObject::update(float frameTime)
	{
	if(this->cachedSceneNode->sceneNode != NULL)
		{
		bool mustUpdate = false;
		if(this->cachedSceneNode->sceneNode->_getDerivedPosition() != this->currentPosition)
			{
			this->currentPosition = this->cachedSceneNode->sceneNode->_getDerivedPosition();
			mustUpdate = true;
			}
		if(this->cachedSceneNode->sceneNode->_getDerivedOrientation() != this->currentOrientation)
			{
			this->currentOrientation = this->cachedSceneNode->sceneNode->_getDerivedOrientation();
			mustUpdate = true;
			}
		if(this->cachedSceneNode->sceneNode->getScale() != this->currentScale)
			{
			this->currentScale = this->cachedSceneNode->sceneNode->getScale();
			mustUpdate = true;
			}
		//
		if(mustUpdate && this->collisionObject != NULL)
			this->collisionObject->applyTransform(this->currentPosition , this->currentOrientation, this->currentScale);
		//
		// updates all active skeletal animations
		std::map<std::string, EntityAnimation*>::iterator it;
		for(it = entityAnimations.begin(); it != entityAnimations.end(); it++)
			{
			EntityAnimation* anim = it->second;
			if(anim != NULL)
				anim->update(frameTime);
			}
		}
	}

// -----------------------------------------------------------------------------
