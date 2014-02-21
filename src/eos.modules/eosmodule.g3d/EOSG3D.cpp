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

#include "EOSG3D.h"

// -----------------------------------------------------------------------------

bool EOSG3D::instanceFlag = false;
EOSG3D* EOSG3D::instance = NULL;

// -----------------------------------------------------------------------------

EOSG3D* EOSG3D::getInstance()
    {
    if (!instanceFlag)
        {
        instance = new EOSG3D();
        instanceFlag = true;
        }
    return instance;
    }

// -----------------------------------------------------------------------------

EOSG3D::EOSG3D()
    {
    this->initialized = false;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::powerOn(long windowHandle)
    {
    bool success = false;
    //
    if (!initialized)
        {
        this->applicationRoot = EOSLuaRuntime::getInstance()->getLuaValueAsString("eos.application.path");
        //
        // # gfxRoot object creation
        std::string pluginsCfgFilename = applicationRoot + "/settings/OgrePlugins.cfg";
        std::string ogreCfgFilename = applicationRoot + "/settings/Ogre.cfg";
        std::string ogreLogFilename = applicationRoot + "logs/Ogre.log";
        printf("*** ogreLogFilename: %s\n", ogreLogFilename.c_str());
        this->gfxRoot = new Ogre::Root(pluginsCfgFilename.c_str(), ogreCfgFilename.c_str(), ogreLogFilename.c_str());
        if (gfxRoot == NULL)
            {
            EOSLuaRuntime::getInstance()->logErrorMessage("[eos.g3d] FATAL! Can't create Ogre::Root object!");
            return false;
            }
        Ogre::LogManager::getSingleton().createLog(ogreLogFilename.c_str(), true, false, false);
        //
        // # resources setup
        setupResources();
        //
        if (windowHandle != 0) // ogre inherits window from other application
            {
            EOSLuaRuntime::getInstance()->logUserMessage("[eos.g3d] *** initialization on window with id: %d", windowHandle);

            Ogre::RenderSystemList pList = gfxRoot->getAvailableRenderers();
            Ogre::RenderSystem* pRenderSystem = NULL;
            bool found = false;
            for (Ogre::RenderSystemList::iterator it = pList.begin(); it != pList.end() && !found; ++it)
                {
                Ogre::RenderSystem* pSystem = *it;
                if (pSystem != NULL)
                    {
                    EOSLuaRuntime::getInstance()->logUserMessage("[eos.g3d] *** Host window RenderSystem: %s", pSystem->getName().c_str());

                    if (pSystem->getName() == "OpenGL Rendering Subsystem")
                        {
                        // favorize OpenGL
                        pRenderSystem = pSystem;
                        found = true;
                        break;
                        }
                    else if (pRenderSystem->getName() != "OpenGL Rendering Subsystem" && pSystem->getName() == "Direct3D9 Rendering SubSystem")
                        {
                        // if no OpenGL avail, then take D3D9
                        pRenderSystem = pSystem;
                        found = true;
                        break;
                        }
                    else if (pRenderSystem->getName() != "OpenGL Rendering Subsystem" && pRenderSystem->getName() != "Direct3D9 Rendering SubSystem" && pSystem->getName() == "Direct3D7 Rendering SubSystem")
                        {
                        // well if this system sucks so hard, then take D3D7
                        pRenderSystem = pSystem;
                        found = true;
                        }
                    }
                }

            // We need one!
            if (pRenderSystem == NULL)
                {
                EOSLuaRuntime::getInstance()->logErrorMessage("[eos.g3d] FATAL! No supported render system available! Aborting...");
                return false;
                }
            EOSLuaRuntime::getInstance()->logUserMessage("[eos.g3d] *** Render System found...");

            // tell Ogre about the render system
            gfxRoot->setRenderSystem(pRenderSystem);
            EOSLuaRuntime::getInstance()->logUserMessage("[eos.g3d] *** Set Render System ok...");

            // fire up the root node
            gfxRoot->initialise(false);
            EOSLuaRuntime::getInstance()->logUserMessage("[eos.g3d] *** Initialization ok...");

            // Compose RT properties
            Ogre::NameValuePairList Properties;
            if (windowHandle != 0)
                {
                // Properties.insert(Ogre::NameValuePairList::value_type("parentWindowHandle", Ogre::StringConverter::toString((long)windowHandle)));
                Properties.insert(Ogre::NameValuePairList::value_type("currentGLContext", Ogre::StringConverter::toString(true)));
                Properties.insert(Ogre::NameValuePairList::value_type("externalGLControl", Ogre::StringConverter::toString(true)));
                }

            // create render window            
            this->renderWindow = gfxRoot->createRenderWindow("OGRE Render Window", 0, 0, false, &Properties);
            this->renderWindow->setActive(true);
            EOSLuaRuntime::getInstance()->logUserMessage("[eos.g3d] *** createRenderWindow ok...");
            }
        //
        //
        // # mipmap level setting (NB some APIs ignore this)
        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

        // # scene initialization
        // setting SceneManager type
        this->sceneManager = gfxRoot->createSceneManager(Ogre::ST_GENERIC, "defaultSceneManager");
        /*std::string sceneManagerType = EOSLuaRuntime::getInstance()->getLuaValueAsString("EOSOgre._settings.sceneManagerType");
                  if(sceneManagerType == "GENERIC")
                          {
                          //this->sceneManager = gfxRoot->getSceneManager(Ogre::ST_GENERIC);
                          this->sceneManager = gfxRoot->createSceneManager(Ogre::ST_GENERIC, "defaultSceneManager");
                          EOSLuaRuntime::getInstance()->logInfoMessage("[eos.g3d] Using GENERIC scene manager");
                          }
                  else if(sceneManagerType == "EXTERIOR_CLOSE")
                          {
                          this->sceneManager = gfxRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE, "defaultSceneManager");
                          EOSLuaRuntime::getInstance()->logInfoMessage("[eos.g3d] Using EXTERIOR_CLOSE scene manager");
                          }
                  else if(sceneManagerType == "EXTERIOR_FAR")
                          {
                          this->sceneManager = gfxRoot->createSceneManager(Ogre::ST_EXTERIOR_FAR, "defaultSceneManager");
                          EOSLuaRuntime::getInstance()->logInfoMessage("[eos.g3d] Using EXTERIOR_FAR scene manager");
                          }
                  else if(sceneManagerType == "EXTERIOR_REAL_FAR")
                          {
                          this->sceneManager = gfxRoot->createSceneManager(Ogre::ST_EXTERIOR_REAL_FAR, "defaultSceneManager");
                          EOSLuaRuntime::getInstance()->logInfoMessage("[eos.g3d] Using EXTERIOR_REAL_FAR scene manager");
                          }
                  else if(sceneManagerType == "INTERIOR")
                          {
                          this->sceneManager = gfxRoot->createSceneManager(Ogre::ST_INTERIOR, "defaultSceneManager");
                          EOSLuaRuntime::getInstance()->logInfoMessage("[eos.g3d] Using INTERIOR scene manager");
                          }
                  else // unrecognized string passed
                          {
                          this->sceneManager = gfxRoot->createSceneManager(Ogre::ST_GENERIC, "defaultSceneManager");
                          EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d] Warning: unrecognized scene manager type passed. Falling back on default (GENERIC) scene manager.");
                          }*/
        //
        //
        if (sceneManager != NULL)
            {
            this->createDefaultCamera(true);
            //
            // sets shadow techinque
            sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
            /*std::string shadowMode = EOSLuaRuntime::getInstance()->getLuaValueAsString("EOSOgre._settings.shadowsMode");
            if(shadowMode == "TEXTURE_MODULATIVE")
                    {
                    sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
                    EOSLuaRuntime::getInstance()->logInfoMessage("[eos.g3d] Using TEXTURE_MODULATIVE shadows mode");
                    }
            else if(shadowMode == "STENCIL_MODULATIVE")
                    {
                    sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
                    EOSLuaRuntime::getInstance()->logInfoMessage("[eos.g3d] Using STENCIL_MODULATIVE shadows mode");
                    }
            else if(shadowMode == "STENCIL_ADDITIVE")
                    {
                    sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
                    EOSLuaRuntime::getInstance()->logInfoMessage("[eos.g3d] Using STENCIL_ADDITIVE shadows mode");
                    }
            else
                    EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d] Warning: no shadows selected. Check value of 'ShadowsMode' in settings file");*/
            }
        //
        //
        /* the code below has been moved in the setRenderTarget function
                  // Create one viewport, entire window
                  this->defaultViewport = renderWindow->addViewport(defaultCamera);
                  this->defaultViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
                  //
                  // Alter the camera aspect ratio to match the viewport
                  defaultCamera->setAspectRatio(Ogre::Real(this->defaultViewport->getActualWidth()) / Ogre::Real(this->defaultViewport->getActualHeight()));
         */
        //
        // Initialise, parse scripts etc
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
        //
        //
        // Initializes EOSOdeCollisionWorld instance
        this->collisionWorld = new EOSOdeCollisionWorld();
        //
        //
        this->overlayManager = Ogre::OverlayManager::getSingletonPtr();
        //
        //
        this->prof = new EOSProfiler("g3d");
        //
        this->initialized = true;
        }
    }

// -----------------------------------------------------------------------------

void EOSG3D::update(Ogre::Real frameTime)
    {
    prof->beginProfiling("ogre.update");
    float frameTimeSecs = frameTime / 1000.0f;
    // # ogre graphics update
    //
    //  - updates the animation state and physic position/orientation of all objects
    std::map<std::string, EOSOgreObject*>::iterator it;
    for (it = objects.begin(); it != objects.end(); it++)
        {
        EOSOgreObject* object = it->second;
        if (object != NULL)
            object->update(frameTimeSecs);
        }
    //
    // updates all controlled-by-external-objects scene nodes
    unsigned int size = this->controlledSceneNodes.size();
    for (int i = 0; i < size; i++)
        {
        ControlledSceneNode* controlledSceneNode = this->controlledSceneNodes[i];
        if (controlledSceneNode != NULL)
            {
            eos_3dobject* externalObject = controlledSceneNode->externalObject;
            controlledSceneNode->sceneNode->setPosition((Ogre::Real)externalObject->position.x, -(Ogre::Real)externalObject->position.y, (Ogre::Real)externalObject->position.z);
            controlledSceneNode->sceneNode->setOrientation((Ogre::Real)externalObject->orientation.w, (Ogre::Real)externalObject->orientation.x, (Ogre::Real)externalObject->orientation.y, (Ogre::Real)externalObject->orientation.z);
            }
        }

    // updates animation states (eos.g3d 'paths' and 'pathTracks')
    int animStatesTotal = animStates.size();
    for (int i = 0; i < animStatesTotal; i++)
        animStates[i]->addTime(frameTimeSecs);
    //
    //this->collisionWorld->checkCollisions();
    //
    prof->endProfiling("ogre.update");
    }

// -----------------------------------------------------------------------------

void EOSG3D::render()
    {
    prof->beginProfiling("renderTextureUpdate");
    //
    std::map<std::string, OgreGLRenderTarget*>::iterator it;
    for (it = renderTargets.begin(); it != renderTargets.end(); it++)
        {
        OgreGLRenderTarget* target = it->second;
        target->ogreTexturePtr->getBuffer()->getRenderTarget()->update(false);
        }
    //
    prof->endProfiling("renderTextureUpdate");
    }

// -----------------------------------------------------------------------------

bool EOSG3D::shutdown()
    {
    if (!initialized)
        return false;
    //
    if (prof)
        delete prof;
    //
    LOGINFO("[eos.g3d] Shutting down...");
    //
    // deletes all loaded objects
    std::map<std::string, EOSOgreObject*>::iterator it;
    for (it = objects.begin(); it != objects.end(); it++)
        delete it->second;
    objects.clear();
    LOGINFO("[eos.g3d] All objects deleted.");
    //
    // deletes all loaded RenderTexture
    std::map<std::string, Ogre::RenderTexture*>::iterator it2;
    for (it2 = renderTextures.begin(); it2 != renderTextures.end(); it2++)
        delete it2->second;
    renderTextures.clear();
    LOGINFO("[eos.g3d] All render textures deleted.");
    //
    // deletes all loaded SimpleSpline
    std::map<std::string, EOSOgreSimpleSpline*>::iterator it3;
    for (it3 = simpleSplines.begin(); it3 != simpleSplines.end(); it3++)
        delete it3->second;
    simpleSplines.clear();
    LOGINFO("[eos.g3d] All SimpleSplines deleted.");
    //
    sceneManager->clearScene();
    LOGINFO("[eos.g3d] Scene cleared.");
    //
    if (gfxRoot)
        delete gfxRoot;
    LOGINFO("[eos.g3d] Ogre::Root deleted.");
    //
    LOGINFO("[eos.g3d] shutdown complete.");
    return true;
    }

// -----------------------------------------------------------------------------

int EOSG3D::setupResources()
    {
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    std::string configFile = applicationRoot + "/settings/OgreResources.cfg";
    cf.load(configFile.c_str());

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
        {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
            {
            typeName = i->first;
            archName = i->second;
            //ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(applicationRoot + "/" + archName, typeName, secName);
            }
        }

    return 0;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::clearScene()
    {
    std::map<std::string, EOSOgreObject*>::iterator it;
    for (it = objects.begin(); it != objects.end(); it++)
        {
        EOSOgreObject* theObject = it->second;
        theObject->destroy();
        //delete it->second;
        }
    objects.clear();
    //
    EOSLuaRuntime::getInstance()->logInfoMessage("[eos.g3d][clearScene()] Memory amount of allocated meshes BEFORE cleaning: %d KBytes", Ogre::MeshManager::getSingleton().getMemoryUsage() / 1024);
    EOSLuaRuntime::getInstance()->logInfoMessage("[eos.g3d][clearScene()] Memory amount of allocated textures BEFORE cleaning: %d KBytes", Ogre::TextureManager::getSingleton().getMemoryUsage() / 1024);
    //
    sceneManager->clearScene();
    Ogre::MeshManager::getSingleton().unloadUnreferencedResources();
    Ogre::TextureManager::getSingleton().unloadAll();
    //
    EOSLuaRuntime::getInstance()->logInfoMessage("[eos.g3d][clearScene()] Memory amount of allocated meshes AFTER  cleaning: %d KBytes", Ogre::MeshManager::getSingleton().getMemoryUsage() / 1024);
    EOSLuaRuntime::getInstance()->logInfoMessage("[eos.g3d][clearScene()] Memory amount of allocated textures AFTER  cleaning: %d KBytes", Ogre::TextureManager::getSingleton().getMemoryUsage() / 1024);
    //
    int size = this->sceneNodeCache.size();
    for (int i = 0; i < size; i++)
        delete sceneNodeCache[i];
    sceneNodeCache.clear();
    //
    // re-create the camera node
    this->createDefaultCamera(false);
    }

// -----------------------------------------------------------------------------

void EOSG3D::createDefaultCamera(bool createcamera)
    {
    // creates default camera
    if (createcamera)
        {
        this->defaultCamera = sceneManager->createCamera("defaultCamera");
        this->defaultCamera->setNearClipDistance(5);
        }
    // added 08/06/2006:
    this->defaultCameraNode = sceneManager->getRootSceneNode()->createChildSceneNode("defaultCameraNode");
    this->defaultCameraNode->attachObject(this->defaultCamera);
    this->defaultCameraNode->resetOrientation();
    }

// -----------------------------------------------------------------------------

OgreGLRenderTarget* EOSG3D::renderTargetByName(const char* targetName)
    {
    OgreGLRenderTarget* target = NULL;
    if (targetName != NULL)
        {
        std::map<std::string, OgreGLRenderTarget*>::iterator it;
        it = renderTargets.find(targetName);
        if (it != renderTargets.end()) // name found
            target = it->second;
        }
    return target;
    }

// -----------------------------------------------------------------------------

EOSObject* EOSG3D::createRenderSurface(const char* targetName, int width, int height)
    {
    if (targetName == NULL)
        {
        EOSLuaRuntime::getInstance()->logErrorMessage("[eos.g3d][createRenderSurface] Error! Unable to create RenderTarget: invalid 'name' string.");
        return 0;
        }
    if (renderTargetByName(targetName) != NULL)
        {
        EOSLuaRuntime::getInstance()->logErrorMessage("[eos.g3d][createRenderSurface] Error! RenderTarget with the name '%s' already exists. Ignoring request.", targetName);
        return 0;
        }

    // creates a new OgreGLRenderTarget structure, to contain new RenderTexture
    OgreGLRenderTarget* newOgreGLRenderTarget = new OgreGLRenderTarget;

    // create a texture and link it with the camera
    newOgreGLRenderTarget->ogreTexturePtr = Ogre::TextureManager::getSingleton().createManual(targetName,
                                                                                              Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                                                              Ogre::TEX_TYPE_2D,
                                                                                              width,
                                                                                              height,
                                                                                              1,
                                                                                              Ogre::PF_BYTE_RGBA,
                                                                                              Ogre::TU_RENDERTARGET);

    Ogre::RenderTexture* newRenderTarget = newOgreGLRenderTarget->ogreTexturePtr->getBuffer()->getRenderTarget();
    newRenderTarget->addViewport(defaultCamera);
    newRenderTarget->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
    //
    // Alter the camera aspect ratio to match the viewport
    defaultCamera->setAspectRatio(Ogre::Real(width / height));
    // Gets hardware pixel buffer related texture id - *alex trick* :)
    newOgreGLRenderTarget->image.glTextureHandle = newOgreGLRenderTarget->ogreTexturePtr->getBuffer()->getHandle();
    newOgreGLRenderTarget->image.width = width;
    newOgreGLRenderTarget->image.height = height;
    newOgreGLRenderTarget->image.imageType = EOS_IMAGETYPE_GLTEXTURE;
    // fills attached eosObject with correct data
    newOgreGLRenderTarget->eosObject.setType("GL_TEXTURE_HANDLE");
    newOgreGLRenderTarget->eosObject.setUserData(&(newOgreGLRenderTarget->image));
    newOgreGLRenderTarget->eosObject.setUserDataSize(sizeof (newOgreGLRenderTarget->image));
    // inserts new render target in the hashmap
    renderTargets.insert(std::pair<std::string, OgreGLRenderTarget*>(targetName, newOgreGLRenderTarget));
    //
    return &(newOgreGLRenderTarget->eosObject);
    }

// -----------------------------------------------------------------------------

CachedSceneNode* EOSG3D::requestCachedSceneNode(const char* nodeName)
    {
    CachedSceneNode* cachedSceneNode = NULL;
    int size = this->sceneNodeCache.size();
    bool found = false;
    for (int i = 0; i < size && !found; i++)
        {
        if (!this->sceneNodeCache[i]->inUse)
            {
            cachedSceneNode = this->sceneNodeCache[i];
            cachedSceneNode->inUse = true;
            //
            Ogre::SceneNode* sceneNode = cachedSceneNode->sceneNode;
            sceneNode->setPosition(0, 0, 0);
            //sceneNode->setOrientation(Ogre::Quaternion::ZERO);
            sceneNode->resetOrientation();
            sceneNode->setScale(1, 1, 1);
            //sceneNode->resetToInitialState();
            //printf("RECYCLED scene node attachedObjects / childs: %d, %d\n", sceneNode->numAttachedObjects(), sceneNode->numChildren());
            /*if(sceneNode->getParent() != sceneManager->getRootSceneNode())
                    {
                    sceneNode->getParent()->removeChild(sceneNode);
                    sceneManager->getRootSceneNode()->addChild(sceneNode);
                    }*/
            //
            found = true;
            }
        }
    //
    if (cachedSceneNode == NULL)
        {
        //printf("allocating new node...\n");
        cachedSceneNode = new CachedSceneNode;
        cachedSceneNode->sceneNode = static_cast<Ogre::SceneNode*> ((nodeName != NULL) ? sceneManager->getRootSceneNode()->createChild(nodeName) : sceneManager->getRootSceneNode()->createChild());
        cachedSceneNode->inUse = true;
        this->sceneNodeCache.push_back(cachedSceneNode);
        //
        //Ogre::Quaternion q = cachedSceneNode->sceneNode->getOrientation();
        //printf("new node orientation: %f, %f, %f, %f\n", q.x, q.y, q.z, q.w);
        }
    //else
    //	printf("reusing existing node...\n");
    //
    return cachedSceneNode;
    }

// -----------------------------------------------------------------------------

EOSOgreObject* EOSG3D::createEmptyObject(const char* objectName)
    {
    EOSOgreObject* newOgreObject = NULL;
    if (objectName != NULL)
        {
        newOgreObject = new EOSOgreObject(objectName, this->sceneManager);
        // inserts newOgreObject in the hashmap
        objects.insert(std::pair<std::string, EOSOgreObject*>(objectName, newOgreObject));
        }
    return newOgreObject;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::removeObject(const char* objectName)
    {
    EOSOgreObject* theObject = NULL;
    if (objectName != NULL)
        {
        std::map<std::string, EOSOgreObject*>::iterator it;
        it = objects.find(objectName);
        if (it != objects.end()) // object found
            {
            theObject = it->second;
            objects.erase(it);
            theObject->destroy();
            return true;
            }
        }
    return false;
    }

// -----------------------------------------------------------------------------

Ogre::SceneNode* EOSG3D::objectLoad(const char* objectName, const char* meshFileName, CachedSceneNode* cachedSceneNode)
    {
    Ogre::SceneNode* newObjectSceneNode = NULL;
    if (objectName != NULL && meshFileName != NULL)
        {
        EOSOgreObject* newOgreObject = new EOSOgreObject(objectName, sceneManager);
        newOgreObject->attachEntity(sceneManager->createEntity(objectName, meshFileName));
        if (newOgreObject->getType() == EOSOgreObject::EntityObjectType)
            {
            newOgreObject->getEntity()->setCastShadows(true); // <-- default
            //newOgreObject->ogreEntity->setStatic(false); // <-- default
            //
            if (cachedSceneNode != NULL) // uses pre-existent scenenode
                newOgreObject->cachedSceneNode = cachedSceneNode;
            else
                {
                newOgreObject->cachedSceneNode = requestCachedSceneNode(NULL);
                //newOgreObject->sceneNode = static_cast<Ogre::SceneNode*>(sceneManager->getRootSceneNode()->createChild());
                //newOgreObject->sceneNode->setPosition(0, 0, 0);
                }
            //
            if (newOgreObject->cachedSceneNode != NULL)
                {
                newObjectSceneNode = newOgreObject->cachedSceneNode->sceneNode;
                newOgreObject->cachedSceneNode->sceneNode->attachObject(newOgreObject->getEntity());
                //newOgreObject->getEntity()->setNormaliseNormals(true); // method removed in ogre 1.6?
                // inserts newOgreObject in the hashmap
                objects.insert(std::pair<std::string, EOSOgreObject*>(objectName, newOgreObject));
                //
                }
            else
                {
                EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d] Warning! Problems loading Object '%s' (mesh name: '%s'): unable to create scene node.", objectName, meshFileName);
                delete newOgreObject;
                }
            }
        else
            {
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d] Warning! Problems loading Object '%s' (mesh name: '%s'): unable to create graphic entity.", objectName, meshFileName);
            delete newOgreObject;
            }
        }
    return newObjectSceneNode;
    }

// -----------------------------------------------------------------------------

Ogre::SceneNode* EOSG3D::particleSystemLoad(const char* objectName, const char* particleMaterialName, CachedSceneNode* cachedSceneNode)
    {
    Ogre::SceneNode* newObjectSceneNode = NULL;
    if (objectName != NULL && particleMaterialName != NULL)
        {
        EOSOgreObject* newOgreObject = new EOSOgreObject(objectName, sceneManager);
        //newOgreObject->attachParticleSystem(Ogre::ParticleSystemManager::getSingleton().createSystem(objectName, particleMaterialName));
        newOgreObject->attachParticleSystem(this->sceneManager->createParticleSystem(objectName, particleMaterialName));
        if (newOgreObject->getType() == EOSOgreObject::ParticleSystemObjectType)
            {
            //Ogre::SceneNode* entityNode = sceneManager->getRootSceneNode()->createChildSceneNode();
            if (cachedSceneNode != NULL) // uses pre-existent scenenode
                newOgreObject->cachedSceneNode = cachedSceneNode;
            else
                {
                newOgreObject->cachedSceneNode = requestCachedSceneNode(NULL);
                //newOgreObject->sceneNode = static_cast<Ogre::SceneNode*>(sceneManager->getRootSceneNode()->createChild());
                //newOgreObject->sceneNode->setPosition(0, 0, 0);
                }
            //
            if (newOgreObject->cachedSceneNode != NULL)
                {
                newObjectSceneNode = newOgreObject->cachedSceneNode->sceneNode;
                newOgreObject->cachedSceneNode->sceneNode->attachObject(newOgreObject->getParticleSystem());
                // inserts object in the hashmap
                objects.insert(std::pair<std::string, EOSOgreObject*>(objectName, newOgreObject));
                //
                EOSLuaRuntime::getInstance()->logInfoMessage("[eos.g3d] loaded Particle System '%s' (material name: '%s').", objectName, particleMaterialName);
                }
            else
                {
                EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d] Warning! Problems loading Particle System '%s' (material name: '%s'): invalid scene node.", objectName, particleMaterialName);
                delete newOgreObject;
                }
            }
        else
            {
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d] Warning! Problems loading Particle System '%s' (material name: '%s'): unable to create graphic entity.", objectName, particleMaterialName);
            delete newOgreObject;
            }
        }
    return newObjectSceneNode;
    }

