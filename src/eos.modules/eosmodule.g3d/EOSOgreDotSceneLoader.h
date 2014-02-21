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

#ifndef EOS_GFX_DOTSCENELOADER_H
#define EOS_GFX_DOTSCENELOADER_H

#include <Ogre.h>
#include "tinyxml2.h"
#include "EOSLuaRuntime.h"
#include "EOSOgreObject.h"
#include "EOSG3D.h"

using namespace tinyxml2;

class EOSOgreDotSceneLoader
	{
	public:

	EOSOgreDotSceneLoader(EOSG3D* ogreWrapperInstance)
		{
		this->ogreWrapper = ogreWrapperInstance;
		}

	~EOSOgreDotSceneLoader()
		{
		}
	//
	void loadDotScene(const char* sceneFileName);
	//
	//
	private:
		void parseEnvironmentElement(XMLElement* envElement);
		void parseFogElement(XMLElement* fogElement);
		void parseSkyBoxElement(XMLElement* skyboxElement);
		void parseSkyDomeElement(XMLElement* skydomeElement);
		void parseSkyPlaneElement(XMLElement* skyplaneElement);
		void parseSceneCameraElement(XMLElement* cameraElement);
		void parseNodeElement(XMLElement* nodeElement);
		void parseEntityElement(XMLElement* entityElement, CachedSceneNode* cachedSceneNode);
		void parseLightElement(XMLElement* lightElement, CachedSceneNode* cachedSceneNode);
		void parseCameraElement(XMLElement* cameraElement, CachedSceneNode* cachedSceneNode);
		void parseParticleSystemElement(XMLElement* particleSystemElement, CachedSceneNode* cachedSceneNode);
		void parseBillboardSetElement(XMLElement* billboardSetElement, CachedSceneNode* cachedSceneNode);
		//
		//bool               getAttribute(XMLElement* element, const char* attributeName, std::string* recipientString);
		Ogre::Vector3& parseVector3(XMLElement* vector3Element);
		Ogre::Quaternion& parseQuaternion(XMLElement* quaternionElement);
		Ogre::ColourValue& parseColourValue(XMLElement* colourValueElement);
		void parseClipping(XMLElement* clippingElement, Ogre::Real &nearClip, Ogre::Real &farClip);
		//
		Ogre::Vector3 tempVector3;
		Ogre::Quaternion tempQuaternion;
		Ogre::ColourValue tempColourValue;
		EOSG3D* ogreWrapper;
	};



#endif // EOS_GFX_DOTSCENELOADER_H
