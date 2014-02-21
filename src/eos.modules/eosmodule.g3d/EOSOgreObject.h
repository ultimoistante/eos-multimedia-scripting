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

#ifndef EOS_GFX_EOSOGREOBJECT_H
#define EOS_GFX_EOSOGREOBJECT_H

#include "Ogre.h"
#include "EOSOdeCollisionObject.h"
#include "EOSLuaRuntime.h"
#include "CachedSceneNode.h"

class EOSOgre3D;


class EntityAnimation
	{
	public:
		EntityAnimation(Ogre::AnimationState* animation)
			{
			this->animation = animation;
			this->speed     = 1.0;
			this->enabled   = false;
			this->loop      = false;
			this->frozen    = true;
			this->direction = 1;
			}

		void update(float frameTime)
			{
			if(enabled && !frozen)
				{
				float timeToAdd = (frameTime * speed * direction);
				//
				// enqueue message on animation end
				if((animation->getTimePosition() + timeToAdd) > animation->getLength())
					{
					//printf("!!! - Anim Alert Message - !!!\n");
					snprintf(msgBuffer, 1024, "EOSKernel.messageInit(\"EOSOgre\") EOSKernel.messagePushData(\"messageType\", \"animationAlert\") EOSKernel.messagePushData(\"objectName\", \"%s\") EOSKernel.messagePushData(\"animationName\", \"%s\") EOSKernel.messagePushData(\"alertType\", \"animation_end\") EOSKernel.messageSend(\"\")", parentObjectName.c_str(), animationName.c_str());
					EOSLuaRuntime::getInstance()->executeLuaChunk(msgBuffer);
					}
				//
				animation->addTime( timeToAdd );
				}
			}

		void enable()         { animation->setEnabled(true); animation->setLoop(this->loop); this->frozen = false; this->enabled = animation->getEnabled(); }
		void disable()        { animation->setEnabled(false); this->frozen = true; this->enabled = animation->getEnabled(); }
		//void restart()        { animation->setEnabled(false); animation->setTimePosition(1.0); animation->setEnabled(true); animation->setLoop(this->loop); this->enabled = animation->getEnabled(); }
		std::string           parentObjectName;
		std::string           animationName;
		Ogre::AnimationState* animation;
		Ogre::Real            speed;
		Ogre::Real            direction;
		bool                  enabled;
		bool                  loop;
		bool                  frozen;
		//
		char msgBuffer[1024];
	//
	};


class EOSOgreObject
	{
	public:
		EOSOgreObject(const char* objectName, Ogre::SceneManager* sceneMgr);
		~EOSOgreObject();
		//
		enum ObjectType
			{
			VoidObjectType,
			EntityObjectType,
			ParticleSystemObjectType,
			BillboardSetObjectType,
			CollisionObjectType,
			RibbonTrailObjectType
			};
		//
		void destroy();
		//void requestSceneNode(const char* requestedNodeName);
		//
		void attachEntity(Ogre::Entity* entityPtr);
		void attachParticleSystem(Ogre::ParticleSystem* particleSystemPtr);
		void attachBillboardSet(Ogre::BillboardSet* bbSetPtr);
		void attachCollisionObject(EOSOdeCollisionObject* collisionObject);
		void attachRibbonTrail(Ogre::RibbonTrail* ribbonTrailPtr);
		//
		EOSOgreObject::ObjectType getType() { return this->objectType; }
		//
		EntityAnimation* animByName(const char* animationName);
		EntityAnimation* addEntityAnim(const char* animName, Ogre::AnimationState* animState);
		//
		Ogre::Entity*           getEntity()          { return this->entity; }
		Ogre::ParticleSystem*   getParticleSystem()  { return this->particleSystem; }
		Ogre::BillboardSet*     getBillboardSet()    { return this->billboardSet; }
		EOSOdeCollisionObject*  getCollisionObject() { return this->collisionObject; }
		Ogre::RibbonTrail*      getRibbonTrail()     { return this->ribbonTrail; }
		//
		void setVisible(bool status);
		//
		void update(float frameTime);
		//
		Ogre::SceneManager*       sceneManager;
		//Ogre::SceneNode*          sceneNode;
		CachedSceneNode*          cachedSceneNode;
		//
		Ogre::AnimationState*     currentAnimationState;
		Ogre::Real                currentAnimationSpeed;
		//
		Ogre::Vector3             currentPosition;
		Ogre::Quaternion          currentOrientation;
		Ogre::Vector3             currentScale;
		//
		const char*               objectName;
		//
	private:
		//
		Ogre::Entity*             entity;
		Ogre::ParticleSystem*     particleSystem;
		Ogre::BillboardSet*       billboardSet;
		Ogre::RibbonTrail*        ribbonTrail;
		//
		EOSOdeCollisionObject*    collisionObject;
		//dGeomID                   collisionGeometry;
		//
		EOSOgreObject::ObjectType objectType;
		//
		std::map<std::string, EntityAnimation*> entityAnimations;
		//
	};



#endif