// -----------------------------------------------------------------------------

Ogre::SceneNode* EOSG3D::billboardSetCreate(const char* bbSetName, CachedSceneNode* cachedSceneNode)
    {
    Ogre::SceneNode* newObjectSceneNode = NULL;
    if (bbSetName != NULL)
        {
        EOSOgreObject* newOgreObject = new EOSOgreObject(bbSetName, this->sceneManager);
        newOgreObject->attachBillboardSet(this->sceneManager->createBillboardSet(bbSetName));
        if (newOgreObject->getType() == EOSOgreObject::BillboardSetObjectType)
            {
            if (cachedSceneNode != NULL) // uses pre-existent scenenode
                newOgreObject->cachedSceneNode = cachedSceneNode;
            else
                {
                newOgreObject->cachedSceneNode = requestCachedSceneNode(NULL);
                //newOgreObject->sceneNode = static_cast<Ogre::SceneNode*>(sceneManager->getRootSceneNode()->createChild());
                //newOgreObject->sceneNode->setPosition(0, 0, 0);
                }
            //
            if (newOgreObject->cachedSceneNode != NULL)
                {
                newObjectSceneNode = newOgreObject->cachedSceneNode->sceneNode;
                newOgreObject->cachedSceneNode->sceneNode->attachObject(newOgreObject->getBillboardSet());
                // inserts object in the hashmap
                objects.insert(std::pair<std::string, EOSOgreObject*>(bbSetName, newOgreObject));
                //
                EOSLuaRuntime::getInstance()->logInfoMessage("[eos.g3d] loaded BillboardSet '%s'.", bbSetName);
                }
            else
                {
                EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d] Warning! Problems loading BillboardSet '%s': invalid scene node.", bbSetName);
                delete newOgreObject;
                }
            }
        else
            {
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d] Warning! Problems creating BillboardSet '%s': unable to create Ogre BillboardSet.", bbSetName);
            delete newOgreObject;
            }
        }
    return newObjectSceneNode;
    }

