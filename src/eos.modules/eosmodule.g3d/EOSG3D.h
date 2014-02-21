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

#ifndef EOS_MODULE_OGRE3D_EOSOGRE3D_H
#define EOS_MODULE_OGRE3D_EOSOGRE3D_H

#include <GL/glx.h>
#include <Ogre.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreBlendMode.h>
#include "EOSOgreObject.h"
#include "EOSOgreSimpleSpline.h"
#include "EOSOgreRotationalSpline.h"
#include "EOSLuaRuntime.h"
#include "EOSOdeCollisionWorld.h"
#include "CachedSceneNode.h"
#include "EOSProfiler.h"
#include "EOSObject.h"
#include "eos_data_containers.h"

#define _LBX_SOURCE1               111
#define _LBX_SOURCE2               112
#define _LBX_MODULATE              113
#define _LBX_MODULATE_X2           114
#define _LBX_MODULATE_X4           115
#define _LBX_ADD                   116
#define _LBX_ADD_SIGNED            117
#define _LBX_ADD_SMOOTH            118
#define _LBX_SUBTRACT              119
#define _LBX_BLEND_DIFFUSE_ALPHA   120
#define _LBX_BLEND_TEXTURE_ALPHA   121
#define _LBX_BLEND_CURRENT_ALPHA   122
#define _LBX_BLEND_MANUAL          123
#define _LBX_DOTPRODUCT            124
#define _LBX_BLEND_DIFFUSE_COLOUR  125

#define _LBS_CURRENT               211
#define _LBS_TEXTURE               212
#define _LBS_DIFFUSE               213
#define _LBS_SPECULAR              214
#define _LBS_MANUAL                215


typedef struct ogre_gl_render_target
    {
    Ogre::TexturePtr ogreTexturePtr;
    eos_imagedata    image;
    EOSObject        eosObject;
    } OgreGLRenderTarget;


typedef struct controlledSceneNode_t
    {
    Ogre::SceneNode* sceneNode;
    eos_3dobject*    externalObject;
    } ControlledSceneNode;


class EOSG3D
    {
    public:
        static EOSG3D* getInstance();
        ~EOSG3D() { instanceFlag = false; }
        //
        bool powerOn(long windowHandle);
        void update(Ogre::Real frameTime);
        void render();
        bool shutdown();
        //
        int setupResources();
        bool clearScene();
        CachedSceneNode* requestCachedSceneNode(const char* nodeName);
        void createDefaultCamera(bool createcamera);
        //
        OgreGLRenderTarget* renderTargetByName(const char* targetName);
        EOSObject* createRenderSurface(const char* targetName, int width, int height);
        //
        //
        EOSOgreObject*       createEmptyObject(const char* objectName);
        bool                 removeObject(const char* objectName);
        //
        Ogre::SceneNode*     objectLoad(const char* objectName, const char* meshFileName, CachedSceneNode* cachedSceneNode);
        Ogre::SceneNode*     particleSystemLoad(const char* objectName, const char* particleMaterialName, CachedSceneNode* cachedSceneNode);
        Ogre::SceneNode*     billboardSetCreate(const char* bbSetName, CachedSceneNode* cachedSceneNode);
        //
        Ogre::Pass*          getMaterialTechniquePass(const char* materialName, unsigned short techniqueIndex, unsigned short passIndex, const char* requestingFunctionName);
        Ogre::TextureUnitState* getMaterialTechniquePassTextureUnit(const char* materialName, unsigned short techniqueIndex, unsigned short passIndex, unsigned short textureUnitIndex, const char* requestingFunctionName);
        //
        bool                 createRenderTexture(const char* textureName, unsigned int textureWidth, unsigned int textureHeight);
        bool                 bindRenderTextureToMaterial(const char* textureName, const char* materialName, unsigned int materialTechnique, unsigned int materialPass, unsigned int materialTextureUnitState);
        //
        EOSOgreObject*       objectByName(const char* objectName, const char* requestingFunctionName);
        Ogre::SceneNode*     nodeByName(const char* nodeName, const char* requestingFunctionName);
        //
        bool                 setObjectCastsShadow(const char* objectName, bool castsShadow);
        //
        bool                 overlayCreate(const char* overlayName);
        bool                 overlaySetVisible(const char* overlayName, bool visible);
        bool                 overlaySetZOrder(const char* overlayName, unsigned short zOrder);
        bool                 overlayAdd2D(const char* overlayName, const char* overlayElementName);
        bool                 overlayAdd3D(const char* overlayName, const char* sceneNodeName);
        bool                 overlayContainerCreate(const char* containerName);
        bool                 overlayContainerAddChild(const char* containerName, const char* childElementName);
        bool                 overlayTextAreaElementCreate(const char* textAreaName);
        bool                 overlayTextAreaSetFontName(const char* textAreaName, const char* fontName);
        bool                 overlayTextAreaSetFontSize(const char* textAreaName, float fontSize);
        bool                 overlayTextAreaSetCaption(const char* textAreaName, const char* caption);
        const char*          overlayTextAreaGetCaption(const char* textAreaName);
        bool                 overlayTextAreaSetColor(const char* textAreaName, float r, float g, float b, float a);
        bool                 overlayTextAreaSetColorTop(const char* textAreaName, float r, float g, float b, float a);
        bool                 overlayTextAreaSetColorBottom(const char* textAreaName, float r, float g, float b, float a);
        Ogre::ColourValue    overlayTextAreaGetColor(const char* textAreaName);
        Ogre::ColourValue    overlayTextAreaGetColorTop(const char* textAreaName);
        Ogre::ColourValue    overlayTextAreaGetColorBottom(const char* textAreaName);
        bool                 overlayElementSetVisible(const char* elementName, bool visible);
        bool                 overlayElementSetDimensions(const char* containerName, float width, float height);
        bool                 overlayElementSetPosition(const char* containerName, float x, float y);
        bool                 overlayElementSetMaterial(const char* containerName, const char* materialName);
        bool                 overlayElementSetMetricsMode(const char* containerName, const char* metricsMode);
        //
        bool                 billboardSetSetMaterial(const char* bbSetName, const char* materialName);
        bool                 billboardSetSetDefaultDimensions(const char* bbSetName, float width, float height);
        bool                 billboardSetSetCastShadows(const char* bbSetName, bool shadowsActive);
        bool                 billboardCreate(const char* bbSetName, float x, float y, float z, float r, float g, float b);
        float                billboardGetRotation(const char* bbSetName, unsigned int bbIndex);
        bool                 billboardSetRotation(const char* bbSetName, unsigned int bbIndex, float rotationAngle);
        Ogre::Vector3        billboardGetPosition(const char* bbSetName, unsigned int bbIndex);
        bool                 billboardSetPosition(const char* bbSetName, unsigned int bbIndex, float x, float y, float z);
        bool                 billboardSetDimensions(const char* bbSetName, unsigned int bbIndex, float width, float height);
        bool                 billboardResetDimensions(const char* bbSetName, unsigned int bbIndex);
        bool                 billboardSetColor(const char* bbSetName, unsigned int bbIndex, float r, float g, float b);
        Ogre::ColourValue    billboardGetColor(const char* bbSetName, unsigned int bbIndex);
        Ogre::Vector3        billboardGetDimensions(const char* bbSetName, unsigned int bbIndex); /* note: in returned vector, only x and y values are used */
        //
        bool                 createSimpleSpline(const char* simpleSplineName, unsigned int maxPoints = 0);
        EOSOgreSimpleSpline* simpleSplineByName(const char* simpleSplineName);
        bool                     createRotationalSpline(const char* rotationalSplineName, unsigned int maxPoints = 0);
        EOSOgreRotationalSpline* rotationalSplineByName(const char* rotationalSplineName);
        //
        void                 unloadUnreferencedTextures(bool reloadableOnly);
        void                 unloadUnreferencedMeshes(bool reloadableOnly);
        //
        //Ogre::Vector3        getPositionFromLuaTable(const char* luaTableName);
        bool                 bindNodeToEOSObject(const char* sceneNodeName, EOSObject* externalObject);
        //
        //
        Ogre::SceneManager*    getSceneManager() { return this->sceneManager; }
        Ogre::Camera*          getDefaultCamera() { return this->defaultCamera; }
        Ogre::SceneNode*       getDefaultCameraNode() { return this->defaultCameraNode; }
        Ogre::Viewport*        getDefaultViewport() { return this->defaultViewport; }
        EOSOdeCollisionWorld*  getCollisionWorld() { return this->collisionWorld; }
        //
        bool                 isInitialized() { return initialized; }

        std::vector<Ogre::AnimationState*> animStates;

    private:
        EOSG3D();
        Ogre::TextAreaOverlayElement* overlayTextAreaElementByName(const char* textAreaName);
        //
        static bool instanceFlag;
        static EOSG3D* instance;
        //
        bool initialized;
        //
        Ogre::Root*              gfxRoot;
        Ogre::RenderWindow*      renderWindow;
        //Ogre::RenderTexture*     defaultRenderTexture;
        //Ogre::TexturePtr         texture;
        Ogre::Camera*            defaultCamera;
        Ogre::Viewport*          defaultViewport;
        Ogre::SceneNode*         defaultCameraNode;
        Ogre::SceneManager*      sceneManager;
        Ogre::OverlayManager*    overlayManager;
        EOSOdeCollisionWorld*    collisionWorld;
        //
        // ogre objects (meshes) hashmap
        std::map<std::string, EOSOgreObject*> objects;
        // render targets hashmap
        std::map<std::string, OgreGLRenderTarget*> renderTargets;
        // render textures hashmap
        std::map<std::string, Ogre::RenderTexture*> renderTextures;
        // simpleSpline hashmap
        std::map<std::string, EOSOgreSimpleSpline*> simpleSplines;
        // rotationalSpline hashmap
        std::map<std::string, EOSOgreRotationalSpline*> rotationalSplines;
        //
        std::map<std::string, std::string> properties;
        //
        std::vector<CachedSceneNode*> sceneNodeCache;
        //
        std::vector<ControlledSceneNode*> controlledSceneNodes;
        //
        std::string applicationRoot;
        //
        EOSProfiler *prof;
        //
    };



#endif