// -----------------------------------------------------------------------------

Ogre::Pass* EOSG3D::getMaterialTechniquePass(const char* materialName, unsigned short techniqueIndex, unsigned short passIndex, const char* requestingFunctionName)
    {
    Ogre::Pass* materialPass = NULL;
    Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(materialName);
    if (!material.isNull())
        {
        Ogre::Technique* technique = material.getPointer()->getTechnique(techniqueIndex);
        if (technique != NULL)
            {
            materialPass = technique->getPass(passIndex);
            if (materialPass == NULL)
                EOSLuaRuntime::getInstance()->logErrorMessage("[eos.g3d][%s] Error: requested Material '%s' doesn't have Pass %d.", requestingFunctionName, materialName, passIndex);
            }
        else
            EOSLuaRuntime::getInstance()->logErrorMessage("[eos.g3d][%s] Error: requested Material '%s' doesn't have Technique %d.", requestingFunctionName, materialName, techniqueIndex);
        }
    else
        EOSLuaRuntime::getInstance()->logErrorMessage("[eos.g3d][%s] Error: requested Material '%s' doesn't exists!", requestingFunctionName, materialName);
    //
    return materialPass;
    }

// -----------------------------------------------------------------------------

Ogre::TextureUnitState* EOSG3D::getMaterialTechniquePassTextureUnit(const char* materialName, unsigned short techniqueIndex, unsigned short passIndex, unsigned short textureUnitIndex, const char* requestingFunctionName)
    {
    Ogre::TextureUnitState* textureUnitState = NULL;
    Ogre::Pass* materialPass = getMaterialTechniquePass(materialName, techniqueIndex, passIndex, requestingFunctionName);
    if (materialPass != NULL)
        {
        textureUnitState = materialPass->getTextureUnitState(textureUnitIndex);
        if (textureUnitState == NULL)
            EOSLuaRuntime::getInstance()->logErrorMessage("[eos.g3d][%s] Error: requested Material '%s' doesn't have texture_unit state %d in Technique %d -> Pass %d.", requestingFunctionName, materialName, textureUnitIndex, techniqueIndex, passIndex);
        }
    //
    return textureUnitState;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::createRenderTexture(const char* textureName, unsigned int textureWidth, unsigned int textureHeight)
    {
    bool done = false;
    printf("WARNING: this method must be implemented!!\n");
    /*if(textureName != NULL && textureWidth > 0 && textureHeight > 0)
            {
            // checks if doesn't exists a texture with the given name
            std::map<std::string, Ogre::RenderTexture*>::iterator it;
            it = renderTextures.find(textureName);
            if(it == renderTextures.end()) // name not found
                    {
                    Ogre::RenderTexture* rttTex = this->gfxRoot->getRenderSystem()->createRenderTexture( textureName, textureWidth, textureHeight );
                    if(rttTex != NULL)
                            {
                            this->renderTextures.insert(std::pair<std::string, Ogre::RenderTexture*>(textureName, rttTex));
                            Ogre::Viewport* v = rttTex->addViewport( this->defaultCamera );
                            v->setOverlaysEnabled(false);
                            //
                            done = true;
                            }
                    }
            }*/
    return done;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::bindRenderTextureToMaterial(const char* textureName, const char* materialName, unsigned int materialTechnique, unsigned int materialPass, unsigned int materialTextureUnitState)
    {
    bool done = false;
    if (textureName != NULL && materialName != NULL)
        {
        // checks if doesn't exists a texture with the given name
        std::map<std::string, Ogre::RenderTexture*>::iterator it;
        it = renderTextures.find(textureName);
        if (it != renderTextures.end()) // texture found
            {
            Ogre::RenderTexture* rttTex = (*it).second;
            Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(materialName);
            material->getTechnique(materialTechnique)->getPass(materialPass)->getTextureUnitState(materialTextureUnitState)->setTextureName(textureName);
            done = true;
            }
        }
    return done;
    }

// -----------------------------------------------------------------------------

EOSOgreObject* EOSG3D::objectByName(const char* objectName, const char* requestingFunctionName)
    {
    EOSOgreObject* object = NULL;
    if (objectName != NULL)
        {
        std::map<std::string, EOSOgreObject*>::iterator it;
        it = objects.find(objectName);
        if (it != objects.end()) // name found
            object = (*it).second;
        else
            if (requestingFunctionName != NULL) EOSLuaRuntime::getInstance()->logErrorMessage("[eos.g3d][%s] Error: requested object named '%s' doesn't exists.", requestingFunctionName, objectName);
        }
    return object;
    }

// -----------------------------------------------------------------------------

Ogre::SceneNode* EOSG3D::nodeByName(const char* nodeName, const char* requestingFunctionName)
    {
    Ogre::SceneNode* theNode = NULL;
    try
        {
        theNode = sceneManager->getSceneNode(nodeName);
        }
    catch (...)
        {
        if (nodeName == NULL)
            EOSLuaRuntime::getInstance()->logErrorMessage("[eos.g3d][%s] Error: got NIL as nodeName!", requestingFunctionName);
        else
            EOSLuaRuntime::getInstance()->logErrorMessage("[eos.g3d][%s] Error: requested node named '%s' doesn't exists.", requestingFunctionName, nodeName);
        }
    return theNode;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::setObjectCastsShadow(const char* objectName, bool castsShadow)
    {
    bool done = false;
    EOSOgreObject* object = objectByName(objectName, "setObjectCastsShadow");
    if (object != NULL)
        {
        switch (object->getType())
            {
            case EOSOgreObject::EntityObjectType:
                object->getEntity()->setCastShadows(castsShadow);
                done = true;
                break;
            case EOSOgreObject::BillboardSetObjectType:
                object->getBillboardSet()->setCastShadows(castsShadow);
                done = true;
                break;
            }
        }
    //
    return done;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

bool EOSG3D::overlayCreate(const char* overlayName)
    {
    bool done = false;
    if (overlayName != NULL)
        {
        Ogre::Overlay* newOverlay = this->overlayManager->create(overlayName);
        done = (newOverlay != NULL);
        }
    return done;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlaySetVisible(const char* overlayName, bool visible)
    {
    bool done = false;
    if (overlayName != NULL)
        {
        Ogre::Overlay* theOverlay = (Ogre::Overlay*)this->overlayManager->getByName(overlayName);
        if (theOverlay != NULL)
            {
            if (visible)
                theOverlay->show();
            else
                theOverlay->hide();
            done = true;
            }
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d] WARNING: Unable to find overlay named '%s'", overlayName);
        }
    return done;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlaySetZOrder(const char* overlayName, unsigned short zOrder)
    {
    bool done = false;
    if (overlayName != NULL)
        {
        Ogre::Overlay* theOverlay = (Ogre::Overlay*)this->overlayManager->getByName(overlayName);
        if (theOverlay != NULL)
            {
            theOverlay->setZOrder(zOrder);
            done = true;
            }
        }
    return done;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayAdd2D(const char* overlayName, const char* overlayElementName)
    {
    bool done = false;
    if (overlayName != NULL)
        {
        Ogre::Overlay* theOverlay = (Ogre::Overlay*)this->overlayManager->getByName(overlayName);
        Ogre::OverlayElement* element = this->overlayManager->getOverlayElement(overlayElementName);
        if (theOverlay != NULL && element != NULL)
            {
            if (element->getTypeName() == "Panel")
                {
                theOverlay->add2D(static_cast<Ogre::OverlayContainer*> (element));
                done = true;
                }
            else
                EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][overlayAdd2D()] WARNING: Trying to attach a non-container overlay element '%s'. Skipping.", overlayName);
            }
        }
    return done;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayAdd3D(const char* overlayName, const char* sceneNodeName)
    {
    bool done = false;
    if (overlayName != NULL)
        {
        Ogre::Overlay* theOverlay = (Ogre::Overlay*)this->overlayManager->getByName(overlayName);
        Ogre::SceneNode* sceneNode = nodeByName(sceneNodeName, "overlayAdd3D");
        if (theOverlay != NULL && sceneNode != NULL)
            {
            theOverlay->add3D(sceneNode);
            done = true;
            }
        }
    return done;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayContainerCreate(const char* containerName)
    {
    if (containerName != NULL)
        {
        Ogre::OverlayContainer* container = static_cast<Ogre::OverlayContainer*> (this->overlayManager->createOverlayElement("Panel", containerName));
        return (container != NULL);
        }
    return false;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayContainerAddChild(const char* containerName, const char* childElementName)
    {
    if (containerName != NULL && childElementName != NULL)
        {
        Ogre::OverlayElement* container = this->overlayManager->getOverlayElement(containerName);
        if (container != NULL)
            {
            if (container->getTypeName() == "Panel")
                {
                Ogre::OverlayElement* element = this->overlayManager->getOverlayElement(childElementName);
                if (element != NULL)
                    {
                    (static_cast<Ogre::OverlayContainer*> (container))->addChild(element);
                    return true;
                    }
                else
                    EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][overlayContainerAddChild()] Error: element '%s' not found. Skipping.", childElementName);
                }
            else
                EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][overlayContainerAddChild()] WARNING: Trying to attach an overlay element to a non-container element '%s'. Skipping.", containerName);
            }
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][overlayContainerAddChild()] Error: container '%s' not found. Skipping.", containerName);
        }
    return false;
    }

// -----------------------------------------------------------------------------

Ogre::TextAreaOverlayElement* EOSG3D::overlayTextAreaElementByName(const char* textAreaName)
    {
    if (textAreaName != NULL)
        {
        Ogre::OverlayElement* element = this->overlayManager->getOverlayElement(textAreaName);
        if (element != NULL && element->getTypeName() == "TextArea")
            return static_cast<Ogre::TextAreaOverlayElement*> (element);
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][overlayTextAreaElementByName()] WARNING: Element '%s' not found or not of 'TextArea' type. Skipping.", textAreaName);
        }
    return NULL;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayTextAreaElementCreate(const char* textAreaName)
    {
    if (textAreaName != NULL)
        {
        Ogre::TextAreaOverlayElement* textArea = static_cast<Ogre::TextAreaOverlayElement*> (this->overlayManager->createOverlayElement("TextArea", textAreaName));
        return (textArea != NULL);
        }
    return false;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayTextAreaSetFontName(const char* textAreaName, const char* fontName)
    {
    Ogre::TextAreaOverlayElement* textArea = overlayTextAreaElementByName(textAreaName);
    if (textArea != NULL)
        {
        textArea->setFontName(fontName);
        return true;
        }
    return false;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayTextAreaSetFontSize(const char* textAreaName, float fontSize)
    {
    Ogre::TextAreaOverlayElement* textArea = overlayTextAreaElementByName(textAreaName);
    if (textArea != NULL)
        {
        textArea->setCharHeight(fontSize);
        return true;
        }
    return false;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayTextAreaSetCaption(const char* textAreaName, const char* caption)
    {
    Ogre::TextAreaOverlayElement* textArea = overlayTextAreaElementByName(textAreaName);
    if (textArea != NULL)
        {
        textArea->setCaption(caption);
        return true;
        }
    return false;
    }

// -----------------------------------------------------------------------------

const char* EOSG3D::overlayTextAreaGetCaption(const char* textAreaName)
    {
    Ogre::String caption = "";
    Ogre::TextAreaOverlayElement* textArea = overlayTextAreaElementByName(textAreaName);
    if (textArea != NULL)
        caption = textArea->getCaption();
    return caption.c_str();
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayTextAreaSetColor(const char* textAreaName, float r, float g, float b, float a)
    {
    Ogre::TextAreaOverlayElement* textArea = overlayTextAreaElementByName(textAreaName);
    if (textArea != NULL)
        {
        textArea->setColour(Ogre::ColourValue(r, g, b, a));
        return true;
        }
    return false;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayTextAreaSetColorTop(const char* textAreaName, float r, float g, float b, float a)
    {
    Ogre::TextAreaOverlayElement* textArea = overlayTextAreaElementByName(textAreaName);
    if (textArea != NULL)
        {
        textArea->setColourTop(Ogre::ColourValue(r, g, b, a));
        return true;
        }
    return false;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayTextAreaSetColorBottom(const char* textAreaName, float r, float g, float b, float a)
    {
    Ogre::TextAreaOverlayElement* textArea = overlayTextAreaElementByName(textAreaName);
    if (textArea != NULL)
        {
        textArea->setColourBottom(Ogre::ColourValue(r, g, b, a));
        return true;
        }
    return false;
    }

// -----------------------------------------------------------------------------

Ogre::ColourValue EOSG3D::overlayTextAreaGetColor(const char* textAreaName)
    {
    Ogre::ColourValue color = Ogre::ColourValue::White;
    Ogre::TextAreaOverlayElement* textArea = overlayTextAreaElementByName(textAreaName);
    if (textArea != NULL)
        color = textArea->getColour();
    return color;
    }

// -----------------------------------------------------------------------------

Ogre::ColourValue EOSG3D::overlayTextAreaGetColorTop(const char* textAreaName)
    {
    Ogre::ColourValue color = Ogre::ColourValue::White;
    Ogre::TextAreaOverlayElement* textArea = overlayTextAreaElementByName(textAreaName);
    if (textArea != NULL)
        color = textArea->getColourTop();
    return color;
    }

// -----------------------------------------------------------------------------

Ogre::ColourValue EOSG3D::overlayTextAreaGetColorBottom(const char* textAreaName)
    {
    Ogre::ColourValue color = Ogre::ColourValue::White;
    Ogre::TextAreaOverlayElement* textArea = overlayTextAreaElementByName(textAreaName);
    if (textArea != NULL)
        color = textArea->getColourBottom();
    return color;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayElementSetVisible(const char* elementName, bool visible)
    {
    if (elementName != NULL)
        {
        Ogre::OverlayElement* element = this->overlayManager->getOverlayElement(elementName);
        if (element != NULL)
            {
            if (visible)
                element->show();
            else
                element->hide();
            return true;
            }
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][overlayElementSetVisible()] WARNING: overlay element '%s' not found. Skipping.", elementName);
        }
    return false;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayElementSetDimensions(const char* elementName, float width, float height)
    {
    if (elementName != NULL)
        {
        Ogre::OverlayElement* element = this->overlayManager->getOverlayElement(elementName);
        if (element != NULL)
            {
            element->setDimensions(width, height);
            return true;
            }
        }
    return false;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayElementSetPosition(const char* elementName, float x, float y)
    {
    if (elementName != NULL)
        {
        Ogre::OverlayElement* element = this->overlayManager->getOverlayElement(elementName);
        if (element != NULL)
            {
            element->setPosition(x, y);
            return true;
            }
        }
    return false;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayElementSetMaterial(const char* elementName, const char* materialName)
    {
    if (elementName != NULL && materialName != NULL)
        {
        Ogre::OverlayElement* element = this->overlayManager->getOverlayElement(elementName);
        if (element != NULL)
            {
            element->setMaterialName(materialName);
            return true;
            }
        }
    return false;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::overlayElementSetMetricsMode(const char* elementName, const char* metricsMode)
    {
    if (elementName != NULL && metricsMode != NULL)
        {
        Ogre::OverlayElement* element = this->overlayManager->getOverlayElement(elementName);
        if (element != NULL)
            {
            std::string metricsModeString = metricsMode;
            if (metricsModeString == "RELATIVE")
                element->setMetricsMode(Ogre::GMM_RELATIVE);
            else if (metricsModeString == "ABSOLUTE")
                element->setMetricsMode(Ogre::GMM_PIXELS);
            else if (metricsModeString == "VIRTUAL")
                element->setMetricsMode(Ogre::GMM_RELATIVE_ASPECT_ADJUSTED);
            else
                return false;
            //
            return true;
            }
        }
    return false;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

bool EOSG3D::billboardSetSetMaterial(const char* bbSetName, const char* materialName)
    {
    bool done = false;
    EOSOgreObject* bbSetObject = objectByName(bbSetName, "billboardSetSetMaterial");
    if (bbSetObject != NULL)
        {
        if (bbSetObject->getBillboardSet() != NULL)
            {
            if (materialName != NULL)
                bbSetObject->getBillboardSet()->setMaterialName(materialName);
            done = true;
            }
        }
    else
        EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardSetSetMaterial()] WARNING: object '%s' doesn't exists. Skipping.", bbSetName);
    //
    return done;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::billboardSetSetDefaultDimensions(const char* bbSetName, float width, float height)
    {
    bool done = false;
    EOSOgreObject* bbSetObject = objectByName(bbSetName, "billboardSetSetDefaultDimensions");
    if (bbSetObject != NULL)
        {
        if (bbSetObject->getBillboardSet() != NULL)
            {
            bbSetObject->getBillboardSet()->setDefaultDimensions(width, height);
            done = true;
            }
        }
    else
        EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardSetSetDefaultDimensions()] WARNING: object '%s' doesn't exists. Skipping.", bbSetName);
    //
    return done;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::billboardSetSetCastShadows(const char* bbSetName, bool shadowsActive)
    {
    bool done = false;
    EOSOgreObject* bbSetObject = objectByName(bbSetName, "billboardSetSetCastShadows");
    if (bbSetObject != NULL)
        {
        if (bbSetObject->getBillboardSet() != NULL)
            {
            bbSetObject->getBillboardSet()->setCastShadows(shadowsActive);
            done = true;
            }
        }
    else
        EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardSetSetCastShadows()] WARNING: object '%s' doesn't exists. Skipping.", bbSetName);
    //
    return done;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::billboardCreate(const char* bbSetName, float x, float y, float z, float r, float g, float b)
    {
    bool done = false;
    EOSOgreObject* bbSetObject = objectByName(bbSetName, "billboardCreate");
    if (bbSetObject != NULL)
        {
        if (bbSetObject->getBillboardSet() != NULL)
            {
            bbSetObject->getBillboardSet()->createBillboard(x, y, z, Ogre::ColourValue(r, g, b));
            done = true;
            }
        }
    else
        EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardCreate()] WARNING: object '%s' doesn't exists. Skipping.", bbSetName);
    //
    return done;
    }

// -----------------------------------------------------------------------------

float EOSG3D::billboardGetRotation(const char* bbSetName, unsigned int bbIndex)
    {
    float rotation = -1.0;
    EOSOgreObject* bbSetObject = objectByName(bbSetName, "billboardGetRotation");
    if (bbSetObject != NULL)
        {
        if (bbSetObject->getBillboardSet() != NULL)
            {
            Ogre::Billboard* billboard = bbSetObject->getBillboardSet()->getBillboard(bbIndex);
            if (billboard != NULL)
                rotation = billboard->getRotation().valueRadians();
            else
                EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardGetRotation()] WARNING: Billboard at index %d not found. Skipping.", bbIndex);
            }
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardGetRotation()] WARNING: object '%s' doesn't contain a valid BillboardSet. Skipping.", bbSetName);
        }
    //
    return rotation;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::billboardSetRotation(const char* bbSetName, unsigned int bbIndex, float rotationAngle)
    {
    bool done = true;
    EOSOgreObject* bbSetObject = objectByName(bbSetName, "billboardSetRotation");
    if (bbSetObject != NULL)
        {
        if (bbSetObject->getBillboardSet() != NULL)
            {
            Ogre::Billboard* billboard = bbSetObject->getBillboardSet()->getBillboard(bbIndex);
            if (billboard != NULL)
                {
                billboard->setRotation(Ogre::Radian(rotationAngle));
                done = true;
                }
            else
                EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardGetRotation()] WARNING: Billboard at index %d not found. Skipping.", bbIndex);
            }
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardGetRotation()] WARNING: object '%s' doesn't contain a valid BillboardSet. Skipping.", bbSetName);
        }
    //
    return done;
    }

// -----------------------------------------------------------------------------

Ogre::Vector3 EOSG3D::billboardGetPosition(const char* bbSetName, unsigned int bbIndex)
    {
    Ogre::Vector3 position = Ogre::Vector3::ZERO;
    EOSOgreObject* bbSetObject = objectByName(bbSetName, "billboardGetPosition");
    if (bbSetObject != NULL)
        {
        if (bbSetObject->getBillboardSet() != NULL)
            {
            Ogre::Billboard* billboard = bbSetObject->getBillboardSet()->getBillboard(bbIndex);
            if (billboard != NULL)
                position = billboard->getPosition();
            else
                EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardGetPosition()] WARNING: Billboard at index %d not found. Skipping.", bbIndex);
            }
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardGetPosition()] WARNING: object '%s' doesn't contain a valid BillboardSet. Skipping.", bbSetName);
        }
    //
    return position;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::billboardSetPosition(const char* bbSetName, unsigned int bbIndex, float x, float y, float z)
    {
    bool done = true;
    EOSOgreObject* bbSetObject = objectByName(bbSetName, "billboardSetPosition");
    if (bbSetObject != NULL)
        {
        if (bbSetObject->getBillboardSet() != NULL)
            {
            Ogre::Billboard* billboard = bbSetObject->getBillboardSet()->getBillboard(bbIndex);
            if (billboard != NULL)
                {
                billboard->setPosition(x, y, z);
                done = true;
                }
            else
                EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardSetPosition()] WARNING: Billboard at index %d not found. Skipping.", bbIndex);
            }
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardSetPosition()] WARNING: object '%s' doesn't contain a valid BillboardSet. Skipping.", bbSetName);
        }
    //
    return done;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::billboardSetDimensions(const char* bbSetName, unsigned int bbIndex, float width, float height)
    {
    bool done = true;
    EOSOgreObject* bbSetObject = objectByName(bbSetName, "billboardSetDimensions");
    if (bbSetObject != NULL)
        {
        if (bbSetObject->getBillboardSet() != NULL)
            {
            Ogre::Billboard* billboard = bbSetObject->getBillboardSet()->getBillboard(bbIndex);
            if (billboard != NULL)
                {
                billboard->setDimensions(width, height);
                done = true;
                }
            else
                EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardSetDimensions()] WARNING: Billboard at index %d not found. Skipping.", bbIndex);
            }
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardSetDimensions()] WARNING: object '%s' doesn't contain a valid BillboardSet. Skipping.", bbSetName);
        }
    //
    return done;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::billboardResetDimensions(const char* bbSetName, unsigned int bbIndex)
    {
    bool done = true;
    EOSOgreObject* bbSetObject = objectByName(bbSetName, "billboardResetDimensions");
    if (bbSetObject != NULL)
        {
        if (bbSetObject->getBillboardSet() != NULL)
            {
            Ogre::Billboard* billboard = bbSetObject->getBillboardSet()->getBillboard(bbIndex);
            if (billboard != NULL)
                {
                billboard->resetDimensions();
                done = true;
                }
            else
                EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardResetDimensions()] WARNING: Billboard at index %d not found. Skipping.", bbIndex);
            }
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardResetDimensions()] WARNING: object '%s' doesn't contain a valid BillboardSet. Skipping.", bbSetName);
        }
    //
    return done;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::billboardSetColor(const char* bbSetName, unsigned int bbIndex, float r, float g, float b)
    {
    bool done = true;
    EOSOgreObject* bbSetObject = objectByName(bbSetName, "billboardSetColor");
    if (bbSetObject != NULL)
        {
        if (bbSetObject->getBillboardSet() != NULL)
            {
            Ogre::Billboard* billboard = bbSetObject->getBillboardSet()->getBillboard(bbIndex);
            if (billboard != NULL)
                {
                billboard->setColour(Ogre::ColourValue(r, g, b));
                done = true;
                }
            else
                EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardSetColor()] WARNING: Billboard at index %d not found. Skipping.", bbIndex);
            }
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardSetColor()] WARNING: object '%s' doesn't contain a valid BillboardSet. Skipping.", bbSetName);
        }
    //
    return done;
    }

// -----------------------------------------------------------------------------

Ogre::ColourValue EOSG3D::billboardGetColor(const char* bbSetName, unsigned int bbIndex)
    {
    Ogre::ColourValue colourValue = Ogre::ColourValue::White;
    EOSOgreObject* bbSetObject = objectByName(bbSetName, "billboardGetColor");
    if (bbSetObject != NULL)
        {
        if (bbSetObject->getBillboardSet() != NULL)
            {
            Ogre::Billboard* billboard = bbSetObject->getBillboardSet()->getBillboard(bbIndex);
            if (billboard != NULL)
                colourValue = billboard->getColour();
            else
                EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardGetColor()] WARNING: Billboard at index %d not found. Skipping.", bbIndex);
            }
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardGetColor()] WARNING: object '%s' doesn't contain a valid BillboardSet. Skipping.", bbSetName);
        }
    //
    return colourValue;
    }

// -----------------------------------------------------------------------------

/* note: in returned vector, only x and y values are used */
Ogre::Vector3 EOSG3D::billboardGetDimensions(const char* bbSetName, unsigned int bbIndex)
    {
    Ogre::Vector3 dimensions = Ogre::Vector3::ZERO;
    EOSOgreObject* bbSetObject = objectByName(bbSetName, "billboardGetDimensions");
    if (bbSetObject != NULL)
        {
        if (bbSetObject->getBillboardSet() != NULL)
            {
            Ogre::Billboard* billboard = bbSetObject->getBillboardSet()->getBillboard(bbIndex);
            if (billboard != NULL)
                {
                dimensions.x = billboard->getOwnWidth();
                dimensions.y = billboard->getOwnHeight();
                }
            else
                EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardGetDimensions()] WARNING: Billboard at index %d not found. Skipping.", bbIndex);
            }
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos.g3d][billboardGetDimensions()] WARNING: object '%s' doesn't contain a valid BillboardSet. Skipping.", bbSetName);
        }
    //
    return dimensions;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

bool EOSG3D::createSimpleSpline(const char* simpleSplineName, unsigned int maxPoints)
    {
    if (simpleSplineName != NULL)
        {
        //EOSOgreSimpleSpline* newSimpleSpline = (maxPoints != 0) ? new EOSOgreSimpleSpline(maxPoints) : new EOSOgreSimpleSpline();
        EOSOgreSimpleSpline* newSimpleSpline = new EOSOgreSimpleSpline();
        if (newSimpleSpline != NULL)
            {
            //newSimpleSpline->setAutoCalculate(true);
            simpleSplines.insert(std::pair<std::string, EOSOgreSimpleSpline*>(simpleSplineName, newSimpleSpline));
            return true;
            }
        }
    return false;
    }

// -----------------------------------------------------------------------------

EOSOgreSimpleSpline* EOSG3D::simpleSplineByName(const char* simpleSplineName)
    {
    EOSOgreSimpleSpline* spline = NULL;
    if (simpleSplineName != NULL)
        {
        std::map<std::string, EOSOgreSimpleSpline*>::iterator it;
        it = simpleSplines.find(simpleSplineName);
        if (it != simpleSplines.end()) // name found
            spline = (*it).second;
        }
    return spline;
    }

// -----------------------------------------------------------------------------

bool EOSG3D::createRotationalSpline(const char* rotationalSplineName, unsigned int maxPoints)
    {
    if (rotationalSplineName != NULL)
        {
        EOSOgreRotationalSpline* newRotationalSpline = (maxPoints != 0) ? new EOSOgreRotationalSpline(maxPoints) : new EOSOgreRotationalSpline();
        if (newRotationalSpline != NULL)
            {
            newRotationalSpline->setAutoCalculate(true);
            rotationalSplines.insert(std::pair<std::string, EOSOgreRotationalSpline*>(rotationalSplineName, newRotationalSpline));
            return true;
            }
        }
    return false;
    }

// -----------------------------------------------------------------------------

EOSOgreRotationalSpline* EOSG3D::rotationalSplineByName(const char* rotationalSplineName)
    {
    EOSOgreRotationalSpline* spline = NULL;
    if (rotationalSplineName != NULL)
        {
        std::map<std::string, EOSOgreRotationalSpline*>::iterator it;
        it = rotationalSplines.find(rotationalSplineName);
        if (it != rotationalSplines.end()) // name found
            spline = (*it).second;
        }
    return spline;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void EOSG3D::unloadUnreferencedTextures(bool reloadableOnly)
    {
    this->gfxRoot->getTextureManager()->unloadUnreferencedResources(reloadableOnly);
    }

// -----------------------------------------------------------------------------

void EOSG3D::unloadUnreferencedMeshes(bool reloadableOnly)
    {
    this->gfxRoot->getMeshManager()->unloadUnreferencedResources(reloadableOnly);
    }

// -----------------------------------------------------------------------------

bool EOSG3D::bindNodeToEOSObject(const char* sceneNodeName, EOSObject* externalObject)
    {
    Ogre::SceneNode* sceneNode = nodeByName(sceneNodeName, "bindNodeToEOSObject");
    if (sceneNode != NULL && externalObject != NULL)
        {
        if (strcmp(externalObject->getType(), "3D_OBJECT") == 0)
            {
            ControlledSceneNode* controlledSceneNode = new ControlledSceneNode;
            controlledSceneNode->sceneNode = sceneNode;
            controlledSceneNode->externalObject = (eos_3dobject*) externalObject->getUserData();
            //
            this->controlledSceneNodes.push_back(controlledSceneNode);
            //
            return true;
            }
        else
            EOSLuaRuntime::getInstance()->logErrorMessage("[eos.g3d][bindNodeToEOSObject()] ERROR: Bad object type. Can only accept 3D_OBJECT type.");
        }
    return false;
    }

/*Ogre::Vector3 EOSG3D::getPositionFromLuaTable(const char* luaTableName)
        {
        Ogre::Vector3 pos = Ogre::Vector3::ZERO;
        if(luaTableName != NULL)
                {
                char luaCommand[512];
                lua_State* luaState = EOSLuaRuntime::getInstance()->getRuntime();
                //
                snprintf(luaCommand, 510, "__tmpLuaValue1 = %s.position.x  __tmpLuaValue2 = %s.position.y  __tmpLuaValue3 = %s.position.z", luaTableName, luaTableName, luaTableName);
                if(EOSLuaRuntime::getInstance()->executeLuaChunk(luaCommand))
                        {
                        lua_getglobal(luaState, "__tmpLuaValue1");
                        if(!lua_isnil(luaState, -1))
                                pos.x = (float)lua_tonumber(luaState, -1);
                        lua_pop(luaState, 1);
                        //
                        lua_getglobal(luaState, "__tmpLuaValue2");
                        if(!lua_isnil(luaState, -1))
                                pos.y = (float)lua_tonumber(luaState, -1);
                        lua_pop(luaState, 1);
                        //
                        lua_getglobal(luaState, "__tmpLuaValue3");
                        if(!lua_isnil(luaState, -1))
                                pos.z = (float)lua_tonumber(luaState, -1);
                        lua_pop(luaState, 1);
                        }
                }
        return pos;
        }*/

// -----------------------------------------------------------------------------

