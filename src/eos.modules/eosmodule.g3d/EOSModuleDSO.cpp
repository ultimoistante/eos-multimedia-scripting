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


#include <stdio.h>
#include <string>

#include "EOSG3D.h"
#include "EOSOgreDotSceneLoader.h"

extern "C"
    {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "EOSLuaRuntime.h"

    // ----------------------------------------------------------------------------
    // ----------------------------------------------------------------------------
    //
    // Module manifest structure. Developer must edit just the #define values below.
    //
#define _MODULE_NAME            "g3d"
#define _MODULE_VERSION         "0.9.5"
#define _MODULE_DATE		        "November 05, 2012"
#define _MODULE_DESC_SHORT      "OGRE (Object-Oriented Graphics Rendering Engine) 3D Graphics Module for EOS environment"
#define _MODULE_DESC_LONG       "This module uses OGRE (Object-Oriented Graphics Rendering Engine) - www.ogre3d.org"
#define _MODULE_AUTHOR_NAME     "EOS Scripting Team"
#define _MODULE_AUTHOR_WEBSITE  "http://sourceforge.net/projects/eos-scripting/ | http://ultimoistante.wordpress.com/"
    //
    // ----------------------------------------------------------------------------
    // ----------------------------------------------------------------------------

#define COLLISIONSHAPE_SPHERE    1
#define COLLISIONSHAPE_BOX       2
#define COLLISIONSHAPE_MESH      3
#define COLLISIONSHAPE_RAY       4

    //#define _MODULE_NAME              "eos.g3d" // I use this for "checkparameters" functions

    //std::vector<int*> exportedDataSources;

    // -----------------------------------------------------------------------------
    // utility functions -----------------------------------------------------------

    bool checkModuleInitialized()
        {
        if (!EOSG3D::getInstance()->isInitialized())
            {
            EOSLuaRuntime::getInstance()->logWarningMessage("[eos."_MODULE_NAME"] Warning: eos."_MODULE_NAME" not initialized. Please initialize it, first.");
            return false;
            }
        return true;
        }

    // end utility functions -------------------------------------------------------

    // -----------------------------------------------------------------------------
    // ====================== COMMON REQUIRED FUNCTIONS ============================
    // -----------------------------------------------------------------------------

    static int moduleCleanup(lua_State* luaRuntime)
        {
        /*
         * insert here code for resources cleanup
         * on module unload (automatically done by kernel on close)
         */
        EOSG3D::getInstance()->shutdown();
        //
        return 0;
        }

    // ====================== CUSTOM MODULE FUNCTIONS ==============================
    // -----------------------------------------------------------------------------

    /**
            <function name="initialize">
                    <language code="en">
                            <brief text="Initializes Ogre3D module. This is the first function you must call to use the module." />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Inizializza il modulo Ogre3D. Questa e' la prima funzione da chiamare, per poter utilizzare questo modulo." />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    static int initialize(lua_State* luaRuntime)
        {
        bool done = false;
        if (EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "initialize", 1))
            done = EOSG3D::getInstance()->powerOn((int) lua_tonumber(luaRuntime, 1));
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="update">
                    <language code="en">
                            <brief text="Updates Ogre3D engine status (but *not* renders frame output)" />
                            <description text="" />
                            <params>
                                    <param name="frameTime" type="Numeric" desc="update time, in seconds" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    static int update(lua_State* luaRuntime)
        {
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "update", 1))
            EOSG3D::getInstance()->update(lua_tonumber(luaRuntime, 1));
        //
        return 0;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="render">
                    <language code="en">
                            <brief text="Draws Ogre3D render targets (that means 'drawing a frame')" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    static int render(lua_State* luaRuntime)
        {
        if (checkModuleInitialized())
            EOSG3D::getInstance()->render();
        //
        return 0;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="shutdown">
                    <language code="en">
                            <brief text="Shuts down Ogre3D engine" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Chiude l'istanza del motore grafico Ogre3D" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    static int shutdown(lua_State* luaRuntime)
        {
        bool done = EOSG3D::getInstance()->shutdown();
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="createRenderSurface">
                    <language code="en">
                            <brief text="Create a renderTarget to bind OGRE with an external surface" />
                            <description text="" />
                            <params>				
                                    <param name="width"  type="Numeric" desc="pixel width of the buffer" />
                                    <param name="height" type="Numeric" desc="pixel height of the buffer" />
                            </params>
                            <returns>
                                    <return name="textureId" type="Numeric" desc="texture id of the render texture" />
                            </returns>
                    </language>
            </function>
     */
    static int createRenderSurface(lua_State* luaRuntime)
        {
        void* eosobject = NULL;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "createRenderSurface", 3))
            eosobject = EOSG3D::getInstance()->createRenderSurface(luaL_checkstring(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3));
        //
        lua_pushlightuserdata(luaRuntime, eosobject);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="pointToScreenCoordinates">
                    <language code="en">
                            <brief text="" />
                            <description text="Returns the screen-projected position of a 3D point (based on current camera view)" />
                            <params>
                                    <param name="x" type="Numeric" desc="X component of the 3D point" />
                                    <param name="y" type="Numeric" desc="Y component of the 3D point" />
                                    <param name="z" type="Numeric" desc="Z component of the 3D point" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="X coordinate of the point onto the screen" />
                                    <return name="y" type="Numeric" desc="Y coordinate of the point onto the screen" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="" />
                            <description text="Ritorna la posizione proiettata sullo schermo di un punto 3D (basato sulla visuale corrente della camera)" />
                            <params>
                                    <param name="x" type="Numeric" desc="componente X del punto 3D" />
                                    <param name="y" type="Numeric" desc="componente Y del punto 3D" />
                                    <param name="z" type="Numeric" desc="componente Z del punto 3D" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="coordinata X del punto a schermo" />
                                    <return name="y" type="Numeric" desc="coordinata Y del punto a schermo" />
                            </returns>
                    </language>
            </function>
     */
    int pointToScreenCoordinates(lua_State* luaRuntime)
        {
        Ogre::Real x = 0.0;
        Ogre::Real y = 0.0;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "pointToScreenCoordinates", 3))
            {
            Ogre::Vector3 point = Ogre::Vector3(lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3));
            Ogre::Camera* camera = EOSG3D::getInstance()->getDefaultCamera();
            // multiply the point by the view matrix to get a camera-space point
            point = camera->getProjectionMatrix() * (camera->getViewMatrix() * point);
            // make 2D relative/normalized coords from the view-space vertex
            x = 1.0f - ((point.x * 0.5f) + 0.5);
            y = 1.0f - ((point.y * 0.5f) + 0.5);
            }
        //
        lua_pushnumber(luaRuntime, x);
        lua_pushnumber(luaRuntime, y);
        return 2;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="worldSetSkyBox">
                    <language code="en">
                            <brief text="Sets the skyBox of the current scene" />
                            <description text="" />
                            <params>
                                    <param name="skyBoxName"  type="String" desc="name of the skyBox to set" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta lo skyBox della scena corrente" />
                            <description text="" />
                            <params>
                                    <param name="skyBoxName"  type="String" desc="nome dello skyBox da settare" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int worldSetSkyBox(lua_State* luaRuntime)
        {
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "world.setSkyBox", 1))
            EOSG3D::getInstance()->getSceneManager()->setSkyBox(true, luaL_checkstring(luaRuntime, 1));
        return 0;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="worldSetSkyDome">
                    <language code="en">
                            <brief text="Sets the skyDome of the current scene" />
                            <description text="" />
                            <params>
                                    <param name="skyBoxName"  type="String" desc="name of the skyDome to set" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta lo skyDome della scena corrente" />
                            <description text="" />
                            <params>
                                    <param name="skyBoxName"  type="String" desc="nome dello skyDome da settare" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int worldSetSkyDome(lua_State* luaRuntime)
        {
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "world.setSkyDome", 1))
            EOSG3D::getInstance()->getSceneManager()->setSkyDome(true, luaL_checkstring(luaRuntime, 1));
        return 0;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="worldSetAmbientLight">
                    <language code="en">
                            <brief text="Sets the ambiental light color of the current scene" />
                            <description text="" />
                            <params>
                                    <param name="r" type="Numeric" desc="Red   amount of the light color to set (0.0 to 1.0)" />
                                    <param name="g" type="Numeric" desc="Green amount of the light color to set (0.0 to 1.0)" />
                                    <param name="b" type="Numeric" desc="Blue  amount of the light color to set (0.0 to 1.0)" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il colore della luce ambientale della scena corrente" />
                            <description text="" />
                            <params>
                                    <param name="r" type="Numeric" desc="Livello del Rosso della luce ambientale da settare (compreso tra 0.0 e 1.0)" />
                                    <param name="g" type="Numeric" desc="Livello del Verde della luce ambientale da settare (compreso tra 0.0 e 1.0)" />
                                    <param name="b" type="Numeric" desc="Livello del Blu   della luce ambientale da settare (compreso tra 0.0 e 1.0)" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int worldSetAmbientLight(lua_State* luaRuntime)
        {
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "world.setAmbientLight", 3))
            EOSG3D::getInstance()->getSceneManager()->setAmbientLight(Ogre::ColourValue((Ogre::Real)lua_tonumber(luaRuntime, 1), (Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3)));
        return 0;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="worldGetAmbientLight">
                    <language code="en">
                            <brief text="Returns the ambiental light color of the current scene" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="r" type="Numeric" desc="Red   amount of the current ambiental light color (0.0 to 1.0)" />
                                    <return name="g" type="Numeric" desc="Green amount of the current ambiental light color (0.0 to 1.0)" />
                                    <return name="b" type="Numeric" desc="Blue  amount of the current ambiental light color (0.0 to 1.0)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il colore della luce ambientale della scena corrente" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="r" type="Numeric" desc="Livello del Rosso della luce ambientale corrente (compreso tra 0.0 e 1.0)" />
                                    <return name="g" type="Numeric" desc="Livello del Verde della luce ambientale corrente (compreso tra 0.0 e 1.0)" />
                                    <return name="b" type="Numeric" desc="Livello del Blu  della luce ambientale corrente (compreso tra 0.0 e 1.0)" />
                            </returns>
                    </language>
            </function>
     */
    int worldGetAmbientLight(lua_State* luaRuntime)
        {
        Ogre::ColourValue ambientColor = Ogre::ColourValue::White;
        if (checkModuleInitialized())
            {
            ambientColor = EOSG3D::getInstance()->getSceneManager()->getAmbientLight();
            }
        lua_pushnumber(luaRuntime, ambientColor.r);
        lua_pushnumber(luaRuntime, ambientColor.g);
        lua_pushnumber(luaRuntime, ambientColor.b);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="worldLoadDotScene">
                    <language code="en">
                            <brief text="Loads a scene in dotScene 1.0 format" />
                            <description text="This function loads a 3d scene from a xml file formatted as dotScene 1.0 DTD" />
                            <params>
                                    <param name="dotSceneFile" type="String" desc="Name of the .scene file to load" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Carica una scena 3d in formato dotScene 1.0" />
                            <description text="Questa funzione carica una scena 3d da un file xml formattato secondo lo schema (DTD) dotScene 1.0" />
                            <params>
                                    <param name="dotSceneFile" type="String" desc="Nome del file .scene da caricare" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int worldLoadDotScene(lua_State* luaRuntime)
        {
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "world.loadDotScene", 1))
            {
            EOSOgreDotSceneLoader dotSceneLoader(EOSG3D::getInstance());
            dotSceneLoader.loadDotScene(luaL_checkstring(luaRuntime, 1));
            }
        return 0;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="worldSetFog">
                    <language code="en">
                            <brief text="Sets the environmental fog" />
                            <description text="" />
                            <params>
                                    <param name="fogMode" type="String" desc="fog mode: a string between NONE, EXP, EXP2, LINEAR " />
                                    <param name="fogColor_r"  type="Numeric" desc="Red amount of the fog color" />
                                    <param name="fogColor_g"  type="Numeric" desc="Green amount of the fog color" />
                                    <param name="fogColor_b"  type="Numeric" desc="Blue amount of the fog color" />
                                    <param name="expDensity"  type="Numeric" desc="The density of the fog in EXP or EXP2 mode, as a value between 0 and 1" />
                                    <param name="linearStart" type="Numeric" desc="Distance in world units at which linear fog starts to encroach. Only applicable if mode is LINEAR" />
                                    <param name="linearEnd"   type="Numeric" desc="Distance in world units at which linear fog becomes completely opaque. Only applicable if mode is LINEAR" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="" />
                            <description text="" />
                            <params>
                                    <param name="dotSceneFile" type="String" desc="Nome del file .scene da caricare" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int worldSetFog(lua_State* luaRuntime)
        {
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "world.setFog", 7))
            {
            std::string fogModeString = luaL_checkstring(luaRuntime, 1);
            std::transform(fogModeString.begin(), fogModeString.end(), fogModeString.begin(), toupper);
            //LOGUSER(fogModeString.c_str());
            Ogre::FogMode mode = Ogre::FOG_NONE;
            if (fogModeString == "NONE")
                mode = Ogre::FOG_NONE;
            else if (fogModeString == "EXP")
                mode = Ogre::FOG_EXP;
            else if (fogModeString == "EXP2")
                mode = Ogre::FOG_EXP2;
            else if (fogModeString == "LINEAR")
                mode = Ogre::FOG_LINEAR;
            else
                EOSLuaRuntime::getInstance()->logWarningMessage("[eos."_MODULE_NAME"][world.setFog] - warning: unrecognized fog mode '%s'. Using default value ('NONE', fog disabled).", fogModeString.c_str());
            //
            EOSG3D::getInstance()->getSceneManager()->setFog(mode, Ogre::ColourValue((Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4)), (Ogre::Real)lua_tonumber(luaRuntime, 5), (Ogre::Real)lua_tonumber(luaRuntime, 6), (Ogre::Real)lua_tonumber(luaRuntime, 7));
            }
        return 0;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="worldClear">
                    <language code="en">
                            <brief text="" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int worldClear(lua_State* luaRuntime)
        {
        if (checkModuleInitialized())
            EOSG3D::getInstance()->clearScene();
        return 0;
        }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="cameraSetPosition">
                    <language code="en">
                            <brief text="Sets absolute camera position" />
                            <description text="" />
                            <params>
                                    <param name="x" type="Numeric" desc="Absolute X position where to set the camera" />
                                    <param name="y" type="Numeric" desc="Absolute Y position where to set the camera" />
                                    <param name="z" type="Numeric" desc="Absolute Z position where to set the camera" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la posizione assoluta della camera" />
                            <description text="" />
                            <params>
                                    <param name="x" type="Numeric" desc="Coordinata assoluta X dove posizionare la camera" />
                                    <param name="y" type="Numeric" desc="Coordinata assoluta Y dove posizionare la camera" />
                                    <param name="z" type="Numeric" desc="Coordinata assoluta Z dove posizionare la camera" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int cameraSetPosition(lua_State* luaRuntime)
        {
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "camera.setPosition", 3))
            {
            EOSG3D::getInstance()->getDefaultCameraNode()->setPosition((Ogre::Real)lua_tonumber(luaRuntime, 1), (Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3));
            //EOSG3D::getInstance()->getDefaultCamera()->setPosition((Ogre::Real)lua_tonumber(luaRuntime, 1), (Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3));
            }
        return 0;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="cameraGetPosition">
                    <language code="en">
                            <brief text="Returns current absolute camera position" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="Absolute X coordinate of current camera position" />
                                    <return name="y" type="Numeric" desc="Absolute Y coordinate of current camera position" />
                                    <return name="z" type="Numeric" desc="Absolute Z coordinate of current camera position" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la posizione assoluta corrente della camera" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="Coordinata X assoluta della posizione corrente della camera" />
                                    <return name="y" type="Numeric" desc="Coordinata Y assoluta della posizione corrente della camera" />
                                    <return name="z" type="Numeric" desc="Coordinata Z assoluta della posizione corrente della camera" />
                            </returns>
                    </language>
            </function>
     */
    int cameraGetPosition(lua_State* luaRuntime)
        {
        Ogre::Vector3 position = Ogre::Vector3::ZERO;
        if (checkModuleInitialized())
            position = EOSG3D::getInstance()->getDefaultCameraNode()->_getDerivedPosition(); // was getWorldPosition();
        lua_pushnumber(luaRuntime, position.x);
        lua_pushnumber(luaRuntime, position.y);
        lua_pushnumber(luaRuntime, position.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="cameraGetOrientation">
                    <language code="en">
                            <brief text="Returns current absolute camera orientation" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="X component of the quaternion representing current camera orientation" />
                                    <return name="y" type="Numeric" desc="Y component of the quaternion representing current camera orientation" />
                                    <return name="z" type="Numeric" desc="Z component of the quaternion representing current camera orientation" />
                                    <return name="w" type="Numeric" desc="W component of the quaternion representing current camera orientation" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna l'orientamento corrente della camera" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="Componente X del quaternione rappresentante l'orientamento corrente della camera" />
                                    <return name="y" type="Numeric" desc="Componente Y del quaternione rappresentante l'orientamento corrente della camera" />
                                    <return name="z" type="Numeric" desc="Componente Z del quaternione rappresentante l'orientamento corrente della camera" />
                                    <return name="w" type="Numeric" desc="Componente W del quaternione rappresentante l'orientamento corrente della camera" />
                            </returns>
                    </language>
            </function>
     */
    int cameraGetOrientation(lua_State* luaRuntime)
        {
        Ogre::Quaternion orientation = Ogre::Quaternion::IDENTITY;
        if (checkModuleInitialized())
            orientation = EOSG3D::getInstance()->getDefaultCameraNode()->_getDerivedOrientation(); // was getWorldOrientation()
        lua_pushnumber(luaRuntime, orientation.x);
        lua_pushnumber(luaRuntime, orientation.y);
        lua_pushnumber(luaRuntime, orientation.z);
        lua_pushnumber(luaRuntime, orientation.w);
        return 4;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="cameraSetOrientation">
                    <language code="en">
                            <brief text="Sets current absolute camera orientation" />
                            <description text="" />
                            <params>
                                    <param name="x" type="Numeric" desc="X component of the orientation quaternion to set" />
                                    <param name="y" type="Numeric" desc="Y component of the orientation quaternion to set" />
                                    <param name="z" type="Numeric" desc="Z component of the orientation quaternion to set" />
                                    <param name="w" type="Numeric" desc="W component of the orientation quaternion to set" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta l'orientamento corrente della camera" />
                            <description text="" />
                            <params>
                                    <param name="x" type="Numeric" desc="componente X del quaternione di orientamento da settare" />
                                    <param name="y" type="Numeric" desc="componente Y del quaternione di orientamento da settare" />
                                    <param name="z" type="Numeric" desc="componente Z del quaternione di orientamento da settare" />
                                    <param name="w" type="Numeric" desc="componente W del quaternione di orientamento da settare" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int cameraSetOrientation(lua_State* luaRuntime)
        {
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "camera.setOrientation", 4))
            {
            Ogre::Quaternion q = Ogre::Quaternion::IDENTITY;
            q.x = luaL_checknumber(luaRuntime, 1);
            q.y = luaL_checknumber(luaRuntime, 2);
            q.z = luaL_checknumber(luaRuntime, 3);
            q.w = luaL_checknumber(luaRuntime, 4);
            EOSG3D::getInstance()->getDefaultCameraNode()->setOrientation(q);
            }
        return 0;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="cameraSetFOV">
                    <language code="en">
                            <brief text="Sets current camera FOV (Field Of View) angle" />
                            <description text="" />
                            <params>
                                    <param name="fovAngle" type="Numeric" desc="Angle of the FOV to set" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta l'angolo di FOV (Field Of View) corrente della camera" />
                            <description text="" />
                            <params>
                                    <param name="fovAngle" type="Numeric" desc="Angolo del FOV da settare" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int cameraSetFOV(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "camera.setFOV", 1))
            {
            EOSG3D::getInstance()->getDefaultCamera()->setFOVy(Ogre::Angle(lua_tonumber(luaRuntime, 1)));
            done = true;
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="cameraLookAt">
                    <language code="en">
                            <brief text="Sets current camera look-at point" />
                            <description text="" />
                            <params>
                                    <param name="x" type="Numeric" desc="X component of the absolute position where to point the camera" />
                                    <param name="y" type="Numeric" desc="Y component of the absolute position where to point the camera" />
                                    <param name="z" type="Numeric" desc="Z component of the absolute position where to point the camera" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il punto corrente di inquadratura della camera" />
                            <description text="" />
                            <params>
                                    <param name="x" type="Numeric" desc="Componente X assoluta del punto da inquadrare" />
                                    <param name="y" type="Numeric" desc="Componente Y assoluta del punto da inquadrare" />
                                    <param name="z" type="Numeric" desc="Componente Z assoluta del punto da inquadrare" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int cameraLookAt(lua_State* luaRuntime)
        {
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "camera.lookAt", 3))
            EOSG3D::getInstance()->getDefaultCamera()->lookAt((Ogre::Real)lua_tonumber(luaRuntime, 1), (Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3));
        return 0;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="cameraTranslate">
                    <language code="en">
                            <brief text="Translates the camera along a vector" />
                            <description text="" />
                            <params>
                                    <param name="x" type="Numeric" desc="Displacement along X axis" />
                                    <param name="y" type="Numeric" desc="Displacement along Y axis" />
                                    <param name="z" type="Numeric" desc="Displacement along Z axis" />
                                    <param name="absRelMode" type="Boolean" desc="true to translate along absolute (world) axes, false to translate along local axes" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Trasla la camera lungo un vettore" />
                            <description text="" />
                            <params>
                                    <param name="x" type="Numeric" desc="Traslazione sull'asse X" />
                                    <param name="y" type="Numeric" desc="Traslazione sull'asse Y" />
                                    <param name="z" type="Numeric" desc="Traslazione sull'asse Z" />
                                    <param name="absRelMode" type="Boolean" desc="true per traslare lungo gli assi assoluti (world), false per traslare lungo gli assi locali" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int cameraTranslate(lua_State* luaRuntime)
        {
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "camera.translate", 4))
            EOSG3D::getInstance()->getDefaultCameraNode()->translate((Ogre::Real)lua_tonumber(luaRuntime, 1), (Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), ((bool)lua_toboolean(luaRuntime, 4) ? Ogre::Node::TS_WORLD : Ogre::Node::TS_LOCAL));
        return 0;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="cameraRotateByAngles">
                    <language code="en">
                            <brief text="Rotates the camera by pitch, yaw and roll" />
                            <description text="" />
                            <params>
                                    <param name="pitch" type="Numeric" desc="Rotation (in degrees) around local X axis" />
                                    <param name="yaw"   type="Numeric" desc="Rotation (in degrees) around local Y axis" />
                                    <param name="roll"  type="Numeric" desc="Rotation (in degrees) around local Z axis" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ruota la camera per pitch, yaw e roll" />
                            <description text="" />
                            <params>
                                    <param name="pitch" type="Numeric" desc="Rotazione (in gradi) attorno all'asse X locale" />
                                    <param name="yaw"   type="Numeric" desc="Rotazione (in gradi) attorno all'asse Y locale" />
                                    <param name="roll"  type="Numeric" desc="Rotazione (in gradi) attorno all'asse Z locale" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int cameraRotateByAngles(lua_State* luaRuntime)
        {
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "camera.rotateByAngles", 3))
            {
            Ogre::SceneNode* cameraNode = EOSG3D::getInstance()->getDefaultCameraNode();
            cameraNode->pitch(Ogre::Radian(Ogre::Degree(lua_tonumber(luaRuntime, 1))));
            cameraNode->yaw(Ogre::Radian(Ogre::Degree(lua_tonumber(luaRuntime, 2))));
            cameraNode->roll(Ogre::Radian(Ogre::Degree(lua_tonumber(luaRuntime, 3))));
            }
        return 0;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="cameraRotateByAxisAngle">
                    <language code="en">
                            <brief text="Rotates the camera by axis and angle" />
                            <description text="" />
                            <params>
                                    <param name="axis_x" type="Numeric" desc="X component of the rotation axis" />
                                    <param name="axis_y" type="Numeric" desc="Y component of the rotation axis" />
                                    <param name="axis_z" type="Numeric" desc="Z component of the rotation axis" />
                                    <param name="angle" type="Numeric" desc="rotation angle (in degrees)" />
                                    <param name="absRel" type="Numeric" desc="true for absolute rotation (world), false for relative (local) rotation" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ruota la camera per asse ed angolo" />
                            <description text="" />
                            <params>
                                    <param name="axis_x" type="Numeric" desc="componente X dell'asse di rotazione" />
                                    <param name="axis_y" type="Numeric" desc="componente Y dell'asse di rotazione" />
                                    <param name="axis_z" type="Numeric" desc="componente Z dell'asse di rotazione" />
                                    <param name="angle" type="Numeric" desc="angolo di rotazione (in gradi)" />
                                    <param name="absRel" type="Numeric" desc="true per rotazione assoluta (world), false per rotazione relativa (local)" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int cameraRotateByAxisAngle(lua_State* luaRuntime)
        {
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "camera.rotateByAxisAngle", 5))
            EOSG3D::getInstance()->getDefaultCameraNode()->rotate(Ogre::Vector3((Ogre::Real)lua_tonumber(luaRuntime, 1), (Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3)), Ogre::Radian(Ogre::Degree((Ogre::Real)lua_tonumber(luaRuntime, 4))), (lua_toboolean(luaRuntime, 5) ? Ogre::Node::TS_WORLD : Ogre::Node::TS_LOCAL));
        //
        return 0;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="cameraGetNodeName">
                    <language code="en">
                            <brief text="Returns the name of the node where default camera is attached" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="nodeName" type="String" desc="name of the scene node" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il nome dello scene node al quale e' attaccata la default camera" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="nodeName" type="String" desc="nome dello scene node" />
                            </returns>
                    </language>
            </function>
     */
    int cameraGetNodeName(lua_State* luaRuntime)
        {
        std::string nodeName = "";
        if (checkModuleInitialized())
            nodeName = EOSG3D::getInstance()->getDefaultCameraNode()->getName();
        lua_pushstring(luaRuntime, nodeName.c_str());
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="cameraSetAutoTracking">
                    <language code="en">
                            <brief text="Enables automatic camera tracking of a SceneNode" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="Name of the scene node to track" />
                                    <param name="x_offset" type="Numeric" desc="X offset from the scene node position where look at" />
                                    <param name="y_offset" type="Numeric" desc="Y offset from the scene node position where look at" />
                                    <param name="z_offset" type="Numeric" desc="Z offset from the scene node position where look at" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true if success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Attiva l'inquadratura automatica di uno SceneNode da parte della camera" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="Nome dello scene node da tenere inquadrato" />
                                    <param name="x_offset" type="Numeric" desc="Offset X dell'inquadratura dalla posizione del nodo" />
                                    <param name="y_offset" type="Numeric" desc="Offset Y dell'inquadratura dalla posizione del nodo" />
                                    <param name="z_offset" type="Numeric" desc="Offset Z dell'inquadratura dalla posizione del nodo" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int cameraSetAutoTracking(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "camera.setAutoTracking", 4))
            {
            std::string nodeName = luaL_checkstring(luaRuntime, 1);
            Ogre::SceneNode* trackedNode = EOSG3D::getInstance()->getSceneManager()->getSceneNode(nodeName.c_str());
            if (trackedNode != NULL)
                {
                EOSG3D::getInstance()->getDefaultCamera()->setAutoTracking(true, trackedNode, Ogre::Vector3((Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4)));
                done = true;
                }
            else
                LOGERROR("[eos."_MODULE_NAME"][camera.setAutoTracking] - unable to find node '%s'.", nodeName.c_str());
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="cameraDisableAutoTracking">
                    <language code="en">
                            <brief text="Disables automatic camera tracking of a SceneNode previously set" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true if success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Disattiva l'inquadratura automatica di uno SceneNode da parte della camera" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int cameraDisableAutoTracking(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized())
            {
            EOSG3D::getInstance()->getDefaultCamera()->setAutoTracking(false, NULL);
            done = true;
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="nodeExists">
                    <language code="en">
                            <brief text="Checks the existence of a given SceneNode in the scene tree" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the SceneNode to check" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true if node exists, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Testa l'esistenza di un determinato SceneNode nell'albero della scena corrente" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome dello SceneNode da testare" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true se il nodo esiste, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeExists(lua_State* luaRuntime)
        {
        bool result = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.exists", 1))
            result = (EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.exists") != NULL);
        lua_pushboolean(luaRuntime, result);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeCreate">
                    <language code="en">
                            <brief text="Creates a new SceneNode in the scene tree" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the new SceneNode to create" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Crea un nuovo SceneNode nell'albero della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nuovo SceneNode da creare" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeCreate(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.create", 1))
            {
            Ogre::SceneNode* newSceneNode = static_cast<Ogre::SceneNode*> (EOSG3D::getInstance()->getSceneManager()->getRootSceneNode()->createChild(luaL_checkstring(luaRuntime, 1)));
            if (newSceneNode != NULL)
                {
                newSceneNode->setPosition(Ogre::Vector3::ZERO);
                newSceneNode->setOrientation(Ogre::Quaternion::IDENTITY);
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeReparent">
                    <language code="en">
                            <brief text="Reparents a SceneNode (detaches a node from its current parent and reattaches it under another node)" />
                            <description text="" />
                            <params>
                                    <param name="nodeOne" type="String" desc="name of the node to reparent" />
                                    <param name="nodeTwo" type="String" desc="name of the node where to attach nodeOne" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Sposta uno SceneNode sotto un altro SceneNode (stacca un nodo dal suo padre corrente e lo riattacca sotto un altro nodo)" />
                            <description text="" />
                            <params>
                                    <param name="nodeOne" type="String" desc="nome del nodo da spostare" />
                                    <param name="nodeTwo" type="String" desc="nome del nodo sotto il quale spostare nodeOne" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeReparent(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.reparent", 2))
            {
            Ogre::SceneNode* sceneNodeOne = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.reparent");
            Ogre::SceneNode* sceneNodeTwo = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 2), "node.reparent");
            if (sceneNodeOne != NULL && sceneNodeTwo != NULL)
                {
                sceneNodeOne->getParent()->removeChild(sceneNodeOne);
                sceneNodeTwo->addChild(sceneNodeOne);
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeGetParentNodeName">
                    <language code="en">
                            <brief text="" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the interested node" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="" />
                            <description text="" />
                            <params>
                                    <param name="nodeOne" type="String" desc="nome del nodo interessato" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeGetParentNodeName(lua_State* luaRuntime)
        {
        const char* parentNodeName = NULL;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.getParentNodeName", 1))
            {
            Ogre::SceneNode* sceneNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.getParentNodeName");
            if (sceneNode != NULL)
                {
                Ogre::SceneNode* parentNode = sceneNode->getParentSceneNode();
                if (parentNode != NULL)
                    parentNodeName = parentNode->getName().c_str();
                }
            }
        lua_pushstring(luaRuntime, parentNodeName);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeGetPosition">
                    <language code="en">
                            <brief text="Returns current position of a SceneNode" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="absRelMode" type="Boolean" desc="true to get absolute (world) position, false for relative (to its parent) position" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="X component of current node position" />
                                    <return name="y" type="Numeric" desc="Y component of current node position" />
                                    <return name="z" type="Numeric" desc="Z component of current node position" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la posizione corrente di un nodo della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="absRelMode" type="Boolean" desc="true per leggere la posizione assoluta (world), false per la posizione relativa (al suo nodo padre)" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="componente X della posizione corrente del nodo" />
                                    <return name="y" type="Numeric" desc="componente Y della posizione corrente del nodo" />
                                    <return name="z" type="Numeric" desc="componente Z della posizione corrente del nodo" />
                            </returns>
                    </language>
            </function>
     */
    int nodeGetPosition(lua_State* luaRuntime)
        {
        Ogre::Vector3 pos = Ogre::Vector3::ZERO;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.getPosition", 2))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.getPosition");
            bool absRel = (bool)lua_toboolean(luaRuntime, 2);
            if (theNode != NULL)
                pos = (absRel) ? theNode->_getDerivedPosition() : theNode->getPosition();
            }
        //
        lua_pushnumber(luaRuntime, pos.x);
        lua_pushnumber(luaRuntime, pos.y);
        lua_pushnumber(luaRuntime, pos.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeSetPosition">
                    <language code="en">
                            <brief text="Sets current position of a SceneNode" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="x" type="Numeric" desc="X component of node position to set" />
                                    <param name="y" type="Numeric" desc="Y component of node position to set" />
                                    <param name="z" type="Numeric" desc="Z component of node position to set" />
                                    <param name="absRelMode" type="Boolean" desc="true to set absolute position, false to set relative (to its parent) position" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la posizione corrente di un nodo della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="x" type="Numeric" desc="componente X della posizione da settare al nodo" />
                                    <param name="y" type="Numeric" desc="componente Y della posizione da settare al nodo" />
                                    <param name="z" type="Numeric" desc="componente Z della posizione da settare al nodo" />
                                    <param name="absRelMode" type="Boolean" desc="true per settare la posizione assoluta, false per settare quella relativa (al suo nodo padre)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeSetPosition(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.setPosition", 5))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.setPosition");
            Ogre::Vector3 position = Ogre::Vector3((Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4));
            bool absRel = (bool)lua_toboolean(luaRuntime, 5);
            if (theNode != NULL)
                {
                if (absRel)
                    {
                    Ogre::Vector3 parentPosition = theNode->_getDerivedPosition();
                    theNode->setPosition(parentPosition.x + (Ogre::Real)lua_tonumber(luaRuntime, 2), parentPosition.y + (Ogre::Real)lua_tonumber(luaRuntime, 3), parentPosition.z + (Ogre::Real)lua_tonumber(luaRuntime, 4));
                    }
                else
                    theNode->setPosition(position);
                //
                done = true;
                }
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeTranslate">
                    <language code="en">
                            <brief text="Translates a SceneNode along a vector" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="x" type="Numeric" desc="X component of node position to set" />
                                    <param name="y" type="Numeric" desc="Y component of node position to set" />
                                    <param name="z" type="Numeric" desc="Z component of node position to set" />
                                    <param name="absRelMode" type="Boolean" desc="true to translate along absolute (world) axes, false to translate along local (relative to parent node) axes" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la posizione corrente di un nodo della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="x" type="Numeric" desc="componente X della posizione da settare al nodo" />
                                    <param name="y" type="Numeric" desc="componente Y della posizione da settare al nodo" />
                                    <param name="z" type="Numeric" desc="componente Z della posizione da settare al nodo" />
                                    <param name="absRelMode" type="Boolean" desc="true per traslare lungo gli assi assoluti (world), false per traslare lungo gli assi locali (relativi al nodo parent)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeTranslate(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.translate", 5))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.translate");
            bool absRel = (bool)lua_toboolean(luaRuntime, 5);
            if (theNode != NULL)
                theNode->translate((Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4), (lua_toboolean(luaRuntime, 5) ? Ogre::Node::TS_WORLD : Ogre::Node::TS_LOCAL));
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeSetScale">
                    <language code="en">
                            <brief text="Sets the scale factor of a SceneNode" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="x" type="Numeric" desc="scale factor along X axis" />
                                    <param name="y" type="Numeric" desc="scale factor along Y axis" />
                                    <param name="z" type="Numeric" desc="scale factor along Z axis" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il fattore di scala di un nodo della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="x" type="Numeric" desc="fattore di scala sull'asse X" />
                                    <param name="y" type="Numeric" desc="fattore di scala sull'asse Y" />
                                    <param name="z" type="Numeric" desc="fattore di scala sull'asse Z" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeSetScale(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.setScale", 4))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.setScale");
            if (theNode != NULL)
                {
                theNode->scale((Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4));
                done = true;
                }
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeSetVisible">
                    <language code="en">
                            <brief text="" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="visibilityStatus" type="Boolean" desc="true to make node visible, false to hide it" />
                                    <param name="wholeNodeHierarchy" type="Boolean" desc="true to set the visibility of the entire node hierarchy, false to set only affected node visibility" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="visibilityStatus" type="Boolean" desc="true to make node visible, false to hide it" />
                                    <param name="wholeNodeHierarchy" type="Boolean" desc="true to set the visibility of the entire node hierarchy, false to set only affected node visibility" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeSetVisible(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.setVisible", 3))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.setVisible");
            if (theNode != NULL)
                {
                theNode->setVisible(lua_toboolean(luaRuntime, 2), lua_toboolean(luaRuntime, 3));
                done = true;
                }
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeToggleVisibility">
                    <language code="en">
                            <brief text="" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="wholeNodeHierarchy" type="Boolean" desc="true to flip the visibility of the entire node hierarchy, false to flip only affected node visibility" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="wholeNodeHierarchy" type="Boolean" desc="true to flip the visibility of the entire node hierarchy, false to flip only affected node visibility" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeToggleVisibility(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.toggleVisibility", 2))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.toggleVisibility");
            if (theNode != NULL)
                {
                theNode->flipVisibility(lua_toboolean(luaRuntime, 2));
                done = true;
                }
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeGetOrientation">
                    <language code="en">
                            <brief text="Returns current orientation quaternion of a scene node" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="absRel" type="Boolean" desc="true to get absolute orientation, false to get relative (to its parent) orientation" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="X component of the quaternion" />
                                    <return name="y" type="Numeric" desc="Y component of the quaternion" />
                                    <return name="z" type="Numeric" desc="Z component of the quaternion" />
                                    <return name="w" type="Numeric" desc="W component of the quaternion" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il quaternione dell'orientamento corrente di un nodo della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="absRel" type="Boolean" desc="true per leggere l'orientamento assoluto, false per quello relativo (al nodo padre)" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="componente X del quaternione" />
                                    <return name="y" type="Numeric" desc="componente Y del quaternione" />
                                    <return name="z" type="Numeric" desc="componente Z del quaternione" />
                                    <return name="w" type="Numeric" desc="componente W del quaternione" />
                            </returns>
                    </language>
            </function>
     */
    int nodeGetOrientation(lua_State* luaRuntime)
        {
        Ogre::Quaternion quat = Ogre::Quaternion::IDENTITY;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.getOrientation", 2))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.getOrientation");
            bool absRel = (bool)lua_toboolean(luaRuntime, 2);
            if (theNode != NULL)
                quat = (absRel == true) ? theNode->_getDerivedOrientation() : theNode->getOrientation();
            }
        //
        lua_pushnumber(luaRuntime, quat.x);
        lua_pushnumber(luaRuntime, quat.y);
        lua_pushnumber(luaRuntime, quat.z);
        lua_pushnumber(luaRuntime, quat.w);
        return 4;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeSetOrientation">
                    <language code="en">
                            <brief text="Sets orientation of a scene node" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="x" type="Numeric" desc="X component of the orientation quaternion to set" />
                                    <param name="y" type="Numeric" desc="Y component of the orientation quaternion to set" />
                                    <param name="z" type="Numeric" desc="Z component of the orientation quaternion to set" />
                                    <param name="w" type="Numeric" desc="W component of the orientation quaternion to set" />
                                    <param name="absRel" type="Boolean" desc="true to set absolute orientation, false to set relative (to its parent) orientation" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta l'orientamento di un nodo della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="x" type="Numeric" desc="componente X del quaternione di orientamento da settare" />
                                    <param name="y" type="Numeric" desc="componente Y del quaternione di orientamento da settare" />
                                    <param name="z" type="Numeric" desc="componente Z del quaternione di orientamento da settare" />
                                    <param name="w" type="Numeric" desc="componente W del quaternione di orientamento da settare" />
                                    <param name="absRel" type="Boolean" desc="true per settare l'orientamento assoluto, false per quello relativo (al nodo padre)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeSetOrientation(lua_State* luaRuntime)
        {
        bool done = false;
        Ogre::Quaternion quat = Ogre::Quaternion::IDENTITY;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.setOrientation", 6))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.setOrientation");
            quat.x = (Ogre::Real)lua_tonumber(luaRuntime, 2);
            quat.y = (Ogre::Real)lua_tonumber(luaRuntime, 3);
            quat.z = (Ogre::Real)lua_tonumber(luaRuntime, 4);
            quat.w = (Ogre::Real)lua_tonumber(luaRuntime, 5);
            bool absRel = (bool)lua_toboolean(luaRuntime, 6);
            if (theNode != NULL)
                {
                if (absRel == true)
                    theNode->setOrientation(theNode->getParentSceneNode()->getOrientation() + quat);
                else
                    theNode->setOrientation(quat);
                done = true;
                }
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeResetOrientation">
                    <language code="en">
                            <brief text="Resets scene node orientation to its initial state" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Resetta l'orientamento di un nodo della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeResetOrientation(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.resetOrientation", 1))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.resetOrientation");
            if (theNode != NULL)
                {
                theNode->resetOrientation();
                done = true;
                }
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeRotateByAxisAngle">
                    <language code="en">
                            <brief text="Rotates a scene node by axis and angle" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="axis_x" type="Numeric" desc="X component of the rotation axis" />
                                    <param name="axis_y" type="Numeric" desc="Y component of the rotation axis" />
                                    <param name="axis_z" type="Numeric" desc="Z component of the rotation axis" />
                                    <param name="angle" type="Numeric" desc="rotation angle (in degrees)" />
                                    <param name="absRel" type="Numeric" desc="true for absolute rotation (world), false for relative (local) rotation" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ruota un nodo della scena per asse ed angolo" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="axis_x" type="Numeric" desc="componente X dell'asse di rotazione" />
                                    <param name="axis_y" type="Numeric" desc="componente Y dell'asse di rotazione" />
                                    <param name="axis_z" type="Numeric" desc="componente Z dell'asse di rotazione" />
                                    <param name="angle" type="Numeric" desc="angolo di rotazione (in gradi)" />
                                    <param name="absRel" type="Numeric" desc="true per rotazione assoluta (world), false per rotazione relativa (local)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeRotateByAxisAngle(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.rotateByAxisAngle", 6))
            {
            std::string nodeName = luaL_checkstring(luaRuntime, 1);
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(nodeName.c_str(), "node.rotateByAxisAngle");
            if (theNode != NULL)
                {
                theNode->rotate(Ogre::Vector3((Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4)), Ogre::Radian(Ogre::Degree((Ogre::Real)lua_tonumber(luaRuntime, 5))), (lua_toboolean(luaRuntime, 6) ? Ogre::Node::TS_WORLD : Ogre::Node::TS_LOCAL));
                done = true;
                }
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeRotateByQuaternion">
                    <language code="en">
                            <brief text="Rotates a scene node by quaternion" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="quat_x" type="Numeric" desc="X component of the rotation quaternion" />
                                    <param name="quat_y" type="Numeric" desc="Y component of the rotation quaternion" />
                                    <param name="quat_z" type="Numeric" desc="Z component of the rotation quaternion" />
                                    <param name="quat_w" type="Numeric" desc="W component of the rotation quaternion" />
                                    <param name="absRel" type="Numeric" desc="true for absolute rotation (world), false for relative (local) rotation" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ruota un nodo della scena per quaternione" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="quat_x" type="Numeric" desc="componente X del quaternione di rotazione" />
                                    <param name="quat_y" type="Numeric" desc="componente Y del quaternione di rotazione" />
                                    <param name="quat_z" type="Numeric" desc="componente Z del quaternione di rotazione" />
                                    <param name="quat_w" type="Numeric" desc="componente W del quaternione di rotazione" />
                                    <param name="absRel" type="Numeric" desc="true per rotazione assoluta (world), false per rotazione relativa (local)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeRotateByQuaternion(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.rotateByQuaternion", 6))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.rotateByQuaternion");
            if (theNode != NULL)
                {
                Ogre::Quaternion quat = Ogre::Quaternion::IDENTITY;
                quat.x = (Ogre::Real)lua_tonumber(luaRuntime, 2);
                quat.y = (Ogre::Real)lua_tonumber(luaRuntime, 3);
                quat.z = (Ogre::Real)lua_tonumber(luaRuntime, 4);
                quat.w = (Ogre::Real)lua_tonumber(luaRuntime, 5);
                theNode->rotate(quat, (lua_toboolean(luaRuntime, 6) ? Ogre::Node::TS_WORLD : Ogre::Node::TS_LOCAL));
                done = true;
                }
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeRotateByAngles">
                    <language code="en">
                            <brief text="Rotates a scene node by pitch, yaw and roll" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="pitch" type="Numeric" desc="rotation angle around X axis (pitch) in degrees" />
                                    <param name="yaw" type="Numeric" desc="rotation angle around Y axis (yaw) in degrees" />
                                    <param name="roll" type="Numeric" desc="rotation angle around Z axis (roll) in degrees" />
                                    <param name="absRel" type="Numeric" desc="true for absolute rotation (world), false for relative (local) rotation" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ruota un nodo della scena per pitch, yaw e roll" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="pitch" type="Numeric" desc="rotazione attorno l'asse X (pitch) in gradi" />
                                    <param name="yaw" type="Numeric" desc="rotazione attorno l'asse Y (yaw) in gradi" />
                                    <param name="roll" type="Numeric" desc="rotazione attorno l'asse Z (roll) in gradi" />
                                    <param name="absRel" type="Numeric" desc="true per rotazione assoluta (world), false per rotazione relativa (local)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeRotateByAngles(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.rotateByAngles", 5))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.rotateByAngles");
            if (theNode != NULL)
                {
                Ogre::Real pitch = (Ogre::Real)lua_tonumber(luaRuntime, 2);
                Ogre::Real yaw = (Ogre::Real)lua_tonumber(luaRuntime, 3);
                Ogre::Real roll = (Ogre::Real)lua_tonumber(luaRuntime, 4);
                bool absRel = lua_toboolean(luaRuntime, 5);
                theNode->pitch(Ogre::Radian(Ogre::Degree(pitch)), (absRel) ? Ogre::Node::TS_WORLD : Ogre::Node::TS_LOCAL);
                theNode->yaw(Ogre::Radian(Ogre::Degree(yaw)), (absRel) ? Ogre::Node::TS_WORLD : Ogre::Node::TS_LOCAL);
                theNode->roll(Ogre::Radian(Ogre::Degree(roll)), (absRel) ? Ogre::Node::TS_WORLD : Ogre::Node::TS_LOCAL);
                done = true;
                }
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeSetXAxisDirection">
                    <language code="en">
                            <brief text="Sets the direction of local X axis of a scene node" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="axis_x" type="Numeric" desc="X component of the direction vector to set" />
                                    <param name="axis_y" type="Numeric" desc="Y component of the direction vector to set" />
                                    <param name="axis_z" type="Numeric" desc="Z component of the direction vector to set" />
                                    <param name="absRel" type="Numeric" desc="true for absolute orientation (world), false for relative (local) orientation" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la direzione dell'asse locale X di un nodo della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="axis_x" type="Numeric" desc="componente X del vettore di direzione da settare" />
                                    <param name="axis_y" type="Numeric" desc="componente Y del vettore di direzione da settare" />
                                    <param name="axis_z" type="Numeric" desc="componente Z del vettore di direzione da settare" />
                                    <param name="absRel" type="Numeric" desc="true per orientamento assoluto (world), false per orientamento relativo (local)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeSetXAxisDirection(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.setXAxisDirection", 5))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.setXAxisDirection");
            if (theNode != NULL)
                {
                theNode->setDirection((Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4), (lua_toboolean(luaRuntime, 5) ? Ogre::Node::TS_WORLD : Ogre::Node::TS_LOCAL), Ogre::Vector3::UNIT_X);
                done = true;
                }
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeSetYAxisDirection">
                    <language code="en">
                            <brief text="Sets the direction of local Y axis of a scene node" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="axis_x" type="Numeric" desc="X component of the direction vector to set" />
                                    <param name="axis_y" type="Numeric" desc="Y component of the direction vector to set" />
                                    <param name="axis_z" type="Numeric" desc="Z component of the direction vector to set" />
                                    <param name="absRel" type="Numeric" desc="true for absolute orientation (world), false for relative (local) orientation" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la direzione dell'asse locale X di un nodo della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="axis_x" type="Numeric" desc="componente X del vettore di direzione da settare" />
                                    <param name="axis_y" type="Numeric" desc="componente Y del vettore di direzione da settare" />
                                    <param name="axis_z" type="Numeric" desc="componente Z del vettore di direzione da settare" />
                                    <param name="absRel" type="Numeric" desc="true per orientamento assoluto (world), false per orientamento relativo (local)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeSetYAxisDirection(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.setYAxisDirection", 5))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.setYAxisDirection");
            if (theNode != NULL)
                {
                theNode->setDirection((Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4), (lua_toboolean(luaRuntime, 5) ? Ogre::Node::TS_WORLD : Ogre::Node::TS_LOCAL), Ogre::Vector3::UNIT_Y);
                done = true;
                }
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeSetZAxisDirection">
                    <language code="en">
                            <brief text="Sets the direction of local Z axis of a scene node" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="axis_x" type="Numeric" desc="X component of the direction vector to set" />
                                    <param name="axis_y" type="Numeric" desc="Y component of the direction vector to set" />
                                    <param name="axis_z" type="Numeric" desc="Z component of the direction vector to set" />
                                    <param name="absRel" type="Numeric" desc="true for absolute orientation (world), false for relative (local) orientation" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la direzione dell'asse locale Z di un nodo della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="axis_x" type="Numeric" desc="componente X del vettore di direzione da settare" />
                                    <param name="axis_y" type="Numeric" desc="componente Y del vettore di direzione da settare" />
                                    <param name="axis_z" type="Numeric" desc="componente Z del vettore di direzione da settare" />
                                    <param name="absRel" type="Numeric" desc="true per orientamento assoluto (world), false per orientamento relativo (local)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int nodeSetZAxisDirection(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.setZAxisDirection", 5))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.setZAxisDirection");
            if (theNode != NULL)
                {
                theNode->setDirection((Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4), (lua_toboolean(luaRuntime, 5) ? Ogre::Node::TS_WORLD : Ogre::Node::TS_LOCAL), Ogre::Vector3::UNIT_Z);
                done = true;
                }
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeGetXAxisDirection">
                    <language code="en">
                            <brief text="Gets the direction of local X axis of a scene node" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="absRel" type="Numeric" desc="true to get absolute direction (world), false to get relative (to parent) direction" />
                            </params>
                            <returns>
                                    <return name="axis_x" type="Numeric" desc="X component of current axis direction" />
                                    <return name="axis_y" type="Numeric" desc="Y component of current axis direction" />
                                    <return name="axis_z" type="Numeric" desc="Z component of current axis direction" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la direzione corrente dell'asse locale X di un nodo della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="absRel" type="Numeric" desc="true per leggere la direzione assoluta (world), false per quella relativa (al nodo padre)" />
                            </params>
                            <returns>
                                    <return name="axis_x" type="Numeric" desc="componente X della direzione corrente dell'asse" />
                                    <return name="axis_y" type="Numeric" desc="componente Y della direzione corrente dell'asse" />
                                    <return name="axis_z" type="Numeric" desc="componente Z della direzione corrente dell'asse" />
                            </returns>
                    </language>
            </function>
     */
    int nodeGetXAxisDirection(lua_State* luaRuntime)
        {
        Ogre::Vector3 direction = Ogre::Vector3::ZERO;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.getXAxisDirection", 2))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.getXAxisDirection");
            bool absRel = (bool)lua_toboolean(luaRuntime, 2);
            if (theNode != NULL)
                direction = (absRel == true) ? theNode->_getDerivedOrientation().xAxis() : theNode->getOrientation().xAxis();
            }
        //
        lua_pushnumber(luaRuntime, direction.x);
        lua_pushnumber(luaRuntime, direction.y);
        lua_pushnumber(luaRuntime, direction.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeGetYAxisDirection">
                    <language code="en">
                            <brief text="Gets the direction of local Y axis of a scene node" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="absRel" type="Numeric" desc="true to get absolute direction (world), false to get relative (to parent) direction" />
                            </params>
                            <returns>
                                    <return name="axis_x" type="Numeric" desc="X component of current axis direction" />
                                    <return name="axis_y" type="Numeric" desc="Y component of current axis direction" />
                                    <return name="axis_z" type="Numeric" desc="Z component of current axis direction" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la direzione corrente dell'asse locale Y di un nodo della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="absRel" type="Numeric" desc="true per leggere la direzione assoluta (world), false per quella relativa (al nodo padre)" />
                            </params>
                            <returns>
                                    <return name="axis_x" type="Numeric" desc="componente X della direzione corrente dell'asse" />
                                    <return name="axis_y" type="Numeric" desc="componente Y della direzione corrente dell'asse" />
                                    <return name="axis_z" type="Numeric" desc="componente Z della direzione corrente dell'asse" />
                            </returns>
                    </language>
            </function>
     */
    int nodeGetYAxisDirection(lua_State* luaRuntime)
        {
        Ogre::Vector3 direction = Ogre::Vector3::ZERO;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.getYAxisDirection", 2))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.getYAxisDirection");
            bool absRel = (bool)lua_toboolean(luaRuntime, 2);
            if (theNode != NULL)
                direction = (absRel == true) ? theNode->_getDerivedOrientation().yAxis() : theNode->getOrientation().yAxis();
            }
        //
        lua_pushnumber(luaRuntime, direction.x);
        lua_pushnumber(luaRuntime, direction.y);
        lua_pushnumber(luaRuntime, direction.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeGetZAxisDirection">
                    <language code="en">
                            <brief text="Gets the direction of local Z axis of a scene node" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="absRel" type="Boolean" desc="true to get absolute direction (world), false to get relative (to parent) direction" />
                            </params>
                            <returns>
                                    <return name="axis_x" type="Numeric" desc="X component of current axis direction" />
                                    <return name="axis_y" type="Numeric" desc="Y component of current axis direction" />
                                    <return name="axis_z" type="Numeric" desc="Z component of current axis direction" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la direzione corrente dell'asse locale Z di un nodo della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="absRel" type="Boolean" desc="true per leggere la direzione assoluta (world), false per quella relativa (al nodo padre)" />
                            </params>
                            <returns>
                                    <return name="axis_x" type="Numeric" desc="componente X della direzione corrente dell'asse" />
                                    <return name="axis_y" type="Numeric" desc="componente Y della direzione corrente dell'asse" />
                                    <return name="axis_z" type="Numeric" desc="componente Z della direzione corrente dell'asse" />
                            </returns>
                    </language>
            </function>
     */
    int nodeGetZAxisDirection(lua_State* luaRuntime)
        {
        Ogre::Vector3 direction = Ogre::Vector3::ZERO;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.getZAxisDirection", 2))
            {
            Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.getZAxisDirection");
            bool absRel = (bool)lua_toboolean(luaRuntime, 2);
            if (theNode != NULL)
                direction = (absRel == true) ? theNode->_getDerivedOrientation().zAxis() : theNode->getOrientation().zAxis();
            }
        //
        lua_pushnumber(luaRuntime, direction.x);
        lua_pushnumber(luaRuntime, direction.y);
        lua_pushnumber(luaRuntime, direction.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeGetVectorToNode">
                    <language code="en">
                            <brief text="Returns a vector between two node positions" />
                            <description text="" />
                            <params>
                                    <param name="nodeNameOne" type="String" desc="name of first scene node" />
                                    <param name="nodeNameTwo" type="String" desc="name of second scene node" />
                                    <param name="absRel" type="Boolean" desc="true to get the vector between absolute positions (world), or false to get the vector between relative (to parent) positions" />
                            </params>
                            <returns>
                                    <return name="vector_x" type="Numeric" desc="X component of resulting vector" />
                                    <return name="vector_y" type="Numeric" desc="Y component of resulting vector" />
                                    <return name="vector_z" type="Numeric" desc="Z component of resulting vector" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna un vettore compreso tra le posizioni di 2 nodi della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeNameOne" type="String" desc="nome del primo nodo della scena" />
                                    <param name="nodeNameTwo" type="String" desc="nome del secondo nodo della scena" />
                                    <param name="absRel" type="Boolean" desc="true per avere il vettore tra le posizioni assolute (world) dei nodi, false per avere il vettore tra le posizioni relative (al nodo parent) dei nodi" />
                            </params>
                            <returns>
                                    <return name="axis_x" type="Numeric" desc="componente X del vettore risultante" />
                                    <return name="axis_y" type="Numeric" desc="componente Y del vettore risultante" />
                                    <return name="axis_z" type="Numeric" desc="componente Z del vettore risultante" />
                            </returns>
                    </language>
            </function>
     */
    int nodeGetVectorToNode(lua_State* luaRuntime)
        {
        Ogre::Vector3 direction = Ogre::Vector3::ZERO;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.getVectorToNode", 3))
            {
            Ogre::SceneNode* nodeA = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.getVectorToNode");
            Ogre::SceneNode* nodeB = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 2), "node.getVectorToNode");
            bool absRel = (bool)lua_toboolean(luaRuntime, 3);
            if (nodeA != NULL && nodeB != NULL)
                direction = (absRel ? nodeB->_getDerivedPosition() : nodeB->getPosition()) - (absRel ? nodeA->_getDerivedPosition() : nodeA->getPosition()); // B-A = A->B
            }
        //
        lua_pushnumber(luaRuntime, direction.x);
        lua_pushnumber(luaRuntime, direction.y);
        lua_pushnumber(luaRuntime, direction.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeGetVectorToPoint">
                    <language code="en">
                            <brief text="Returns a vector between a node position and an absolute point in the space" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="point_x" type="Numeric" desc="X component of the point" />
                                    <param name="point_y" type="Numeric" desc="Y component of the point" />
                                    <param name="point_z" type="Numeric" desc="Z component of the point" />
                                    <param name="absRel" type="Boolean" desc="true to use the absolute position of the node (world), false to use its realtive (to parent node) position" />
                            </params>
                            <returns>
                                    <return name="vector_x" type="Numeric" desc="X component of resulting vector" />
                                    <return name="vector_y" type="Numeric" desc="Y component of resulting vector" />
                                    <return name="vector_z" type="Numeric" desc="Z component of resulting vector" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna un vettore compreso tra la posizione di un nodo e un punto assoluto nello spazio" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome del nodo della scena" />
                                    <param name="point_x" type="Numeric" desc="componente X del punto" />
                                    <param name="point_y" type="Numeric" desc="componente y del punto" />
                                    <param name="point_z" type="Numeric" desc="componente z del punto" />
                                    <param name="absRel" type="Boolean" desc="true per usare la posizione assoluta (world) del nodo, false per usare quella relativa (al nodo parent)" />
                            </params>
                            <returns>
                                    <return name="axis_x" type="Numeric" desc="componente X del vettore risultante" />
                                    <return name="axis_y" type="Numeric" desc="componente Y del vettore risultante" />
                                    <return name="axis_z" type="Numeric" desc="componente Z del vettore risultante" />
                            </returns>
                    </language>
            </function>
     */
    int nodeGetVectorToPoint(lua_State* luaRuntime)
        {
        Ogre::Vector3 direction = Ogre::Vector3::ZERO;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.getVectorToPoint", 5))
            {
            Ogre::SceneNode* startNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.getVectorToPoint");
            if (startNode != NULL)
                {
                Ogre::Vector3 destination = Ogre::Vector3(luaL_checknumber(luaRuntime, 2), luaL_checknumber(luaRuntime, 3), luaL_checknumber(luaRuntime, 4)); // mDestination is the next location
                bool absRel = (bool)lua_toboolean(luaRuntime, 5);
                direction = destination - (absRel ? startNode->_getDerivedPosition() : startNode->getPosition()); // B-A = A->B
                }
            }
        //
        lua_pushnumber(luaRuntime, direction.x);
        lua_pushnumber(luaRuntime, direction.y);
        lua_pushnumber(luaRuntime, direction.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeGetDistanceToNode">
                    <language code="en">
                            <brief text="Returns the distance between two scene nodes" />
                            <description text="" />
                            <params>
                                    <param name="nodeNameOne" type="String" desc="name of first scene node" />
                                    <param name="nodeNameTwo" type="String" desc="name of second scene node" />
                                    <param name="absRel" type="Boolean" desc="true to get the distance between absolute positions (world), or false to get the distance between relative (to parent nodes) positions" />
                            </params>
                            <returns>
                                    <return name="distance" type="Numeric" desc="distance between nodes" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la distanza presente tra due nodi della scena" />
                            <description text="" />
                            <params>
                                    <param name="nodeNameOne" type="String" desc="nome del primo nodo della scena" />
                                    <param name="nodeNameTwo" type="String" desc="nome del secondo nodo della scena" />
                                    <param name="absRel" type="Boolean" desc="true per avere la distanza tra le posizioni assolute (world) dei nodi, false per avere la distanza tra le posizioni relative (ai nodi parent) dei nodi" />
                            </params>
                            <returns>
                                    <return name="distance" type="Numeric" desc="distanza presente tra i nodi" />
                            </returns>
                    </language>
            </function>
     */
    int nodeGetDistanceToNode(lua_State* luaRuntime)
        {
        Ogre::Real distance = -1;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.getDistanceToNode", 3))
            {
            Ogre::SceneNode* firstNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 1), "node.getDistanceToNode");
            Ogre::SceneNode* secondNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 2), "node.getDistanceToNode");
            if (firstNode != NULL && secondNode != NULL)
                {
                bool absRel = (bool)lua_toboolean(luaRuntime, 3);
                distance = ((absRel ? secondNode->_getDerivedPosition() : secondNode->getPosition()) - (absRel ? firstNode->_getDerivedPosition() : firstNode->getPosition())).length();
                }
            }
        //
        lua_pushnumber(luaRuntime, distance);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="nodeSetAutoUpdateFromLuaTable">
                    <language code="en">
                            <brief text="Tells to the module to auto update a node fetching position/orientation values from a lua table" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="name of the scene node" />
                                    <param name="luaTableName" type="String" desc="name of the table where values are stored" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Informa il modulo di autoaggionare un nodo prelevando i valori di posizione/orientamento da una table lua" />
                            <description text="" />
                            <params>
                                    <param name="nodeName" type="String" desc="nome dello scene node" />
                                    <param name="luaTableName" type="String" desc="nome della table lua dalla quale prelevare i valori" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int nodeBindToExternalObject(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "node.bindToExternalObject", 2))
            {
            done = EOSG3D::getInstance()->bindNodeToEOSObject(luaL_checkstring(luaRuntime, 1), (EOSObject*) lua_touserdata(luaRuntime, 2));

            }
        //
        return 0;
        }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="entityLoad">
                    <language code="en">
                            <brief text="Loads an object with an Entity (a mesh)" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="name to give to the new object" />
                                    <param name="objectMeshName" type="String" desc="resource name of the new object mesh" />
                            </params>
                            <returns>
                                    <return name="sceneNodeName" type="String" desc="name of the scene node where object is attached, nil in case of errors" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Carica un oggetto con un Entity (una mesh)" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="nome da dare al nuovo oggetto" />
                                    <param name="objectMeshName" type="String" desc="nome risorsa della mesh da caricare nel nuovo oggetto" />
                            </params>
                            <returns>
                                    <return name="sceneNodeName" type="String" desc="nome del nodo della scena dove e' stato legato il nuovo oggetto, nil in caso di errore" />
                            </returns>
                    </language>
            </function>
     */
    int entityLoad(lua_State* luaRuntime)
        {
        const char* sceneNodeName = NULL;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "entity.load", 2))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            const char* meshFileName = luaL_checkstring(luaRuntime, 2);
            //
            Ogre::SceneNode* sceneNode = EOSG3D::getInstance()->objectLoad(objectName, meshFileName, NULL);
            if (sceneNode != NULL) sceneNodeName = sceneNode->getName().c_str();
            }
        lua_pushstring(luaRuntime, sceneNodeName);
        return 1;
        }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="particleSystemLoad">
                    <language code="en">
                            <brief text="Loads an object with a ParticleSystem" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="name to give to the new object" />
                                    <param name="objectMeshName" type="String" desc="resource name of the particle system material" />
                            </params>
                            <returns>
                                    <return name="sceneNodeName" type="String" desc="name of the scene node where object is attached, nil in case of errors" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Carica un oggetto con un ParticleSystem" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="nome da dare al nuovo oggetto" />
                                    <param name="objectMeshName" type="String" desc="nome risorsa del materiale del particle system" />
                            </params>
                            <returns>
                                    <return name="sceneNodeName" type="String" desc="nome del nodo della scena dove e' stato legato il nuovo oggetto, nil in caso di errore" />
                            </returns>
                    </language>
            </function>
     */
    int particleSystemLoad(lua_State* luaRuntime)
        {
        const char* sceneNodeName = NULL;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "particlesystem.load", 2))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            const char* particleMaterialName = luaL_checkstring(luaRuntime, 2);
            //
            Ogre::SceneNode* sceneNode = EOSG3D::getInstance()->particleSystemLoad(objectName, particleMaterialName, NULL);
            if (sceneNode != NULL) sceneNodeName = sceneNode->getName().c_str();
            }
        lua_pushstring(luaRuntime, sceneNodeName);
        return 1;
        }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="collisionBoxCreate">
                    <language code="en">
                            <brief text="" />
                            <description text="" />
                            <params>
                                    <param name="collisionObjectName" type="String" desc="name to give to the new collision object" />
                                    <param name="xSize" type="Number" desc="x size of the box to create" />
                                    <param name="ySize" type="Number" desc="x size of the box to create" />
                                    <param name="zSize" type="Number" desc="x size of the box to create" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="" />
                            <description text="" />
                            <params>
                                    <param name="collisionObjectName" type="String" desc="nome da dare al nuovo oggetto di collisione" />
                                    <param name="entityObjectName" type="String" desc="nome dell'oggetto entity da incapsulare" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int collisionBoxCreate(lua_State* luaRuntime)
            {
            const char* sceneNodeName = NULL;
            if(checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "collision.createBox", 4))
                    {
                    const char* objectName = luaL_checkstring(luaRuntime, 1);
                    if(EOSG3D::getInstance()->objectByName(objectName, NULL) == NULL) // checks if object exists
                            {
                            EOSOgreObject* newOgreObject = EOSG3D::getInstance()->createEmptyObject(objectName);
                            if(newOgreObject != NULL)
                                    {
                                    //newOgreObject->requestSceneNode(NULL);
                                    newOgreObject->cachedSceneNode = EOSG3D::getInstance()->requestCachedSceneNode(NULL);
                                    if(newOgreObject->cachedSceneNode != NULL)
                                            {
                                            newOgreObject->attachCollisionObject( EOSG3D::getInstance()->getCollisionWorld()->createBoxGeometry(lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4)));
                                            sceneNodeName = newOgreObject->cachedSceneNode->sceneNode->getName().c_str();
                                            }
                                    }
                            }
                    }
            lua_pushstring(luaRuntime, sceneNodeName);
            return 1;
            }

    // -----------------------------------------------------------------------------

    /**
            <function name="collisionSphereCreate">
                    <language code="en">
                            <brief text="" />
                            <description text="" />
                            <params>
                                    <param name="collisionObjectName" type="String" desc="name to give to the new collision object" />
                                    <param name="radius" type="Number" desc="x size of the box to create" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la dimensione massima dell'Axis Aligned Box di un oggetto" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int collisionSphereCreate(lua_State* luaRuntime)
            {
            const char* sceneNodeName = NULL;
            if(checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "collision.createSphere", 2))
                    {
                    const char* objectName = luaL_checkstring(luaRuntime, 1);
                    if(EOSG3D::getInstance()->objectByName(objectName, NULL) == NULL) // checks if object exists
                            {
                            EOSOgreObject* newOgreObject = EOSG3D::getInstance()->createEmptyObject(objectName);
                            if(newOgreObject != NULL)
                                    {
                                    //newOgreObject->requestSceneNode(NULL);
                                    newOgreObject->cachedSceneNode = EOSG3D::getInstance()->requestCachedSceneNode(NULL);
                                    if(newOgreObject->cachedSceneNode != NULL)
                                            {
                                            newOgreObject->attachCollisionObject( EOSG3D::getInstance()->getCollisionWorld()->createSphereGeometry(lua_tonumber(luaRuntime, 2)));
                                            sceneNodeName = newOgreObject->cachedSceneNode->sceneNode->getName().c_str();
                                            }
                                    }
                            }
                    }
            lua_pushstring(luaRuntime, sceneNodeName);
            return 1;
            }

    // -----------------------------------------------------------------------------

    /**
            <function name="collisionRayCreate">
                    <language code="en">
                            <brief text="" />
                            <description text="" />
                            <params>
                                    <param name="collisionObjectName" type="String" desc="name to give to the new collision object" />
                                    <param name="radius" type="Number" desc="length of the ray to create" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int collisionRayCreate(lua_State* luaRuntime)
            {
            const char* sceneNodeName = NULL;
            if(checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "collision.createRay", 2))
                    {
                    const char* objectName = luaL_checkstring(luaRuntime, 1);
                    if(EOSG3D::getInstance()->objectByName(objectName, NULL) == NULL) // checks if object exists
                            {
                            EOSOgreObject* newOgreObject = EOSG3D::getInstance()->createEmptyObject(objectName);
                            if(newOgreObject != NULL)
                                    {
                                    //newOgreObject->requestSceneNode(NULL);
                                    newOgreObject->cachedSceneNode = EOSG3D::getInstance()->requestCachedSceneNode(NULL);
                                    if(newOgreObject->cachedSceneNode != NULL)
                                            {
                                            newOgreObject->attachCollisionObject( EOSG3D::getInstance()->getCollisionWorld()->createRayGeometry(lua_tonumber(luaRuntime, 2)));
                                            sceneNodeName = newOgreObject->cachedSceneNode->sceneNode->getName().c_str();
                                            }
                                    }
                            }
                    }
            lua_pushstring(luaRuntime, sceneNodeName);
            return 1;
            }

    // -----------------------------------------------------------------------------

    /**
            <function name="collisionSphereToEntity">
                    <language code="en">
                            <brief text="" />
                            <description text="" />
                            <params>
                                    <param name="collisionObjectName" type="String" desc="name to give to the new collision object" />
                                    <param name="entityObjectName" type="String" desc="name of the entity object to wrap" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la dimensione massima dell'Axis Aligned Box di un oggetto" />
                            <description text="" />
                            <params>
                                    <param name="collisionObjectName" type="String" desc="nome da dare al nuovo oggetto di collisione" />
                                    <param name="entityObjectName" type="String" desc="nome dell'oggetto entity da incapsulare" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int collisionSphereToEntity(lua_State* luaRuntime)
            {
            Ogre::Vector3 size = Ogre::Vector3::ZERO;
            if(checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "collision.sphereToEntity", 2))
                    {
                    EOSOgreObject* ogreObject = EOSG3D::getInstance()->objectByName(luaL_checkstring(luaRuntime, 1), "collision.sphereToEntity");
                    if(ogreObject != NULL)
                            {
                            int collisionShapeType = (int)lua_tonumber(luaRuntime, 2);
                            //
                            if(ogreObject->getType() == EOSOgreObject::EntityObjectType)
                                    {
                                    //
                                    switch(collisionShapeType)
                                            {
                                            case COLLISIONSHAPE_SPHERE:
                                                    printf(" *** adding SPHERE collision shape\n");
                                                    //
                                                    ogreObject->attachCollisionObject( EOSG3D::getInstance()->getCollisionWorld()->createSphereGeometryFromEntity(ogreObject));
                                                    //
                                                    break;
                                            case COLLISIONSHAPE_BOX:
                                                    break;
                                            case COLLISIONSHAPE_MESH:
                                                    break;
                                            }
                                    }
                            else
                                    {
                                    LOGERROR("[eos."_MODULE_NAME"][object.addCollisionShape] - Sorry, but only Entity objects can have collision shapes.");
                                    }
                            }
                    }
            lua_pushnumber(luaRuntime, size.x);
            lua_pushnumber(luaRuntime, size.y);
            lua_pushnumber(luaRuntime, size.z);
            return 3;
            }

    // -----------------------------------------------------------------------------

    /**
            <function name="collisionTrimeshFromEntity">
                    <language code="en">
                            <brief text="" />
                            <description text="" />
                            <params>
                                    <param name="collisionObjectName" type="String" desc="name to give to the new collision object" />
                                    <param name="entityMeshFilename" type="String" desc="filename of the mesh to use (i.e.: 'terrain.mesh')" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int collisionTrimeshFromEntity(lua_State* luaRuntime)
            {
            const char* sceneNodeName = NULL;
            if(checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "collision.trimeshFromEntity", 2))
                    {
                    const char* objectName = luaL_checkstring(luaRuntime, 1);
                    const char* meshFilename = luaL_checkstring(luaRuntime, 2);
                    if(EOSG3D::getInstance()->objectByName(objectName, NULL) == NULL) // checks if object exists
                            {
                            EOSOgreObject* newOgreObject = EOSG3D::getInstance()->createEmptyObject(objectName);
                            if(newOgreObject != NULL)
                                    {
                                    newOgreObject->cachedSceneNode = EOSG3D::getInstance()->requestCachedSceneNode(NULL);
                                    if(newOgreObject->cachedSceneNode != NULL)
                                            {
                                            Ogre::Entity* trimeshEntity = EOSG3D::getInstance()->getSceneManager()->createEntity("TEMPORARY_TRIMESH_ENTITY", meshFilename);
                                            newOgreObject->attachCollisionObject( EOSG3D::getInstance()->getCollisionWorld()->createTrimeshGeometryFromEntity(trimeshEntity->getMesh()) );
                                            sceneNodeName = newOgreObject->cachedSceneNode->sceneNode->getName().c_str();
                                            EOSG3D::getInstance()->getSceneManager()->destroyEntity(trimeshEntity);
                                            }
                                    }
                            }
                    }
            lua_pushstring(luaRuntime, sceneNodeName);
            return 1;
            }

    // -----------------------------------------------------------------------------

    /**
            <function name="collisionObjectSetActive">
                    <language code="en">
                            <brief text="Sets activity of a collision object" />
                            <description text="" />
                            <params>
                                    <param name="collisionObjectName" type="String" desc="name of the collision object" />
                                    <param name="status" type="Boolean" desc="activity status to set to the object" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta lo stato di attivita' di un collision object" />
                            <description text="" />
                            <params>
                                    <param name="collisionObjectName" type="String" desc="nome del collision object" />
                                    <param name="status" type="Boolean" desc="stato di attivita' da settare all'oggetto" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int collisionObjectSetActive(lua_State* luaRuntime)
            {
            bool done = false;
            if(checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "collision.objectSetActive", 2))
                    {
                    const char* objectName = luaL_checkstring(luaRuntime, 1);
                    EOSOgreObject* object = EOSG3D::getInstance()->objectByName(objectName, "collision.objectSetActive");
                    if(object != NULL && object->getType() == EOSOgreObject::CollisionObjectType)
                            {
                            object->getCollisionObject()->setActive(lua_toboolean(luaRuntime, 2));
                            done = true;
                            }
                    else
                            {
                            LOGWARNING("[eos."_MODULE_NAME"][collision.objectSetActive()] - Warning: object %s is not a Collision Object.", objectName);
                            }
                    }
            lua_pushboolean(luaRuntime, done);
            return 1;
            }

    // -----------------------------------------------------------------------------

    /**
            <function name="collisionObjectIsActive">
                    <language code="en">
                            <brief text="Returns the activity status of a collision object" />
                            <description text="" />
                            <params>
                                    <param name="collisionObjectName" type="String" desc="name of the collision object" />
                            </params>
                            <returns>
                                    <return name="status" type="Boolean" desc="activity status of the object" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna lo stato di attivita' di un collision object" />
                            <description text="" />
                            <params>
                                    <param name="collisionObjectName" type="String" desc="nome del collision object" />
                            </params>
                            <returns>
                                    <return name="status" type="Boolean" desc="activity status of the object" />
                            </returns>
                    </language>
            </function>
     */
    int collisionObjectIsActive(lua_State* luaRuntime)
            {
            bool active = false;
            if(checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "collision.objectIsActive", 1))
                    {
                    const char* objectName = luaL_checkstring(luaRuntime, 1);
                    EOSOgreObject* object = EOSG3D::getInstance()->objectByName(objectName, "collision.objectIsActive");
                    if(object != NULL && object->getType() == EOSOgreObject::CollisionObjectType)
                            active = object->getCollisionObject()->isActive();
                    else
                            LOGWARNING("[eos."_MODULE_NAME"][collision.objectIsActive()] - Warning: object %s is not a Collision Object.", objectName);
                    }
            lua_pushboolean(luaRuntime, active);
            return 1;
            }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="objectExists">
                    <language code="en">
                            <brief text="Checks if an object with a given name exists" />
                            <description text="(An object can either be an Entity, a ParticleSystem, or a BillboardSet)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                            </params>
                            <returns>
                                    <return name="existence" type="Boolean" desc="true if object exists, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Controlla l'esistenza di un determinato oggetto" />
                            <description text="(Un oggetto puo' essere sia un Entity, che un ParticleSystem, o un BillboardSet)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                            </params>
                            <returns>
                                    <return name="existence" type="Boolean" desc="true se l'oggetto esiste, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int objectExists(lua_State* luaRuntime)
        {
        bool result = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.exists", 1))
            result = (EOSG3D::getInstance()->objectByName(luaL_checkstring(luaRuntime, 1), "object.exists") != NULL);
        lua_pushboolean(luaRuntime, result);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectRemove">
                    <language code="en">
                            <brief text="Completely removes an object from the scene" />
                            <description text="(An object can be an Entity, a ParticleSystem, a BillboardSet, a Ribbontrail, etc)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Rimuove completamente un oggetto dalla scena (incluso lo SceneNode associato)" />
                            <description text="(Un oggetto puo' essere sia un Entity, un ParticleSystem, un BillboardSet, un RibbonTrail, ecc...)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int objectRemove(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.remove", 1))
            done = EOSG3D::getInstance()->removeObject(luaL_checkstring(luaRuntime, 1));
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectSetCastShadow">
                    <language code="en">
                            <brief text="Sets the 'CastShadow' property of an object" />
                            <description text="(An object can either be an Entity, a ParticleSystem, or a BillboardSet)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="castShadow" type="Boolean" desc="true to make object casting shadow, false to disable it" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la proprieta' 'CastShadow' di un oggetto" />
                            <description text="(Un oggetto puo' essere sia un Entity, che un ParticleSystem, o un BillboardSet)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="castShadow" type="Boolean" desc="true per fare in modo che l'oggetto getti ombra, false per disabilitarla" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int objectSetCastShadow(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.setCastShadow", 2))
            done = EOSG3D::getInstance()->setObjectCastsShadow(luaL_checkstring(luaRuntime, 1), lua_toboolean(luaRuntime, 2));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectSetVisible">
                    <language code="en">
                            <brief text="Sets the visibility status of an object" />
                            <description text="(An object can either be an Entity, a ParticleSystem, or a BillboardSet)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="visible" type="Boolean" desc="true to make object visible, false to make it invisible" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la visibilita' di un oggetto" />
                            <description text="(Un oggetto puo' essere sia un Entity, che un ParticleSystem, o un BillboardSet)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="visible" type="Boolean" desc="true per rendere l'oggetto visibile, false per renderlo invisibile" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int objectSetVisible(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.setVisible", 2))
            {
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(luaL_checkstring(luaRuntime, 1), "object.setVisible");
            if (object != NULL)
                {
                object->setVisible(lua_toboolean(luaRuntime, 2));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectIsVisible">
                    <language code="en">
                            <brief text="Returns the visibility status of an object" />
                            <description text="(An object can either be an Entity, a ParticleSystem, or a BillboardSet)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                            </params>
                            <returns>
                                    <return name="visible" type="Boolean" desc="true if the object is visible, false if it isn't" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna lo stato di visibilita' di un oggetto" />
                            <description text="(Un oggetto puo' essere sia un Entity, che un ParticleSystem, o un BillboardSet)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                            </params>
                            <returns>
                                    <return name="visible" type="Boolean" desc="true se l'oggetto e' visibile, false se non lo e'"/>
                            </returns>
                    </language>
            </function>
     */
    int objectIsVisible(lua_State* luaRuntime)
        {
        bool visible = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.isVisible", 1))
            {
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(luaL_checkstring(luaRuntime, 1), "object.isVisible");
            if (object != NULL)
                {
                switch (object->getType())
                    {
                    case EOSOgreObject::EntityObjectType:
                        visible = object->getEntity()->isVisible();
                        break;
                    case EOSOgreObject::BillboardSetObjectType:
                        object->getBillboardSet()->isVisible();
                        break;
                    }
                }
            }
        lua_pushboolean(luaRuntime, visible);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectGetNodeName">
                    <language code="en">
                            <brief text="Returns the scene node name where the object is attached" />
                            <description text="(An object can either be an Entity, a ParticleSystem, or a BillboardSet)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                            </params>
                            <returns>
                                    <return name="nodeName" type="String" desc="name of the scene node where object is attached, nil in case of errors" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il nome del nodo della scena al quale e' associato l'oggetto" />
                            <description text="(Un oggetto puo' essere sia un Entity, che un ParticleSystem, o un BillboardSet)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                            </params>
                            <returns>
                                    <return name="nodeName" type="String" desc="nome del nodo della scena al quale l'oggetto e' associato, nil in caso di errore" />
                            </returns>
                    </language>
            </function>
     */
    int objectGetNodeName(lua_State* luaRuntime)
        {
        const char* sceneNodeName = NULL;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.getNodeName", 1))
            {
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(luaL_checkstring(luaRuntime, 1), "object.getNodeName");
            if (object != NULL)
                sceneNodeName = object->cachedSceneNode->sceneNode->getName().c_str();
            }
        lua_pushstring(luaRuntime, sceneNodeName);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectAnimationEnable">
                    <language code="en">
                            <brief text="Enables a skeletal animation on an Entity object" />
                            <description text="(this function is valid for Entity type objects only)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="animationName" type="String" desc="name of the animation to set (the animation must exists in the entity model)" />
                                    <param name="loop" type="Boolean" desc="looping flag of the animation (true: loop, false: single)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Avvia un'animazione scheletrale su un oggetto Entity" />
                            <description text="(questa funzione e' valida solo per oggetti di tipo Entity)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="animationName" type="String" desc="nome dell'animazione da attivare (l'animazione deve essere esistente nel modello dell'entity)" />
                                    <param name="loop" type="Boolean" desc="flag per attivare l'animazione in loop (true: loop, false: singola)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int objectAnimationEnable(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.animation.enable", 3))
            {
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(luaL_checkstring(luaRuntime, 1), "object.animation.enable");
            const char* animationName = luaL_checkstring(luaRuntime, 2);
            if (object != NULL && object->getType() == EOSOgreObject::EntityObjectType)
                {
                try
                    {
                    EntityAnimation* entityAnim = object->animByName(animationName);
                    if (entityAnim == NULL)
                        {
                        entityAnim = object->addEntityAnim(animationName, object->getEntity()->getAnimationState(Ogre::String(animationName)));
                        }
                    entityAnim->loop = lua_toboolean(luaRuntime, 3);
                    entityAnim->enable();
                    //
                    done = true;
                    }
                catch (...)
                    {
                    EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.enable] - Animation '%s' not found!", animationName);
                    }
                //
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectAnimationDisable">
                    <language code="en">
                            <brief text="Disables a skeletal animation on an Entity object" />
                            <description text="(this function is valid for Entity type objects only)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="animationName" type="String" desc="name of the animation to disable (the animation must be enabled, first)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Disattiva un'animazione scheletrale su un oggetto Entity" />
                            <description text="(questa funzione e' valida solo per oggetti di tipo Entity)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="animationName" type="String" desc="nome dell'animazione da disattivare (l'animazione deve essere stata abilitata, prima)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int objectAnimationDisable(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.animation.disable", 2))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(objectName, "object.animation.disable");
            const char* animationName = luaL_checkstring(luaRuntime, 2);
            if (object != NULL && object->getType() == EOSOgreObject::EntityObjectType)
                {
                EntityAnimation* entityAnim = object->animByName(animationName);
                if (entityAnim != NULL)
                    {
                    entityAnim->disable();
                    //
                    done = true;
                    }
                else
                    EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.disable] - Object '%s' doesn't have animation named '%s'. Try to enable it, first.", objectName, animationName);
                //
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectAnimationSetPosition">
                    <language code="en">
                            <brief text="Sets current time position of a skeletal animation on an Entity object" />
                            <description text="(i.e: use 0 to move it to start, 1 to end, 0.5 to move it to the middle of the timeline)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="animationName" type="String" desc="name of the animation (the animation must be enabled, first)" />
                                    <param name="timePos" type="Numeric" desc="time position (between 0.0 and 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la posizione lungo la timeline di un'animazione scheletrale su un oggetto Entity" />
                            <description text="(ad esempio: usare 0 per posizionarla all'inizio, 1 per posizionarla alla fine, 0.5 per posizionarla a meta' durata)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="animationName" type="String" desc="nome dell'animazione (l'animazione deve essere stata abilitata, prima)" />
                                    <param name="timePos" type="Numeric" desc="time position (tra 0.0 e 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int objectAnimationSetPosition(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.animation.setPosition", 3))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(objectName, "object.animation.setPosition");
            if (object != NULL && object->getType() == EOSOgreObject::EntityObjectType)
                {
                const char* animationName = luaL_checkstring(luaRuntime, 2);
                //
                EntityAnimation* entityAnim = object->animByName(animationName);
                if (entityAnim != NULL)
                    {
                    Ogre::Real timePos = (Ogre::Real)lua_tonumber(luaRuntime, 3);
                    if (timePos < 0) timePos = 0;
                    if (timePos > 1.0) timePos = 1.0;
                    entityAnim->animation->setTimePosition(timePos);
                    //
                    done = true;
                    }
                else
                    EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.setPosition] - Object '%s' doesn't have animation named '%s'. Try to enable it, first.", objectName, animationName);
                //
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectAnimationGetPosition">
                    <language code="en">
                            <brief text="Gets current time position of a skeletal animation on an Entity object (between 0.0 and 1.0)" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="animationName" type="String" desc="name of the animation (the animation must be enabled, first)" />
                            </params>
                            <returns>
                                    <return name="" type="Numeric" desc="Current animation position (in case of success), -1.0 otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la posizione lungo la timeline di un'animazione scheletrale su un oggetto Entity (compresa tra 0.0 e 1.0)" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="animationName" type="String" desc="nome dell'animazione (l'animazione deve essere stata abilitata, prima)" />
                            </params>
                            <returns>
                                    <return name="" type="Numeric" desc="Posizione attuale dell'animazione (in caso di successo), -1.0 altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int objectAnimationGetPosition(lua_State* luaRuntime)
        {
        Ogre::Real timePos = -1.0;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.animation.getPosition", 2))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(objectName, "object.animation.getPosition");
            if (object != NULL && object->getType() == EOSOgreObject::EntityObjectType)
                {
                const char* animationName = luaL_checkstring(luaRuntime, 2);
                //
                EntityAnimation* entityAnim = object->animByName(animationName);
                if (entityAnim != NULL)
                    timePos = entityAnim->animation->getTimePosition();
                else
                    EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.getPosition] - Object '%s' doesn't have animation named '%s'. Try to enable it, first.", objectName, animationName);
                //
                }
            }
        lua_pushnumber(luaRuntime, timePos);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectAnimationSetWeight">
                    <language code="en">
                            <brief text="Sets the weight (for blending purpose) of a skeletal animation on an Entity object" />
                            <description text="(this function is valid for Entity type objects only)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="animationName" type="String" desc="name of the animation (the animation must be enabled, first)" />
                                    <param name="animationWeight" type="Numeric" desc="weight of the animation to set" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il peso (per scopi di blending) di un'animazione scheletrale su un oggetto Entity" />
                            <description text="(questa funzione e' valida solo per oggetti di tipo Entity)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="animationName" type="String" desc="nome dell'animazione (l'animazione deve essere stata abilitata, prima)" />
                                    <param name="animationWeight" type="Numeric" desc="peso da dare all'animazione" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int objectAnimationSetWeight(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.animation.setWeight", 3))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(objectName, "object.animation.setWeight");
            const char* animationName = luaL_checkstring(luaRuntime, 2);
            if (object != NULL && object->getType() == EOSOgreObject::EntityObjectType)
                {
                EntityAnimation* entityAnim = object->animByName(animationName);
                if (entityAnim != NULL)
                    {
                    Ogre::Real animWeight = (Ogre::Real)lua_tonumber(luaRuntime, 3);
                    if (animWeight < 0) animWeight = 0;
                    if (animWeight > 1.0) animWeight = 1.0;
                    entityAnim->animation->setWeight(animWeight);
                    //
                    done = true;
                    }
                else
                    EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.setWeight] - Object '%s' doesn't have animation named '%s'. Try to enable it, first.", objectName, animationName);
                //
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectAnimationGetWeight">
                    <language code="en">
                            <brief text="Returns current weight (for blending purpose) of a skeletal animation on an Entity object" />
                            <description text="(this function is valid for Entity type objects only)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="animationName" type="String" desc="name of the animation which get the weight (the animation must be enabled, first)" />
                            </params>
                            <returns>
                                    <return name="" type="Numeric" desc="current weight of the animation (between 0.0 and 1.0)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il peso attuale (per scopi di blending) di un'animazione scheletrale da un oggetto Entity" />
                            <description text="(questa funzione e' valida solo per oggetti di tipo Entity)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="animationName" type="String" desc="nome dell'animazione di cui ritornare il peso (l'animazione deve essere stata abilitata, prima)" />
                            </params>
                            <returns>
                                    <return name="" type="Numeric" desc="peso attuale dell'animazione (compreso tra 0.0 e 1.0)" />
                            </returns>
                    </language>
            </function>
     */
    int objectAnimationGetWeight(lua_State* luaRuntime)
        {
        Ogre::Real animWeight = 0;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.animation.getWeight", 2))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(objectName, "object.animation.getWeight");
            const char* animationName = luaL_checkstring(luaRuntime, 2);
            if (object != NULL && object->getType() == EOSOgreObject::EntityObjectType)
                {
                EntityAnimation* entityAnim = object->animByName(animationName);
                if (entityAnim != NULL)
                    {
                    animWeight = entityAnim->animation->getWeight();
                    }
                else
                    EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.getWeight] - Object '%s' doesn't have animation named '%s'. Try to enable it, first.", objectName, animationName);
                //
                }
            }
        lua_pushnumber(luaRuntime, animWeight);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectAnimationSetSpeed">
                    <language code="en">
                            <brief text="Sets the execution speed of a skeletal animation on an Entity object" />
                            <description text="(this function is valid for Entity type objects only)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="animationName" type="String" desc="name of the animation (the animation must be enabled, first)" />
                                    <param name="animationWeight" type="Numeric" desc="weight of the animation to set" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la velocite' di esecuzione di un'animazione scheletrale su un oggetto Entity" />
                            <description text="(questa funzione e' valida solo per oggetti di tipo Entity)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="animationName" type="String" desc="nome dell'animazione (l'animazione deve essere stata abilitata, prima)" />
                                    <param name="animationWeight" type="Numeric" desc="peso da dare all'animazione" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int objectAnimationSetSpeed(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.animation.setSpeed", 3))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(objectName, "object.animation.setSpeed");
            const char* animationName = luaL_checkstring(luaRuntime, 2);
            if (object != NULL && object->getType() == EOSOgreObject::EntityObjectType)
                {
                EntityAnimation* entityAnim = object->animByName(animationName);
                if (entityAnim != NULL)
                    {
                    entityAnim->speed = (Ogre::Real)lua_tonumber(luaRuntime, 3);
                    //
                    done = true;
                    }
                else
                    EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.setSpeed] - Object '%s' doesn't have animation named '%s'. Try to enable it, first.", objectName, animationName);
                //
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectAnimationGetSpeed">
                    <language code="en">
                            <brief text="Returns current speed of a skeletal animation active on an Entity object" />
                            <description text="(this function is valid for Entity type objects only)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="animationName" type="String" desc="name of the animation which get the speed (the animation must be enabled, first)" />
                            </params>
                            <returns>
                                    <return name="" type="Numeric" desc="current speed multiplier of the animation" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il peso attuale (per scopi di blending) di un'animazione scheletrale da un oggetto Entity" />
                            <description text="(questa funzione e' valida solo per oggetti di tipo Entity)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="animationName" type="String" desc="nome dell'animazione di cui ritornare la velocita' (l'animazione deve essere stata abilitata, prima)" />
                            </params>
                            <returns>
                                    <return name="" type="Numeric" desc="moltiplicatore di velocita' attuale dell'animazione" />
                            </returns>
                    </language>
            </function>
     */
    int objectAnimationGetSpeed(lua_State* luaRuntime)
        {
        Ogre::Real animSpeed = 0;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.animation.getSpeed", 2))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(objectName, "object.animation.getSpeed");
            const char* animationName = luaL_checkstring(luaRuntime, 2);
            if (object != NULL && object->getType() == EOSOgreObject::EntityObjectType)
                {
                EntityAnimation* entityAnim = object->animByName(animationName);
                if (entityAnim != NULL)
                    {
                    animSpeed = entityAnim->speed;
                    }
                else
                    EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.getSpeed] - Object '%s' doesn't have animation named '%s'. Try to enable it, first.", objectName, animationName);
                //
                }
            }
        lua_pushnumber(luaRuntime, animSpeed);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectAnimationSetDirection">
                    <language code="en">
                            <brief text="Sets the direction (forward or reverse) of a skeletal animation on an Entity object" />
                            <description text="(this function is valid for Entity type objects only)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="animationName" type="String" desc="name of the animation (the animation must be enabled, first)" />
                                    <param name="direction" type="Boolean" desc="true to set it forward, false to set it reversed" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la direzione di esecuzione (forward o reverse) di un'animazione scheletrale su un oggetto Entity" />
                            <description text="(questa funzione e' valida solo per oggetti di tipo Entity)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="animationName" type="String" desc="nome dell'animazione (l'animazione deve essere stata abilitata, prima)" />
                                    <param name="direction" type="Boolean" desc="true per eseguirla in forward, false per eseguirla in reversed" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int objectAnimationSetDirection(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.animation.setDirection", 3))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(objectName, "object.animation.setDirection");
            const char* animationName = luaL_checkstring(luaRuntime, 2);
            if (object != NULL && object->getType() == EOSOgreObject::EntityObjectType)
                {
                EntityAnimation* entityAnim = object->animByName(animationName);
                if (entityAnim != NULL)
                    {
                    entityAnim->direction = lua_toboolean(luaRuntime, 3) ? 1.0 : -1.0;
                    //
                    done = true;
                    }
                else
                    EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.setDirection] - Object '%s' doesn't have animation named '%s'. Try to enable it, first.", objectName, animationName);
                //
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectAnimationGetDirection">
                    <language code="en">
                            <brief text="Returns current direction of a skeletal animation active on an Entity object" />
                            <description text="(this function is valid for Entity type objects only)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="animationName" type="String" desc="name of the animation (the animation must be enabled, first)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true if direction is forward, false if reversed" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la direzione attuale di esecuzione di un'animazione scheletrale attiva su un oggetto Entity" />
                            <description text="(questa funzione e' valida solo per oggetti di tipo Entity)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="animationName" type="String" desc="nome dell'animazione (l'animazione deve essere stata abilitata, prima)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true se la direzione e' forward, false se e' reversed" />
                            </returns>
                    </language>
            </function>
     */
    int objectAnimationGetDirection(lua_State* luaRuntime)
        {
        bool animDirection = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.animation.getDirection", 2))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(objectName, "object.animation.setDirection");
            const char* animationName = luaL_checkstring(luaRuntime, 2);
            if (object != NULL && object->getType() == EOSOgreObject::EntityObjectType)
                {
                EntityAnimation* entityAnim = object->animByName(animationName);
                if (entityAnim != NULL)
                    {
                    animDirection = (entityAnim->direction > 0);
                    }
                else
                    EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.setDirection] - Object '%s' doesn't have animation named '%s'. Try to enable it, first.", objectName, animationName);
                //
                }
            }
        lua_pushboolean(luaRuntime, animDirection);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectAnimationFreeze">
                    <language code="en">
                            <brief text="Freezes or unfreezes the animation of an Entity object (keeps object on current animation frame)" />
                            <description text="(this function is valid for Entity type objects only)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="animationName" type="String" desc="name of the animation to freeze|unfreeze (the animation must be enabled, first)" />
                                    <param name="action" type="Boolean" desc="true to freeze, false to unfreeze" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Blocca o sblocca l'animazione scheletrale di un oggetto Entity (tiene bloccato l'oggetto sul frame di animazione attuale)" />
                            <description text="(questa funzione e' valida solo per oggetti di tipo Entity)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="animationName" type="String" desc="nome dell'animazione da bloccare|sbloccare (l'animazione deve essere stata abilitata, prima)" />
                                    <param name="action" type="Boolean" desc="true per bloccare, false per sbloccare" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int objectAnimationFreeze(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.animation.freeze", 3))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(objectName, "object.animation.freeze");
            if (object != NULL && object->getType() == EOSOgreObject::EntityObjectType)
                {
                const char* animationName = luaL_checkstring(luaRuntime, 2);
                EntityAnimation* entityAnim = object->animByName(animationName);
                if (entityAnim != NULL)
                    {
                    entityAnim->frozen = lua_toboolean(luaRuntime, 3);
                    //
                    done = true;
                    }
                else
                    EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.freeze] - Object '%s' doesn't have animation named '%s'. Try to enable it, first.", objectName, animationName);
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectAnimationIsFrozen">
                    <language code="en">
                            <brief text="Returns current playing status (frozen or playing) of a skeletal animation active on an Entity object" />
                            <description text="(this function is valid for Entity type objects only)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="animationName" type="String" desc="name of the animation (the animation must be enabled, first)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true if animation is frozen, false if not" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna lo stato di riproduzione attuale (bloccata o in esecuzione) di un'animazione scheletrale attiva su un oggetto Entity" />
                            <description text="(questa funzione e' valida solo per oggetti di tipo Entity)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="animationName" type="String" desc="nome dell'animazione (l'animazione deve essere stata abilitata, prima)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true se l'animazione e' bloccata, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int objectAnimationIsFrozen(lua_State* luaRuntime)
        {
        bool animFrozen = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.animation.isFrozen", 2))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(objectName, "object.animation.isFrozen");
            const char* animationName = luaL_checkstring(luaRuntime, 2);
            if (object != NULL && object->getType() == EOSOgreObject::EntityObjectType)
                {
                EntityAnimation* entityAnim = object->animByName(animationName);
                if (entityAnim != NULL)
                    animFrozen = entityAnim->frozen;
                else
                    EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.isFrozen] - Object '%s' doesn't have animation named '%s'. Try to enable it, first.", objectName, animationName);
                //
                }
            }
        lua_pushboolean(luaRuntime, animFrozen);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectBoneGetPosition">
                    <language code="en">
                            <brief text="Returns current position of a Skeleton Bone of an Entity object" />
                            <description text="(this function applies only to Entity objects with skeletal animation)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="boneName" type="String" desc="name of the bone" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="X component of current position of the bone" />
                                    <return name="y" type="Numeric" desc="Y component of current position of the bone" />
                                    <return name="z" type="Numeric" desc="Z component of current position of the bone" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la posizione corrente di un Bone dello Skeleton di un oggetto Entity" />
                            <description text="(questa funzione si applica solo ad oggetti Entity dotati di animazione scheletrale)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="boneName" type="String" desc="nome del bone" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="componente X della posizione corrente del bone" />
                                    <return name="y" type="Numeric" desc="componente Y della posizione corrente del bone" />
                                    <return name="z" type="Numeric" desc="componente Z della posizione corrente del bone" />
                            </returns>
                    </language>
            </function>
     */
    int objectBoneGetPosition(lua_State* luaRuntime)
        {
        Ogre::Vector3 pos = Ogre::Vector3::ZERO;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.animation.bone.getPosition", 2))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(objectName, "object.animation.bone.getPosition");
            if (object != NULL && object->getType() == EOSOgreObject::EntityObjectType)
                {
                const char* boneName = luaL_checkstring(luaRuntime, 2);
                Ogre::SkeletonInstance* entitySkeleton = object->getEntity()->getSkeleton();
                if (entitySkeleton != NULL)
                    {
                    try
                        {
                        Ogre::Bone* bone = entitySkeleton->getBone(boneName);
                        //pos = bone->getWorldPosition();
                        //pos = object->sceneNode->getWorldPosition() + bone->getWorldPosition();
                        //pos = object->getEntity()->_getParentNodeFullTransform() * bone->_getDerivedPosition();
                        pos = object->cachedSceneNode->sceneNode->_getFullTransform() * bone->_getDerivedPosition();
                        //pos = bone->_getDerivedPosition();
                        }
                    catch (...)
                        {
                        EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.bone.getPosition] - Warning! Object '%s' doesn't have a Bone named '%s'!", objectName, boneName);
                        }
                    }
                else
                    EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.bone.getPosition] - Object '%s' doesn't have a skeleton!", objectName);
                //
                }
            }
        //
        lua_pushnumber(luaRuntime, pos.x);
        lua_pushnumber(luaRuntime, pos.y);
        lua_pushnumber(luaRuntime, pos.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectBoneGetOrientation">
                    <language code="en">
                            <brief text="Returns current orientation of a Skeleton Bone of an Entity object" />
                            <description text="(this function applies only to Entity objects with skeletal animation)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="boneName" type="String" desc="name of the bone" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="X component of current orientation of the bone" />
                                    <return name="y" type="Numeric" desc="Y component of current orientation of the bone" />
                                    <return name="z" type="Numeric" desc="Z component of current orientation of the bone" />
                                    <return name="w" type="Numeric" desc="W component of current orientation of the bone" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna l'orientamento corrente di un Bone dello Skeleton di un oggetto Entity" />
                            <description text="(questa funzione si applica solo ad oggetti Entity dotati di animazione scheletrale)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="boneName" type="String" desc="nome del bone" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="componente X dell'orientamento corrente del bone" />
                                    <return name="y" type="Numeric" desc="componente Y dell'orientamento corrente del bone" />
                                    <return name="z" type="Numeric" desc="componente Z dell'orientamento corrente del bone" />
                                    <return name="w" type="Numeric" desc="componente W dell'orientamento corrente del bone" />
                            </returns>
                    </language>
            </function>
     */
    int objectBoneGetOrientation(lua_State* luaRuntime)
        {
        Ogre::Quaternion q = Ogre::Quaternion::IDENTITY;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.animation.bone.getOrientation", 2))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(objectName, "object.animation.bone.getOrientation");
            if (object != NULL && object->getType() == EOSOgreObject::EntityObjectType)
                {
                const char* boneName = luaL_checkstring(luaRuntime, 2);
                Ogre::SkeletonInstance* entitySkeleton = object->getEntity()->getSkeleton();
                if (entitySkeleton != NULL)
                    {
                    try
                        {
                        Ogre::Bone* bone = entitySkeleton->getBone(boneName);
                        //pos = bone->getWorldPosition();
                        //pos = object->sceneNode->getWorldPosition() + bone->getWorldPosition();
                        //pos = object->getEntity()->_getParentNodeFullTransform() * bone->_getDerivedPosition();
                        //q = object->sceneNode->_getFullTransform() * bone->_getDerivedOrientation();
                        q = bone->_getDerivedOrientation();
                        }
                    catch (...)
                        {
                        EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.bone.getOrientation] - Warning! Object '%s' doesn't have a Bone named '%s'!", objectName, boneName);
                        }
                    }
                else
                    EOSLuaRuntime::getInstance()->logErrorMessage("[eos."_MODULE_NAME"][object.animation.bone.getOrientation] - Object '%s' doesn't have a skeleton!", objectName);
                //
                }
            }
        //
        lua_pushnumber(luaRuntime, q.x);
        lua_pushnumber(luaRuntime, q.y);
        lua_pushnumber(luaRuntime, q.z);
        lua_pushnumber(luaRuntime, q.w);
        return 4;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectShowBoundingBox">
                    <language code="en">
                            <brief text="Activates or deactivates the visibility of the bounding box (AABB) of an object" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="bboxShow" type="Boolean" desc="true to show bounding box, false to hide it" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Attiva o disattiva la visibilita' del bounding box (AABB) di un oggetto" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="bboxShow" type="Boolean" desc="true per mostrare il bounding box, false per nasconderlo" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int objectShowBoundingBox(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.showBoundingBox", 2))
            {
            EOSOgreObject* object = EOSG3D::getInstance()->objectByName(luaL_checkstring(luaRuntime, 1), "object.showBoundingBox");
            if (object != NULL && object->cachedSceneNode != NULL)
                {
                object->cachedSceneNode->sceneNode->showBoundingBox(lua_toboolean(luaRuntime, 2));
                bool done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectSetMaterial">
                    <language code="en">
                            <brief text="Sets the material name of an object" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                                    <param name="materialName" type="String" desc="material name to assign to the object" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il nome del materiale di un oggetto" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                                    <param name="materialName" type="String" desc="nome del materiale da assegnare all'oggetto" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int objectSetMaterial(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.setMaterial", 2))
            {
            EOSOgreObject* theObject = EOSG3D::getInstance()->objectByName(luaL_checkstring(luaRuntime, 1), "object.setMaterial");
            if (theObject != NULL && theObject->getType() == EOSOgreObject::EntityObjectType)
                {
                theObject->getEntity()->setMaterialName(luaL_checkstring(luaRuntime, 2));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectGetMinimumSize">
                    <language code="en">
                            <brief text="Returns the minimum size of the Axis Aligned Box of an object" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                            </params>
                            <returns>
                                    <return name="x_size" type="Numeric" desc="minimum x size of the object" />
                                    <return name="y_size" type="Numeric" desc="minimum y size of the object" />
                                    <return name="z_size" type="Numeric" desc="minimum z size of the object" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la dimensione minima dell'Axis Aligned Box di un oggetto" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                            </params>
                            <returns>
                                    <return name="x_size" type="Numeric" desc="dimensione minima x dell'oggetto" />
                                    <return name="y_size" type="Numeric" desc="dimensione minima y dell'oggetto" />
                                    <return name="z_size" type="Numeric" desc="dimensione minima z dell'oggetto" />
                            </returns>
                    </language>
            </function>
     */
    int objectGetMinimumSize(lua_State* luaRuntime)
        {
        Ogre::Vector3 size = Ogre::Vector3::ZERO;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.getMinimumSize", 1))
            {
            EOSOgreObject* theObject = EOSG3D::getInstance()->objectByName(luaL_checkstring(luaRuntime, 1), "object.getMinimumSize");
            if (theObject != NULL)
                {
                switch (theObject->getType())
                    {
                    case EOSOgreObject::EntityObjectType:
                        size = theObject->getEntity()->getBoundingBox().getMinimum();
                        break;
                    case EOSOgreObject::BillboardSetObjectType:
                        size = theObject->getBillboardSet()->getBoundingBox().getMinimum();
                        break;
                    case EOSOgreObject::ParticleSystemObjectType:
                        size = theObject->getParticleSystem()->getBoundingBox().getMinimum();
                        break;
                    }
                }
            }
        lua_pushnumber(luaRuntime, size.x);
        lua_pushnumber(luaRuntime, size.y);
        lua_pushnumber(luaRuntime, size.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectGetMaximumSize">
                    <language code="en">
                            <brief text="Returns the maximum size of the Axis Aligned Box of an object" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                            </params>
                            <returns>
                                    <return name="x_size" type="Numeric" desc="maximum x size of the object" />
                                    <return name="y_size" type="Numeric" desc="maximum y size of the object" />
                                    <return name="z_size" type="Numeric" desc="maximum z size of the object" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la dimensione massima dell'Axis Aligned Box di un oggetto" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                            </params>
                            <returns>
                                    <return name="x_size" type="Numeric" desc="dimensione massima x dell'oggetto" />
                                    <return name="y_size" type="Numeric" desc="dimensione massima y dell'oggetto" />
                                    <return name="z_size" type="Numeric" desc="dimensione massima z dell'oggetto" />
                            </returns>
                    </language>
            </function>
     */
    int objectGetMaximumSize(lua_State* luaRuntime)
        {
        Ogre::Vector3 size = Ogre::Vector3::ZERO;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.getMaximumSize", 1))
            {
            EOSOgreObject* theObject = EOSG3D::getInstance()->objectByName(luaL_checkstring(luaRuntime, 1), "object.getMaximumSize");
            if (theObject != NULL)
                {
                switch (theObject->getType())
                    {
                    case EOSOgreObject::EntityObjectType:
                        size = theObject->getEntity()->getBoundingBox().getMaximum();
                        break;
                    case EOSOgreObject::BillboardSetObjectType:
                        size = theObject->getBillboardSet()->getBoundingBox().getMaximum();
                        break;
                    case EOSOgreObject::ParticleSystemObjectType:
                        size = theObject->getParticleSystem()->getBoundingBox().getMaximum();
                        break;
                    }
                }
            }
        lua_pushnumber(luaRuntime, size.x);
        lua_pushnumber(luaRuntime, size.y);
        lua_pushnumber(luaRuntime, size.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="objectGetScreenBoundingRect">
                    <language code="en">
                            <brief text="" />
                            <description text="Returns the screen-projected bounding rectangle of an Entity object (based on current camera view)" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the object" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="relative X coordinate of top-left rectangle corner" />
                                    <return name="y" type="Numeric" desc="relative Y coordinate of top-left rectangle corner" />
                                    <return name="width" type="Numeric" desc="relative width of the rectangle" />
                                    <return name="height" type="Numeric" desc="relative height of the rectangle" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="" />
                            <description text="Ritorna il bounding rectangle proiettato sullo schermo di un oggetto Entity (basato sulla visuale corrente della camera)" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="coordinata X relativa dell'angolo superiore sinistro del rectangle" />
                                    <return name="y" type="Numeric" desc="coordinata Y relativa dell'angolo superiore sinistro del rectangle" />
                                    <return name="width" type="Numeric" desc="larghezza relativa del rectangle" />
                                    <return name="height" type="Numeric" desc="altezza relativa del rectangle" />
                            </returns>
                    </language>
            </function>
     */
    int objectGetScreenBoundingRect(lua_State* luaRuntime)
        {
        float min_x = 1.0f, max_x = 0.0f, min_y = 1.0f, max_y = 0.0f;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "object.getScreenBoundingRect", 1))
            {
            EOSOgreObject* theObject = EOSG3D::getInstance()->objectByName(luaL_checkstring(luaRuntime, 1), "object.getScreenBoundingRect");
            if (theObject != NULL && theObject->getType() == EOSOgreObject::EntityObjectType)
                {
                Ogre::Entity* objectEntity = theObject->getEntity();
                //
                // get the projection of the object's AABB into screen space
                const Ogre::AxisAlignedBox& bbox = objectEntity->getWorldBoundingBox(true);
                Ogre::Matrix4 mat = EOSG3D::getInstance()->getDefaultCamera()->getViewMatrix();
                const Ogre::Vector3* corners = bbox.getAllCorners();
                // expand the screen-space bounding-box so that it completely encloses the object's AABB
                Ogre::Camera* camera = EOSG3D::getInstance()->getDefaultCamera();
                for (int i = 0; i < 8; i++)
                    {
                    Ogre::Vector3 corner = corners[i];
                    // multiply the AABB corner vertex by the view matrix to get a camera-space vertex
                    corner = camera->getProjectionMatrix() * (camera->getViewMatrix() * corner);
                    // make 2D relative/normalized coords from the view-space vertex
                    float xx = 1.0f - ((corner.x * 0.5f) + 0.5);
                    float yy = 1.0f - ((corner.y * 0.5f) + 0.5);
                    //
                    if (xx < min_x) min_x = xx;
                    if (xx > max_x) max_x = xx;
                    if (yy < min_y) min_y = yy;
                    if (yy > max_y) max_y = yy;
                    }
                }
            }
        //
        lua_pushnumber(luaRuntime, 1.0 - min_x);
        lua_pushnumber(luaRuntime, min_y);
        lua_pushnumber(luaRuntime, max_x - min_x);
        lua_pushnumber(luaRuntime, max_y - min_y);
        return 4;
        }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="lightCreate">
                    <language code="en">
                            <brief text="Creates a new Light object in the scene" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the new Light to create" />
                            </params>
                            <returns>
                                    <return name="sceneNodeName" type="String" desc="name of the scene node where new light is attached, nil in case of errors" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Crea un nuovo oggetto Light nella scena" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome del nuovo oggetto Light da creare" />
                            </params>
                            <returns>
                                    <return name="sceneNodeName" type="String" desc="nome del nodo della scena dove e' stato legato il nuovo oggetto light, nil in caso di errore" />
                            </returns>
                    </language>
            </function>
     */
    int lightCreate(lua_State* luaRuntime)
        {
        const char* sceneNodeName = NULL;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.create", 1))
            {
            std::string lightName = luaL_checkstring(luaRuntime, 1);
            std::string lightNodeName = lightName + "_node";
            // Creates the light
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->createLight(lightName.c_str());
            if (ogreLight != NULL)
                {
                ogreLight->setType(Ogre::Light::LT_POINT);
                Ogre::SceneNode* lightNode = EOSG3D::getInstance()->getSceneManager()->getRootSceneNode()->createChildSceneNode(lightNodeName.c_str());
                if (lightNode != NULL) sceneNodeName = lightNode->getName().c_str();
                lightNode->attachObject(ogreLight);
                }
            }
        lua_pushstring(luaRuntime, sceneNodeName);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightRemove">
                    <language code="en">
                            <brief text="Removes a Light object from the scene" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object to remove" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Rimuove un oggetto Light dalla scena" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light da rimuovere" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int lightRemove(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.remove", 1))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                {
                EOSG3D::getInstance()->getSceneManager()->destroySceneNode(ogreLight->getParentNode()->getName());
                EOSG3D::getInstance()->getSceneManager()->destroyLight(ogreLight);
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightGetNodeName">
                    <language code="en">
                            <brief text="Returns the name of the node where a Light object is attached" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                            </params>
                            <returns>
                                    <return name="nodeName" type="String" desc="name of the scene node (empty string in case of error)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il nome dello scene node al quale e' attaccato un oggetto Light" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                            </params>
                            <returns>
                                    <return name="nodeName" type="String" desc="nome dello scene node (stringa vuota in caso di errore)" />
                            </returns>
                    </language>
            </function>
     */
    int lightGetNodeName(lua_State* luaRuntime)
        {
        std::string nodeName = "";
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.getNodeName", 1))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                nodeName = ogreLight->getParentNode()->getName();
            }
        lua_pushstring(luaRuntime, nodeName.c_str());
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightSetTypePoint">
                    <language code="en">
                            <brief text="Sets a Light object of type 'POINT'" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Imposta un oggetto Light al tipo 'POINT'" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int lightSetTypePoint(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.setTypePoint", 1))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                {
                ogreLight->setType(Ogre::Light::LT_POINT);
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightSetTypeDirectional">
                    <language code="en">
                            <brief text="Sets a Light object of type 'DIRECTIONAL'" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Imposta un oggetto Light al tipo 'DIRECTIONAL'" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int lightSetTypeDirectional(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.setTypeDirectional", 1))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                {
                ogreLight->setType(Ogre::Light::LT_DIRECTIONAL);
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightSetTypeSpot">
                    <language code="en">
                            <brief text="Sets a Light object of type 'SPOTLIGHT'" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Imposta un oggetto Light al tipo 'SPOTLIGHT'" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int lightSetTypeSpot(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.setTypeSpot", 1))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                {
                ogreLight->setType(Ogre::Light::LT_SPOTLIGHT);
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightGetType">
                    <language code="en">
                            <brief text="Returns the type of a Light object" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                            </params>
                            <returns>
                                    <return name="lightType" type="String" desc="name of the Light type (POINT | DIRECTIONAL | SPOTLIGHT)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il tipo di un oggetto Light" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                            </params>
                            <returns>
                                    <return name="lightType" type="String" desc="nome del Light Type (POINT | DIRECTIONAL | SPOTLIGHT)" />
                            </returns>
                    </language>
            </function>
     */
    int lightGetType(lua_State* luaRuntime)
        {
        std::string lightTypeName = NULL;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.getType", 1))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                {
                Ogre::Light::LightTypes lt = ogreLight->getType();
                switch (lt)
                    {
                    case Ogre::Light::LT_POINT:
                        lightTypeName = "POINT";
                        break;
                    case Ogre::Light::LT_DIRECTIONAL:
                        lightTypeName = "DIRECTIONAL";
                        break;
                    case Ogre::Light::LT_SPOTLIGHT:
                        lightTypeName = "SPOTLIGHT";
                        break;
                    }
                }
            }
        lua_pushstring(luaRuntime, lightTypeName.c_str());
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightSetDiffuseColor">
                    <language code="en">
                            <brief text="Sets the diffuse color of a Light object" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                                    <param name="r" type="Numeric" desc="Red   amount of the color" />
                                    <param name="g" type="Numeric" desc="Green amount of the color" />
                                    <param name="b" type="Numeric" desc="Blue  amount of the color" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il colore della diffusa di un oggetto Light" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                                    <param name="r" type="Numeric" desc="Componente Red   del colore" />
                                    <param name="g" type="Numeric" desc="Componente Green del colore" />
                                    <param name="b" type="Numeric" desc="Componente Blue  del colore" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int lightSetDiffuseColor(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.setDiffuseColor", 4))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                {
                ogreLight->setDiffuseColour(Ogre::ColourValue((Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4)));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightGetDiffuseColor">
                    <language code="en">
                            <brief text="Returns the diffuse color of a Light object. (In case of error, returned value is 0,0,0)" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                            </params>
                            <returns>
                                    <return name="r" type="Numeric" desc="Red   amount of the color" />
                                    <return name="g" type="Numeric" desc="Green amount of the color" />
                                    <return name="b" type="Numeric" desc="Blue  amount of the color" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il colore della diffusa di un oggetto Light. (in caso di errore, il colore ritornato sara' 0,0,0)" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                            </params>
                            <returns>
                                    <return name="r" type="Numeric" desc="Componente Red   del colore" />
                                    <return name="g" type="Numeric" desc="Componente Green del colore" />
                                    <return name="b" type="Numeric" desc="Componente Blue  del colore" />
                            </returns>
                    </language>
            </function>
     */
    int lightGetDiffuseColor(lua_State* luaRuntime)
        {
        Ogre::ColourValue color = Ogre::ColourValue::Black;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.getDiffuseColor", 1))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                color = ogreLight->getDiffuseColour();
            }
        lua_pushnumber(luaRuntime, color.r);
        lua_pushnumber(luaRuntime, color.g);
        lua_pushnumber(luaRuntime, color.b);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightSetSpecularColor">
                    <language code="en">
                            <brief text="Sets the specular color of a Light object" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                                    <param name="r" type="Numeric" desc="Red   amount of the color" />
                                    <param name="g" type="Numeric" desc="Green amount of the color" />
                                    <param name="b" type="Numeric" desc="Blue  amount of the color" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il colore della speculare di un oggetto Light" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                                    <param name="r" type="Numeric" desc="Componente Red   del colore" />
                                    <param name="g" type="Numeric" desc="Componente Green del colore" />
                                    <param name="b" type="Numeric" desc="Componente Blue  del colore" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int lightSetSpecularColor(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.setSpecularColor", 4))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                {
                ogreLight->setSpecularColour(Ogre::ColourValue((Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4)));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightGetSpecularColor">
                    <language code="en">
                            <brief text="Returns the specular color of a Light object. (In case of error, returned value is 0,0,0)" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                            </params>
                            <returns>
                                    <return name="r" type="Numeric" desc="Red   amount of the color" />
                                    <return name="g" type="Numeric" desc="Green amount of the color" />
                                    <return name="b" type="Numeric" desc="Blue  amount of the color" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il colore della speculare di un oggetto Light. (in caso di errore, il colore ritornato sara' 0,0,0)" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                            </params>
                            <returns>
                                    <return name="r" type="Numeric" desc="Componente Red   del colore" />
                                    <return name="g" type="Numeric" desc="Componente Green del colore" />
                                    <return name="b" type="Numeric" desc="Componente Blue  del colore" />
                            </returns>
                    </language>
            </function>
     */
    int lightGetSpecularColor(lua_State* luaRuntime)
        {
        Ogre::ColourValue color = Ogre::ColourValue::Black;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.getSpecularColor", 1))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                color = ogreLight->getSpecularColour();
            }
        lua_pushnumber(luaRuntime, color.r);
        lua_pushnumber(luaRuntime, color.g);
        lua_pushnumber(luaRuntime, color.b);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightSetAttenuation">
                    <language code="en">
                            <brief text="Sets the attenuation parameters of the light source, ie how it diminishes with distance" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                                    <param name="range"     type="Numeric" desc="The absolute upper range of the light in world units" />
                                    <param name="constant"  type="Numeric" desc="The constant factor in the attenuation formula: 1.0 means never attenuate, 0.0 is complete attenuation" />
                                    <param name="linear"    type="Numeric" desc="The linear factor in the attenuation formula: 1 means attenuate evenly over the distance" />
                                    <param name="quadratic" type="Numeric" desc="The quadratic factor in the attenuation formula: adds a curvature to the attenuation formula." />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta i parametri di attenuazione di un oggetto Light, ovvero come deve attenuarsi in base alla distanza" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                                    <param name="range"     type="Numeric" desc="Range superiore assoluto della luce (in unita' world)" />
                                    <param name="constant"  type="Numeric" desc="Fattore costante nella formula di attenuazione: 1.0 nessuna attenuazione, 0.0 attenuazione totale" />
                                    <param name="linear"    type="Numeric" desc="Fattore lineare nella formula di attenuazione: 1 equivale ad un'attenuazione uniforme lungo tutta la distanza" />
                                    <param name="quadratic" type="Numeric" desc="Fattore quadratico nella formula di attenuazione: aggiunge una curvatura nella formula di attenuazione" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int lightSetAttenuation(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.setAttenuation", 5))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                {
                ogreLight->setAttenuation((Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4), (Ogre::Real)lua_tonumber(luaRuntime, 5));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightGetAttenuation">
                    <language code="en">
                            <brief text="Returns the attenuation parameters of the light source" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                            </params>
                            <returns>
                                    <return name="range"     type="Numeric" desc="The absolute upper range of the light in world units" />
                                    <return name="constant"  type="Numeric" desc="The constant factor in the attenuation formula: 1.0 means never attenuate, 0.0 is complete attenuation" />
                                    <return name="linear"    type="Numeric" desc="The linear factor in the attenuation formula: 1 means attenuate evenly over the distance" />
                                    <return name="quadratic" type="Numeric" desc="The quadratic factor in the attenuation formula: adds a curvature to the attenuation formula." />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna i parametri di attenuazione di un oggetto Light" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                            </params>
                            <returns>
                                    <return name="range"     type="Numeric" desc="Range superiore assoluto della luce (in unita' world)" />
                                    <return name="constant"  type="Numeric" desc="Fattore costante nella formula di attenuazione: 1.0 nessuna attenuazione, 0.0 attenuazione totale" />
                                    <return name="linear"    type="Numeric" desc="Fattore lineare nella formula di attenuazione: 1 equivale ad un'attenuazione uniforme lungo tutta la distanza" />
                                    <return name="quadratic" type="Numeric" desc="Fattore quadratico nella formula di attenuazione: aggiunge una curvatura nella formula di attenuazione" />
                            </returns>
                    </language>
            </function>
     */
    int lightGetAttenuation(lua_State* luaRuntime)
        {
        Ogre::Real range = -1, constant = -1, linear = -1, quadratic = -1;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.getAttenuation", 1))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                {
                range = ogreLight->getAttenuationRange();
                constant = ogreLight->getAttenuationConstant();
                linear = ogreLight->getAttenuationLinear();
                quadratic = ogreLight->getAttenuationQuadric();
                }
            }
        lua_pushnumber(luaRuntime, range);
        lua_pushnumber(luaRuntime, constant);
        lua_pushnumber(luaRuntime, linear);
        lua_pushnumber(luaRuntime, quadratic);
        return 4;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightSetPosition">
                    <language code="en">
                            <brief text="Sets the absolute position of a Light object" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                                    <param name="x" type="Numeric" desc="X component of the position" />
                                    <param name="y" type="Numeric" desc="Y component of the position" />
                                    <param name="z" type="Numeric" desc="Z component of the position" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la posizione assoluta di un oggetto Light" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                                    <param name="x" type="Numeric" desc="componente X della posizione" />
                                    <param name="y" type="Numeric" desc="componente Y della posizione" />
                                    <param name="z" type="Numeric" desc="componente Z della posizione" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int lightSetPosition(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.setPosition", 4))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                {
                ogreLight->getParentNode()->setPosition((Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightGetPosition">
                    <language code="en">
                            <brief text="Returns the position of a Light object" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="X component of the position" />
                                    <return name="y" type="Numeric" desc="Y component of the position" />
                                    <return name="z" type="Numeric" desc="Z component of the position" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la posizione di un oggetto Light" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="componente X della posizione" />
                                    <return name="y" type="Numeric" desc="componente Y della posizione" />
                                    <return name="z" type="Numeric" desc="componente Z della posizione" />
                            </returns>
                    </language>
            </function>
     */
    int lightGetPosition(lua_State* luaRuntime)
        {
        Ogre::Vector3 position = Ogre::Vector3(-1, -1, -1);
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.getPosition", 1))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                position = ogreLight->getParentNode()->getPosition();
            }
        lua_pushnumber(luaRuntime, position.x);
        lua_pushnumber(luaRuntime, position.y);
        lua_pushnumber(luaRuntime, position.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightSetDirection">
                    <language code="en">
                            <brief text="Sets the direction in which a Light object points (applicable only to Spot or Directional light sources)" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                                    <param name="x" type="Numeric" desc="X component of the direction vector" />
                                    <param name="y" type="Numeric" desc="Y component of the direction vector" />
                                    <param name="z" type="Numeric" desc="Z component of the direction vector" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la direzione verso la quale punta un oggetto Light (applicabile solo ad oggetti Light di tipo Spot o Directional)" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                                    <param name="x" type="Numeric" desc="componente X del vettore di direzione" />
                                    <param name="y" type="Numeric" desc="componente Y del vettore di direzione" />
                                    <param name="z" type="Numeric" desc="componente Z del vettore di direzione" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int lightSetDirection(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.setDirection", 4))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                {
                ogreLight->setDirection((Ogre::Real)lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightGetDirection">
                    <language code="en">
                            <brief text="Returns the direction of a Light object (valid only for the spotlight and directional light types)" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="X component of the position" />
                                    <return name="y" type="Numeric" desc="Y component of the position" />
                                    <return name="z" type="Numeric" desc="Z component of the position" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la direzione di un oggetto Light (valido solo per oggetti Light di tipo Spot o Directional)" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="componente X della posizione" />
                                    <return name="y" type="Numeric" desc="componente Y della posizione" />
                                    <return name="z" type="Numeric" desc="componente Z della posizione" />
                            </returns>
                    </language>
            </function>
     */
    int lightGetDirection(lua_State* luaRuntime)
        {
        Ogre::Vector3 direction = Ogre::Vector3(-1, -1, -1);
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.getDirection", 1))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                direction = ogreLight->getDirection();
            }
        lua_pushnumber(luaRuntime, direction.x);
        lua_pushnumber(luaRuntime, direction.y);
        lua_pushnumber(luaRuntime, direction.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightSetSpotParams">
                    <language code="en">
                            <brief text="Sets the paramaters of a Spot type Light object (the angle of the inner and outer cones and the rate of falloff between them)" />
                            <description text="(Note: if not yet of the type Spot, the Light object will be automatically set to type SPOTLIGHT)" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                                    <param name="innerAngle" type="Numeric" desc="the angle (in radians) covered by the bright inner cone. (This is applicable only to Direct3D: it will always treated as zero in OpenGL)" />
                                    <param name="outerAngle" type="Numeric" desc="the angle (in radians) covered by the outer cone" />
                                    <param name="falloff" type="Numeric" desc="the rate of falloff between the inner and outer cones. 1.0 = linear falloff, < 1.0 = slower falloff, > 1.0 = faster falloff" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta i parametri di un oggetto Light di tipo Spot (l'angolo del cono interno e quello esterno, e il fattore di decadimento tra essi)" />
                            <description text="(Nota: se l'oggetto Light non e' gia' di tipo Spot, verra' automaticamente settato al tipo SPOTLIGHT)" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                                    <param name="innerAngle" type="Numeric" desc="l'angolo (in radianti) del cono di luce interno. (Applicable solo in modalita' Direct3D: viene ignorato (posto a 0) in OpenGL)" />
                                    <param name="outerAngle" type="Numeric" desc="l'angolo (in radianti) del cono di luce esterno" />
                                    <param name="falloff" type="Numeric" desc="il fattore di decadimento tra il cono interno e quelo esterno. 1.0 = falloff lineare, < 1.0 = falloff minore, > 1.0 = falloff maggiore" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int lightSetSpotParams(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.setSpotParams", 4))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                {
                ogreLight->setType(Ogre::Light::LT_SPOTLIGHT);
                ogreLight->setSpotlightRange(Ogre::Radian(lua_tonumber(luaRuntime, 2)), Ogre::Radian(lua_tonumber(luaRuntime, 3)), (Ogre::Real)lua_tonumber(luaRuntime, 4));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightGetSpotParams">
                    <language code="en">
                            <brief text="Returns the paramaters of a Spot type Light object" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                            </params>
                            <returns>
                                    <return name="innerAngle" type="Numeric" desc="the angle (in radians) covered by the bright inner cone. (This is applicable only to Direct3D: it will always zero in OpenGL)" />
                                    <return name="outerAngle" type="Numeric" desc="the angle (in radians) covered by the outer cone" />
                                    <return name="falloff" type="Numeric" desc="the rate of falloff between the inner and outer cones. 1.0 = linear falloff, < 1.0 = slower falloff, > 1.0 = faster falloff" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna i parametri di un oggetto Light di tipo Spot" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                            </params>
                            <returns>
                                    <return name="innerAngle" type="Numeric" desc="l'angolo (in radianti) del cono di luce interno. (Applicable solo in modalita' Direct3D: sempre uguale a 0 in OpenGL)" />
                                    <return name="outerAngle" type="Numeric" desc="l'angolo (in radianti) del cono di luce esterno" />
                                    <return name="falloff" type="Numeric" desc="il fattore di decadimento tra il cono interno e quelo esterno. 1.0 = falloff lineare, < 1.0 = falloff minore, > 1.0 = falloff maggiore" />
                            </returns>
                    </language>
            </function>
     */
    int lightGetSpotParams(lua_State* luaRuntime)
        {
        Ogre::Radian innerAngle = Ogre::Radian(-1);
        Ogre::Radian outerAngle = Ogre::Radian(-1);
        Ogre::Real falloff = -1;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.getSpotParams", 1))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                {
                innerAngle = ogreLight->getSpotlightInnerAngle();
                outerAngle = ogreLight->getSpotlightOuterAngle();
                falloff = ogreLight->getSpotlightFalloff();
                }
            }
        lua_pushnumber(luaRuntime, innerAngle.valueRadians());
        lua_pushnumber(luaRuntime, outerAngle.valueRadians());
        lua_pushnumber(luaRuntime, falloff);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightSetVisible">
                    <language code="en">
                            <brief text="Sets the visibility status of a Light object" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                                    <param name="status" type="Boolean" desc="the visibility status to set" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta lo stato di visibilita' di un oggetto Light" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                                    <param name="status" type="Boolean" desc="stato di visibilita' da settare" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int lightSetVisible(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.setVisible", 2))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                {
                ogreLight->setVisible(lua_toboolean(luaRuntime, 2));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightIsVisible">
                    <language code="en">
                            <brief text="Returns whether or not a Light object is supposed to be visible or not" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                            </params>
                            <returns>
                                    <return name="visible" type="Boolean" desc="the visibility status of the object" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il probabile stato di visibilita' di un oggetto Light" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                            </params>
                            <returns>
                                    <return name="visible" type="Boolean" desc="lo stato di visibilita' dell'oggetto" />
                            </returns>
                    </language>
            </function>
     */
    int lightIsVisible(lua_State* luaRuntime)
        {
        bool visible = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.isVisible", 1))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                visible = ogreLight->isVisible();
            }
        lua_pushboolean(luaRuntime, visible);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightSetCastShadows">
                    <language code="en">
                            <brief text="Sets whether or not a Light object will cast shadows" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                                    <param name="status" type="Boolean" desc="the status to set" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la capacita' di un oggetto Light di gettare ombre" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                                    <param name="status" type="Boolean" desc="stato da settare" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int lightSetCastShadows(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.setCastShadows", 2))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                {
                ogreLight->setCastShadows(lua_toboolean(luaRuntime, 2));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="lightGetCastShadows">
                    <language code="en">
                            <brief text="Returns whether shadow casting is enabled for a Light object" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Light object" />
                            </params>
                            <returns>
                                    <return name="visible" type="Boolean" desc="the shadow casting status of the object" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna lo stato di shadow casting di oggetto Light" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Light" />
                            </params>
                            <returns>
                                    <return name="visible" type="Boolean" desc="lo stato di shadow casting dell'oggetto" />
                            </returns>
                    </language>
            </function>
     */
    int lightGetCastShadows(lua_State* luaRuntime)
        {
        bool active = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "light.getCastShadows", 1))
            {
            Ogre::Light* ogreLight = EOSG3D::getInstance()->getSceneManager()->getLight(luaL_checkstring(luaRuntime, 1));
            if (ogreLight != NULL)
                active = ogreLight->getCastShadows();
            }
        lua_pushboolean(luaRuntime, active);
        return 1;
        }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="materialCreate">
                    <language code="en">
                            <brief text="Creates a new Material" />
                            <description text="A Material encapsulates ALL aspects of the visual appearance of an object" />
                            <params>
                                    <param name="materialName" type="String" desc="name of the new Material to create" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Crea un nuovo Materiale" />
                            <description text="Un Materiale incapsula TUTTI gli aspetti sulla resa visiva di un oggetto" />
                            <params>
                                    <param name="materialName" type="String" desc="nome del nuovo Materiale da creare" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int materialCreate(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "material.create", 1))
            done = !Ogre::MaterialManager::getSingleton().create(luaL_checkstring(luaRuntime, 1), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).isNull();
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="materialSetAmbient">
                    <language code="en">
                            <brief text="Sets [ambient] value in a Pass contained in a Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="name of the affected Material" />
                                    <param name="techniqueIndex" type="Numeric" desc="index of the affected Technique (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="index of the affected Pass contained in the Technique (zero-based)" />
                                    <param name="redAmount"      type="Numeric" desc="Red component of the color (0.0 to 1.0)" />
                                    <param name="greenAmount"    type="Numeric" desc="Green component of the color (0.0 to 1.0)" />
                                    <param name="blueAmount"     type="Numeric" desc="Blue component of the color (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il valore [ambient] in una struttura Pass di un Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="nome del nuovo Materiale da creare" />
                                    <param name="techniqueIndex" type="Numeric" desc="indice della Technique interessata (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="indice della Pass contenuta nella Technique (zero-based)" />
                                    <param name="redAmount"      type="Numeric" desc="componente Rosso del colore (tra 0.0 e 1.0)" />
                                    <param name="greenAmount"    type="Numeric" desc="componente Verde del colore (tra 0.0 e 1.0)" />
                                    <param name="blueAmount"     type="Numeric" desc="componente Blu del colore (tra 0.0 e 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int materialSetAmbient(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "material.setAmbient", 6))
            {
            Ogre::Pass* materialPass = EOSG3D::getInstance()->getMaterialTechniquePass(luaL_checkstring(luaRuntime, 1), (unsigned short) lua_tonumber(luaRuntime, 2), (unsigned short) lua_tonumber(luaRuntime, 3), "materialSetAmbient");
            if (materialPass != NULL)
                {
                materialPass->setAmbient((Ogre::Real)lua_tonumber(luaRuntime, 4), (Ogre::Real)lua_tonumber(luaRuntime, 5), (Ogre::Real)lua_tonumber(luaRuntime, 6));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="materialGetAmbient">
                    <language code="en">
                            <brief text="Returns [ambient] value of a Pass contained in a Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="name of the affected Material" />
                                    <param name="techniqueIndex" type="Numeric" desc="index of the affected Technique (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="index of the affected Pass contained in the Technique (zero-based)" />
                            </params>
                            <returns>
                                    <return name="redAmount"      type="Numeric" desc="Red component of the color (0.0 to 1.0)" />
                                    <return name="greenAmount"    type="Numeric" desc="Green component of the color (0.0 to 1.0)" />
                                    <return name="blueAmount"     type="Numeric" desc="Blue component of the color (0.0 to 1.0)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il valore [ambient] in una struttura Pass di un Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="nome del nuovo Materiale da creare" />
                                    <param name="techniqueIndex" type="Numeric" desc="indice della Technique interessata (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="indice della Pass contenuta nella Technique (zero-based)" />
                            </params>
                            <returns>
                                    <return name="redAmount"      type="Numeric" desc="componente Rosso del colore (tra 0.0 e 1.0)" />
                                    <return name="greenAmount"    type="Numeric" desc="componente Verde del colore (tra 0.0 e 1.0)" />
                                    <return name="blueAmount"     type="Numeric" desc="componente Blu del colore (tra 0.0 e 1.0)" />
                            </returns>
                    </language>
            </function>
     */
    int materialGetAmbient(lua_State* luaRuntime)
        {
        Ogre::ColourValue colourValue = Ogre::ColourValue::White;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "material.getAmbient", 3))
            {
            Ogre::Pass* materialPass = EOSG3D::getInstance()->getMaterialTechniquePass(luaL_checkstring(luaRuntime, 1), (unsigned short) lua_tonumber(luaRuntime, 2), (unsigned short) lua_tonumber(luaRuntime, 3), "materialGetAmbient");
            if (materialPass != NULL)
                colourValue = materialPass->getAmbient();
            }
        lua_pushnumber(luaRuntime, colourValue.r);
        lua_pushnumber(luaRuntime, colourValue.g);
        lua_pushnumber(luaRuntime, colourValue.b);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="materialSetDiffuse">
                    <language code="en">
                            <brief text="Sets [diffuse] value in a Pass contained in a Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="name of the affected Material" />
                                    <param name="techniqueIndex" type="Numeric" desc="index of the affected Technique (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="index of the affected Pass contained in the Technique (zero-based)" />
                                    <param name="redAmount"      type="Numeric" desc="Red component of the color (0.0 to 1.0)" />
                                    <param name="greenAmount"    type="Numeric" desc="Green component of the color (0.0 to 1.0)" />
                                    <param name="blueAmount"     type="Numeric" desc="Blue component of the color (0.0 to 1.0)" />
                                    <param name="alphaAmount"    type="Numeric" desc="Alpha component of the color (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il valore [diffuse] in una struttura Pass di un Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="nome del nuovo Materiale da creare" />
                                    <param name="techniqueIndex" type="Numeric" desc="indice della Technique interessata (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="indice della Pass contenuta nella Technique (zero-based)" />
                                    <param name="redAmount"      type="Numeric" desc="componente Rosso del colore (tra 0.0 e 1.0)" />
                                    <param name="greenAmount"    type="Numeric" desc="componente Verde del colore (tra 0.0 e 1.0)" />
                                    <param name="blueAmount"     type="Numeric" desc="componente Blu del colore (tra 0.0 e 1.0)" />
                                    <param name="alphaAmount"    type="Numeric" desc="componente Alpha del colore (tra 0.0 e 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int materialSetDiffuse(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "material.setDiffuse", 7))
            {
            Ogre::Pass* materialPass = EOSG3D::getInstance()->getMaterialTechniquePass(luaL_checkstring(luaRuntime, 1), (unsigned short) lua_tonumber(luaRuntime, 2), (unsigned short) lua_tonumber(luaRuntime, 3), "materialSetDiffuse");
            if (materialPass != NULL)
                {
                materialPass->setDiffuse((Ogre::Real)lua_tonumber(luaRuntime, 4), (Ogre::Real)lua_tonumber(luaRuntime, 5), (Ogre::Real)lua_tonumber(luaRuntime, 6), (Ogre::Real)lua_tonumber(luaRuntime, 7));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="materialGetDiffuse">
                    <language code="en">
                            <brief text="Returns [diffuse] value of a Pass contained in a Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="name of the affected Material" />
                                    <param name="techniqueIndex" type="Numeric" desc="index of the affected Technique (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="index of the affected Pass contained in the Technique (zero-based)" />
                            </params>
                            <returns>
                                    <return name="redAmount"      type="Numeric" desc="Red component of the color (0.0 to 1.0)" />
                                    <return name="greenAmount"    type="Numeric" desc="Green component of the color (0.0 to 1.0)" />
                                    <return name="blueAmount"     type="Numeric" desc="Blue component of the color (0.0 to 1.0)" />
                                    <return name="alphaAmount"    type="Numeric" desc="Alpha component of the color (0.0 to 1.0)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il valore [diffuse] in una struttura Pass di un Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="nome del nuovo Materiale da creare" />
                                    <param name="techniqueIndex" type="Numeric" desc="indice della Technique interessata (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="indice della Pass contenuta nella Technique (zero-based)" />
                            </params>
                            <returns>
                                    <return name="redAmount"      type="Numeric" desc="componente Rosso del colore (tra 0.0 e 1.0)" />
                                    <return name="greenAmount"    type="Numeric" desc="componente Verde del colore (tra 0.0 e 1.0)" />
                                    <return name="blueAmount"     type="Numeric" desc="componente Blu del colore (tra 0.0 e 1.0)" />
                                    <return name="alphaAmount"    type="Numeric" desc="componente Alpha del colore (tra 0.0 e 1.0)" />
                            </returns>
                    </language>
            </function>
     */
    int materialGetDiffuse(lua_State* luaRuntime)
        {
        Ogre::ColourValue colourValue = Ogre::ColourValue::White;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "material.getDiffuse", 3))
            {
            Ogre::Pass* materialPass = EOSG3D::getInstance()->getMaterialTechniquePass(luaL_checkstring(luaRuntime, 1), (unsigned short) lua_tonumber(luaRuntime, 2), (unsigned short) lua_tonumber(luaRuntime, 3), "materialGetDiffuse");
            if (materialPass != NULL)
                colourValue = materialPass->getDiffuse();
            }
        lua_pushnumber(luaRuntime, colourValue.r);
        lua_pushnumber(luaRuntime, colourValue.g);
        lua_pushnumber(luaRuntime, colourValue.b);
        lua_pushnumber(luaRuntime, colourValue.a);
        return 4;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="materialSetSpecular">
                    <language code="en">
                            <brief text="Sets [specular] value in a Pass contained in a Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="name of the affected Material" />
                                    <param name="techniqueIndex" type="Numeric" desc="index of the affected Technique (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="index of the affected Pass contained in the Technique (zero-based)" />
                                    <param name="redAmount"      type="Numeric" desc="Red component of the color (0.0 to 1.0)" />
                                    <param name="greenAmount"    type="Numeric" desc="Green component of the color (0.0 to 1.0)" />
                                    <param name="blueAmount"     type="Numeric" desc="Blue component of the color (0.0 to 1.0)" />
                                    <param name="alphaAmount"    type="Numeric" desc="Alpha component of the color (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il valore [specular] in una struttura Pass di un Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="nome del nuovo Materiale da creare" />
                                    <param name="techniqueIndex" type="Numeric" desc="indice della Technique interessata (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="indice della Pass contenuta nella Technique (zero-based)" />
                                    <param name="redAmount"      type="Numeric" desc="componente Rosso del colore (tra 0.0 e 1.0)" />
                                    <param name="greenAmount"    type="Numeric" desc="componente Verde del colore (tra 0.0 e 1.0)" />
                                    <param name="blueAmount"     type="Numeric" desc="componente Blu del colore (tra 0.0 e 1.0)" />
                                    <param name="alphaAmount"    type="Numeric" desc="componente Alpha del colore (tra 0.0 e 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int materialSetSpecular(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "material.setSpecular", 7))
            {
            Ogre::Pass* materialPass = EOSG3D::getInstance()->getMaterialTechniquePass(luaL_checkstring(luaRuntime, 1), (unsigned short) lua_tonumber(luaRuntime, 2), (unsigned short) lua_tonumber(luaRuntime, 3), "materialSetSpecular");
            if (materialPass != NULL)
                {
                materialPass->setSpecular((Ogre::Real)lua_tonumber(luaRuntime, 4), (Ogre::Real)lua_tonumber(luaRuntime, 5), (Ogre::Real)lua_tonumber(luaRuntime, 6), (Ogre::Real)lua_tonumber(luaRuntime, 7));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="materialGetSpecular">
                    <language code="en">
                            <brief text="Returns [specular] value in a Pass contained in a Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="name of the affected Material" />
                                    <param name="techniqueIndex" type="Numeric" desc="index of the affected Technique (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="index of the affected Pass contained in the Technique (zero-based)" />
                            </params>
                            <returns>
                                    <return name="redAmount"      type="Numeric" desc="Red component of the color (0.0 to 1.0)" />
                                    <return name="greenAmount"    type="Numeric" desc="Green component of the color (0.0 to 1.0)" />
                                    <return name="blueAmount"     type="Numeric" desc="Blue component of the color (0.0 to 1.0)" />
                                    <return name="alphaAmount"    type="Numeric" desc="Alpha component of the color (0.0 to 1.0)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il valore [specular] in una struttura Pass di un Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="nome del nuovo Materiale da creare" />
                                    <param name="techniqueIndex" type="Numeric" desc="indice della Technique interessata (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="indice della Pass contenuta nella Technique (zero-based)" />
                            </params>
                            <returns>
                                    <return name="redAmount"      type="Numeric" desc="componente Rosso del colore (tra 0.0 e 1.0)" />
                                    <return name="greenAmount"    type="Numeric" desc="componente Verde del colore (tra 0.0 e 1.0)" />
                                    <return name="blueAmount"     type="Numeric" desc="componente Blu del colore (tra 0.0 e 1.0)" />
                                    <return name="alphaAmount"    type="Numeric" desc="componente Alpha del colore (tra 0.0 e 1.0)" />
                            </returns>
                    </language>
            </function>
     */
    int materialGetSpecular(lua_State* luaRuntime)
        {
        Ogre::ColourValue colourValue = Ogre::ColourValue::White;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "material.getSpecular", 3))
            {
            Ogre::Pass* materialPass = EOSG3D::getInstance()->getMaterialTechniquePass(luaL_checkstring(luaRuntime, 1), (unsigned short) lua_tonumber(luaRuntime, 2), (unsigned short) lua_tonumber(luaRuntime, 3), "materialGetSpecular");
            if (materialPass != NULL)
                colourValue = materialPass->getSpecular();
            }
        lua_pushnumber(luaRuntime, colourValue.r);
        lua_pushnumber(luaRuntime, colourValue.g);
        lua_pushnumber(luaRuntime, colourValue.b);
        lua_pushnumber(luaRuntime, colourValue.a);
        return 4;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="materialSetSEOSIllumination">
                    <language code="en">
                            <brief text="Sets [sEOSIllumination] value of a Pass contained in a Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="name of the affected Material" />
                                    <param name="techniqueIndex" type="Numeric" desc="index of the affected Technique (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="index of the affected Pass contained in the Technique (zero-based)" />
                                    <param name="redAmount"      type="Numeric" desc="Red component of the color (0.0 to 1.0)" />
                                    <param name="greenAmount"    type="Numeric" desc="Green component of the color (0.0 to 1.0)" />
                                    <param name="blueAmount"     type="Numeric" desc="Blue component of the color (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il valore [specular] in una struttura Pass di un Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="nome del nuovo Materiale da creare" />
                                    <param name="techniqueIndex" type="Numeric" desc="indice della Technique interessata (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="indice della Pass contenuta nella Technique (zero-based)" />
                                    <param name="redAmount"      type="Numeric" desc="componente Rosso del colore (tra 0.0 e 1.0)" />
                                    <param name="greenAmount"    type="Numeric" desc="componente Verde del colore (tra 0.0 e 1.0)" />
                                    <param name="blueAmount"     type="Numeric" desc="componente Blu del colore (tra 0.0 e 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int materialSetSelfIllumination(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "material.setSelfIllumination", 6))
            {
            Ogre::Pass* materialPass = EOSG3D::getInstance()->getMaterialTechniquePass(luaL_checkstring(luaRuntime, 1), (unsigned short) lua_tonumber(luaRuntime, 2), (unsigned short) lua_tonumber(luaRuntime, 3), "materialSetSEOSIllumination");
            if (materialPass != NULL)
                {
                materialPass->setSelfIllumination((Ogre::Real)lua_tonumber(luaRuntime, 4), (Ogre::Real)lua_tonumber(luaRuntime, 5), (Ogre::Real)lua_tonumber(luaRuntime, 6));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="materialGetSEOSIllumination">
                    <language code="en">
                            <brief text="Returns [sEOSIllumination] value in a Pass contained in a Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="name of the affected Material" />
                                    <param name="techniqueIndex" type="Numeric" desc="index of the affected Technique (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="index of the affected Pass contained in the Technique (zero-based)" />
                            </params>
                            <returns>
                                    <return name="redAmount"      type="Numeric" desc="Red component of the color (0.0 to 1.0)" />
                                    <return name="greenAmount"    type="Numeric" desc="Green component of the color (0.0 to 1.0)" />
                                    <return name="blueAmount"     type="Numeric" desc="Blue component of the color (0.0 to 1.0)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il valore [sEOSIllumination] in una struttura Pass di un Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="nome del nuovo Materiale da creare" />
                                    <param name="techniqueIndex" type="Numeric" desc="indice della Technique interessata (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="indice della Pass contenuta nella Technique (zero-based)" />
                            </params>
                            <returns>
                                    <return name="redAmount"      type="Numeric" desc="componente Rosso del colore (tra 0.0 e 1.0)" />
                                    <return name="greenAmount"    type="Numeric" desc="componente Verde del colore (tra 0.0 e 1.0)" />
                                    <return name="blueAmount"     type="Numeric" desc="componente Blu del colore (tra 0.0 e 1.0)" />
                            </returns>
                    </language>
            </function>
     */
    int materialGetSelfIllumination(lua_State* luaRuntime)
        {
        Ogre::ColourValue colourValue = Ogre::ColourValue::White;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "material.setSelfIllumination", 3))
            {
            Ogre::Pass* materialPass = EOSG3D::getInstance()->getMaterialTechniquePass(luaL_checkstring(luaRuntime, 1), (unsigned short) lua_tonumber(luaRuntime, 2), (unsigned short) lua_tonumber(luaRuntime, 3), "materialGetSEOSIllumination");
            if (materialPass != NULL)
                colourValue = materialPass->getSelfIllumination();
            }
        lua_pushnumber(luaRuntime, colourValue.r);
        lua_pushnumber(luaRuntime, colourValue.g);
        lua_pushnumber(luaRuntime, colourValue.b);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="materialSetShininess">
                    <language code="en">
                            <brief text="Sets [shininess] value of a Pass contained in a Material (affects the size of specular highlights)" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="name of the affected Material" />
                                    <param name="techniqueIndex" type="Numeric" desc="index of the affected Technique (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="index of the affected Pass contained in the Technique (zero-based)" />
                                    <param name="amount"         type="Numeric" desc="Shininess amount value (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il valore [specular] in una struttura Pass di un Material (agisce sulla dimensione delle aree speculari illuminate)" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="nome del nuovo Materiale da creare" />
                                    <param name="techniqueIndex" type="Numeric" desc="indice della Technique interessata (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="indice della Pass contenuta nella Technique (zero-based)" />
                                    <param name="amount"         type="Numeric" desc="fattore di Shininess (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int materialSetShininess(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "material.setShininess", 4))
            {
            Ogre::Pass* materialPass = EOSG3D::getInstance()->getMaterialTechniquePass(luaL_checkstring(luaRuntime, 1), (unsigned short) lua_tonumber(luaRuntime, 2), (unsigned short) lua_tonumber(luaRuntime, 3), "materialSetShininess");
            if (materialPass != NULL)
                {
                materialPass->setShininess((Ogre::Real)lua_tonumber(luaRuntime, 4));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="materialGetShininess">
                    <language code="en">
                            <brief text="Returns [shininess] value in a Pass contained in a Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="name of the affected Material" />
                                    <param name="techniqueIndex" type="Numeric" desc="index of the affected Technique (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="index of the affected Pass contained in the Technique (zero-based)" />
                            </params>
                            <returns>
                                    <return name="amount" type="Numeric" desc="shininess amount of the pass (0.0 to 1.0)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il valore [shininess] in una struttura Pass di un Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="nome del nuovo Materiale da creare" />
                                    <param name="techniqueIndex" type="Numeric" desc="indice della Technique interessata (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="indice della Pass contenuta nella Technique (zero-based)" />
                            </params>
                            <returns>
                                    <return name="amount" type="Numeric" desc="valore shininess della Pass (0.0 to 1.0)" />
                            </returns>
                    </language>
            </function>
     */
    int materialGetShininess(lua_State* luaRuntime)
        {
        Ogre::Real value = 0.0f;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "material.getShininess", 3))
            {
            Ogre::Pass* materialPass = EOSG3D::getInstance()->getMaterialTechniquePass(luaL_checkstring(luaRuntime, 1), (unsigned short) lua_tonumber(luaRuntime, 2), (unsigned short) lua_tonumber(luaRuntime, 3), "materialGetSEOSIllumination");
            if (materialPass != NULL)
                value = materialPass->getShininess();
            }
        lua_pushnumber(luaRuntime, value);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /*
            <function name="materialSetEmissive">
                    <language code="en">
                            <brief text="Sets [emissive] value in a Pass contained in a Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="name of the affected Material" />
                                    <param name="techniqueIndex" type="Numeric" desc="index of the affected Technique (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="index of the affected Pass contained in the Technique (zero-based)" />
                                    <param name="redAmount"      type="Numeric" desc="Red component of the color (0.0 to 1.0)" />
                                    <param name="greenAmount"    type="Numeric" desc="Green component of the color (0.0 to 1.0)" />
                                    <param name="blueAmount"     type="Numeric" desc="Blue component of the color (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il valore [emissive] in una struttura Pass di un Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"   type="String" desc="nome del nuovo Materiale da creare" />
                                    <param name="techniqueIndex" type="Numeric" desc="indice della Technique interessata (zero-based)" />
                                    <param name="passIndex"      type="Numeric" desc="indice della Pass contenuta nella Technique (zero-based)" />
                                    <param name="redAmount"      type="Numeric" desc="componente Rosso del colore (tra 0.0 e 1.0)" />
                                    <param name="greenAmount"    type="Numeric" desc="componente Verde del colore (tra 0.0 e 1.0)" />
                                    <param name="blueAmount"     type="Numeric" desc="componente Blu del colore (tra 0.0 e 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    /*int materialSetEmissive(lua_State* luaRuntime)
            {
            bool done = false;
            if(checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, 6, "materialSetEmissive"))
                    {
                    Ogre::Pass* materialPass = EOSG3D::getInstance()->getMaterialTechniquePass(luaL_checkstring(luaRuntime, 1), (unsigned short)lua_tonumber(luaRuntime, 2), (unsigned short)lua_tonumber(luaRuntime, 3), "materialSetEmissive");
                    if(materialPass != NULL)
                            {
                            materialPass->setEmissive((Ogre::Real)lua_tonumber(luaRuntime, 4), (Ogre::Real)lua_tonumber(luaRuntime, 5), (Ogre::Real)lua_tonumber(luaRuntime, 6));
                            done = true;
                            }
                    }
            lua_pushboolean(luaRuntime, done);
            return 1;
            }*/

    // -----------------------------------------------------------------------------

    /**
            <function name="materialSetAlphaOperation">
                    <language code="en">
                            <brief text="Sets [emissive] value in a Pass contained in a Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"     type="String" desc="name of the affected Material" />
                                    <param name="techniqueIndex"   type="Numeric" desc="index of the affected Technique (zero-based)" />
                                    <param name="passIndex"        type="Numeric" desc="index of the affected Pass contained in the Technique (zero-based)" />
                                    <param name="textureUnitIndex" type="Numeric" desc="index of the affected TextureUnit contained in the Pass (zero-based)" />
                                    <param name="layerBlendOperation" type="Constant" desc="The operation to be used, e.g. modulate (multiply), add, subtract: a value in eos.g3d.[ LBX_SOURCE1 | LBX_SOURCE2 | LBX_MODULATE | LBX_MODULATE_X2 | LBX_MODULATE_X4 | LBX_ADD | LBX_ADD_SIGNED | LBX_ADD | LBX_ADD_SMOOTH | LBX_SUBTRACT | LBX_BLEND_DIFFUSE_ALPHA | LBX_BLEND_TEXTURE_ALPHA | LBX_BLEND_CURRENT_ALPHA | LBX_BLEND_DIFFUSE_ALPHA | LBX_BLEND_MANUAL | LBX_DOTPRODUCT | LBX_BLEND_DIFFUSE_COLOUR ]" />
                                    <param name="layerBlendSource1" type="Constant" desc="The source of the first alpha value to the operation (e.g. texture alpha): a value in eos.g3d.[ LBS_CURRENT | LBS_TEXTURE | LBS_DIFFUSE | LBS_SPECULAR | LBS_MANUAL ]" />
                                    <param name="layerBlendSource2" type="Constant" desc="The source of the second alpha value to the operation (e.g. current surface alpha): a value in eos.g3d.[ LBS_CURRENT | LBS_TEXTURE | LBS_DIFFUSE | LBS_SPECULAR | LBS_MANUAL ]" />
                                    <param name="alphaValue1" type="Numeric" desc="Manually supplied alpha value (important only if layerBlendSource1 = LBS_MANUAL)" />
                                    <param name="alphaValue2" type="Numeric" desc="Manually supplied alpha value (important only if layerBlendSource2 = LBS_MANUAL)" />
                                    <param name="manualBlend" type="Numeric" desc="Manually supplied blend value - important only for operations which require manual blend e.g. LBX_BLEND_MANUAL" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il valore [emissive] in una struttura Pass di un Material" />
                            <description text="" />
                            <params>
                                    <param name="materialName"     type="String" desc="nome del Material" />
                                    <param name="techniqueIndex"   type="Numeric" desc="indice della Technique (zero-based)" />
                                    <param name="passIndex"        type="Numeric" desc="indice della Pass contenuta nella Technique (zero-based)" />
                                    <param name="textureUnitIndex" type="Numeric" desc="indice della TextureUnit contenuta nella Pass (zero-based)" />
                                    <param name="layerBlendOperation" type="Constant" desc="l'operazione da usare, ad es. modulate (multiply), add, subtract: un valore compreso in eos.g3d.[ LBX_SOURCE1 | LBX_SOURCE2 | LBX_MODULATE | LBX_MODULATE_X2 | LBX_MODULATE_X4 | LBX_ADD | LBX_ADD_SIGNED | LBX_ADD | LBX_ADD_SMOOTH | LBX_SUBTRACT | LBX_BLEND_DIFFUSE_ALPHA | LBX_BLEND_TEXTURE_ALPHA | LBX_BLEND_CURRENT_ALPHA | LBX_BLEND_DIFFUSE_ALPHA | LBX_BLEND_MANUAL | LBX_DOTPRODUCT | LBX_BLEND_DIFFUSE_COLOUR ]" />
                                    <param name="layerBlendSource1" type="Constant" desc="la sorgente del primo valore alpha dell'operazione (ad es. texture alpha): un valore compreso in eos.g3d.[ LBS_CURRENT | LBS_TEXTURE | LBS_DIFFUSE | LBS_SPECULAR | LBS_MANUAL ]" />
                                    <param name="layerBlendSource2" type="Constant" desc="la sorgente del secondo valore alpha dell'operazione (ad es. current surface alpha): un valore compreso in eos.g3d.[ LBS_CURRENT | LBS_TEXTURE | LBS_DIFFUSE | LBS_SPECULAR | LBS_MANUAL ]" />
                                    <param name="alphaValue1" type="Numeric" desc="valore alpha fornito manualmente (importante solo se layerBlendSource1 = LBS_MANUAL)" />
                                    <param name="alphaValue2" type="Numeric" desc="valore alpha fornito manualmente (importante solo se layerBlendSource2 = LBS_MANUAL)" />
                                    <param name="manualBlend" type="Numeric" desc="valore blend fornito manualmente - importante solo per operazioni che richiedono il blend manuale, come LBX_BLEND_MANUAL" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int materialSetAlphaOperation(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "material.setAlphaOperation", 10))
            {
            Ogre::TextureUnitState* textureUnitState = EOSG3D::getInstance()->getMaterialTechniquePassTextureUnit(luaL_checkstring(luaRuntime, 1), (unsigned short) lua_tonumber(luaRuntime, 2), (unsigned short) lua_tonumber(luaRuntime, 3), (unsigned short) lua_tonumber(luaRuntime, 4), "materialSetAlphaOperation");
            if (textureUnitState != NULL)
                {
                Ogre::LayerBlendOperationEx operation;
                switch ((int) lua_tonumber(luaRuntime, 5))
                    {
                    case _LBX_SOURCE1:
                        operation = Ogre::LBX_SOURCE1;
                        break;
                    case _LBX_SOURCE2:
                        operation = Ogre::LBX_SOURCE2;
                        break;
                    case _LBX_MODULATE:
                        operation = Ogre::LBX_MODULATE;
                        break;
                    case _LBX_MODULATE_X2:
                        operation = Ogre::LBX_MODULATE_X2;
                        break;
                    case _LBX_MODULATE_X4:
                        operation = Ogre::LBX_MODULATE_X4;
                        break;
                    case _LBX_ADD:
                        operation = Ogre::LBX_ADD;
                        break;
                    case _LBX_ADD_SIGNED:
                        operation = Ogre::LBX_ADD_SIGNED;
                        break;
                    case _LBX_ADD_SMOOTH:
                        operation = Ogre::LBX_ADD_SMOOTH;
                        break;
                    case _LBX_SUBTRACT:
                        operation = Ogre::LBX_SUBTRACT;
                        break;
                    case _LBX_BLEND_DIFFUSE_ALPHA:
                        operation = Ogre::LBX_BLEND_DIFFUSE_ALPHA;
                        break;
                    case _LBX_BLEND_TEXTURE_ALPHA:
                        operation = Ogre::LBX_BLEND_TEXTURE_ALPHA;
                        break;
                    case _LBX_BLEND_CURRENT_ALPHA:
                        operation = Ogre::LBX_BLEND_CURRENT_ALPHA;
                        break;
                    case _LBX_BLEND_MANUAL:
                        operation = Ogre::LBX_BLEND_MANUAL;
                        break;
                    case _LBX_DOTPRODUCT:
                        operation = Ogre::LBX_DOTPRODUCT;
                        break;
                    case _LBX_BLEND_DIFFUSE_COLOUR:
                        operation = Ogre::LBX_BLEND_DIFFUSE_COLOUR;
                        break;
                    }
                //
                Ogre::LayerBlendSource source1;
                switch ((int) lua_tonumber(luaRuntime, 6))
                    {
                    case _LBS_CURRENT:
                        source1 = Ogre::LBS_CURRENT;
                        break;
                    case _LBS_TEXTURE:
                        source1 = Ogre::LBS_TEXTURE;
                        break;
                    case _LBS_DIFFUSE:
                        source1 = Ogre::LBS_DIFFUSE;
                        break;
                    case _LBS_SPECULAR:
                        source1 = Ogre::LBS_SPECULAR;
                        break;
                    case _LBS_MANUAL:
                        source1 = Ogre::LBS_MANUAL;
                        break;
                    }
                //
                Ogre::LayerBlendSource source2;
                switch ((int) lua_tonumber(luaRuntime, 7))
                    {
                    case _LBS_CURRENT:
                        source2 = Ogre::LBS_CURRENT;
                        break;
                    case _LBS_TEXTURE:
                        source2 = Ogre::LBS_TEXTURE;
                        break;
                    case _LBS_DIFFUSE:
                        source2 = Ogre::LBS_DIFFUSE;
                        break;
                    case _LBS_SPECULAR:
                        source2 = Ogre::LBS_SPECULAR;
                        break;
                    case _LBS_MANUAL:
                        source2 = Ogre::LBS_MANUAL;
                        break;
                    }
                //
                textureUnitState->setAlphaOperation(operation, source1, source2, (Ogre::Real)lua_tonumber(luaRuntime, 8), (Ogre::Real)lua_tonumber(luaRuntime, 9), (Ogre::Real)lua_tonumber(luaRuntime, 10));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="ribbonTrailCreate">
                    <language code="en">
                            <brief text="Creates a new RibbonTrail object" />
                            <description text="A RibbonTrail is an object which automatically leaves a trail behind one or more Node instances." />
                            <params>
                                    <param name="objectName" type="String" desc="name of the new RibbonTrail object to create" />
                            </params>
                            <returns>
                                    <return name="sceneNodeName" type="String" desc="name of the scene node where new object is attached, nil in case of errors" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Crea un nuovo oggetto RibbonTrail" />
                            <description text="Un RibbonTrail e' un oggetto che lascia una scia dietro uno o pi SceneNode." />
                            <params>
                                    <param name="objectName" type="String" desc="nome del nuovo oggetto RibbonTrail da creare" />
                            </params>
                            <returns>
                                    <return name="sceneNodeName" type="String" desc="nome dello SceneNode al quale e' stato agganciato il nuovo oggetto RibbonTrail, nil in caso di errore" />
                            </returns>
                    </language>
            </function>
     */
    int ribbonTrailCreate(lua_State* luaRuntime)
        {
        const char* sceneNodeName = NULL;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "ribbonTrail.create", 1))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            if (EOSG3D::getInstance()->objectByName(objectName, NULL) == NULL) // checks if object already exists
                {
                EOSOgreObject* newOgreObject = EOSG3D::getInstance()->createEmptyObject(objectName);
                if (newOgreObject != NULL)
                    {
                    //newOgreObject->requestSceneNode(NULL);
                    newOgreObject->cachedSceneNode = EOSG3D::getInstance()->requestCachedSceneNode(NULL);
                    if (newOgreObject->cachedSceneNode != NULL)
                        {
                        Ogre::RibbonTrail* trail = EOSG3D::getInstance()->getSceneManager()->createRibbonTrail(objectName);
                        if (trail != NULL)
                            {
                            newOgreObject->attachRibbonTrail(trail);
                            newOgreObject->cachedSceneNode->sceneNode->attachObject(trail);
                            sceneNodeName = newOgreObject->cachedSceneNode->sceneNode->getName().c_str();
                            }
                        else
                            {
                            LOGERROR("[eos."_MODULE_NAME"][ribbonTrail.create()] - unable to create RibbonTrail '%s'.", objectName);
                            }
                        }
                    }
                }
            }
        lua_pushstring(luaRuntime, sceneNodeName);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="ribbonTrailSetMaterial">
                    <language code="en">
                            <brief text="Assigns a material to a RibbonTrail object" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the affected RibbonTrail object" />
                                    <param name="materialName" type="String" desc="name of the material to assign to the RibbonTrail object" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Assegna un material ad un oggetto RibbonTrail" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto RibbonTrail interessato" />
                                    <param name="materialName" type="String" desc="nome del material da assegnare all'oggetto RibbonTrail" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int ribbonTrailSetMaterial(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "ribbonTrail.setMaterial", 2))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* ribbonTrailObject = EOSG3D::getInstance()->objectByName(objectName, "ribbonTrail.setMaterial");
            if (ribbonTrailObject != NULL)
                {
                if (ribbonTrailObject->getType() == EOSOgreObject::RibbonTrailObjectType)
                    {
                    ribbonTrailObject->getRibbonTrail()->setMaterialName(luaL_checkstring(luaRuntime, 2));
                    done = true;
                    }
                else
                    LOGERROR("[eos."_MODULE_NAME"][ribbonTrail.setMaterial()] - error: object named '%s' is not of RibbonTrail type.", objectName);
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="ribbonTrailAddNode">
                    <language code="en">
                            <brief text="Adds to a RibbonTrail object a SceneNode to follow" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the affected RibbonTrail object" />
                                    <param name="sceneNodeName" type="String" desc="name of the SceneNode to follow" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Aggiunge ad un oggetto RibbonTrail uno SceneNode da seguire" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto RibbonTrail interessato" />
                                    <param name="sceneNodeName" type="String" desc="name of the SceneNode to follow" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int ribbonTrailAddNode(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "ribbonTrail.addNode", 2))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* ribbonTrailObject = EOSG3D::getInstance()->objectByName(objectName, "ribbonTrail.addNode");
            if (ribbonTrailObject != NULL)
                {
                if (ribbonTrailObject->getType() == EOSOgreObject::RibbonTrailObjectType)
                    {
                    Ogre::SceneNode* theNode = EOSG3D::getInstance()->nodeByName(luaL_checkstring(luaRuntime, 2), "ribbonTrail.addNode");
                    if (theNode != NULL)
                        {
                        ribbonTrailObject->getRibbonTrail()->addNode(theNode);
                        done = true;
                        }
                    }
                else
                    LOGERROR("[eos."_MODULE_NAME"][ribbonTrail.addNode()] - error: object named '%s' is not of RibbonTrail type.", objectName);
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="ribbonTrailSetLength">
                    <language code="en">
                            <brief text="Sets length of the trail of a RibbonTrail object" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the affected RibbonTrail object" />
                                    <param name="trailLength" type="Numeric" desc="length of the trail, in world units" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la lunghezza della scia di un oggetto RibbonTrail" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto RibbonTrail interessato" />
                                    <param name="trailLength" type="Numeric" desc="lunghezza della scia, in unita' world" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int ribbonTrailSetLength(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "ribbonTrail.setLength", 2))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* ribbonTrailObject = EOSG3D::getInstance()->objectByName(objectName, "ribbonTrail.setLength");
            if (ribbonTrailObject != NULL)
                {
                if (ribbonTrailObject->getType() == EOSOgreObject::RibbonTrailObjectType)
                    {
                    ribbonTrailObject->getRibbonTrail()->setTrailLength((Ogre::Real)lua_tonumber(luaRuntime, 2));
                    done = true;
                    }
                else
                    LOGERROR("[eos."_MODULE_NAME"][ribbonTrail.setLength()] - error: object named '%s' is not of RibbonTrail type.", objectName);
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="ribbonTrailSetNumberOfChains">
                    <language code="en">
                            <brief text="Sets number of chains of a RibbonTrail object" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the affected RibbonTrail object" />
                                    <param name="trailLength" type="Numeric" desc="number of chains to set" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il numero di catene di un oggetto RibbonTrail" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto RibbonTrail interessato" />
                                    <param name="trailLength" type="Numeric" desc="numero delle catene da settare" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int ribbonTrailSetNumberOfChains(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "ribbonTrail.setNumberOfChains", 2))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* ribbonTrailObject = EOSG3D::getInstance()->objectByName(objectName, "ribbonTrail.setNumberOfChains");
            if (ribbonTrailObject != NULL)
                {
                if (ribbonTrailObject->getType() == EOSOgreObject::RibbonTrailObjectType)
                    {
                    ribbonTrailObject->getRibbonTrail()->setNumberOfChains((unsigned int) lua_tonumber(luaRuntime, 2));
                    done = true;
                    }
                else
                    LOGERROR("[eos."_MODULE_NAME"][ribbonTrail.setNumberOfChains()] - error: object named '%s' is not of RibbonTrail type.", objectName);
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="ribbonTrailSetMaxChainElements">
                    <language code="en">
                            <brief text="Sets maximum number of elements per chains that a RibbonTrail object can have" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the affected RibbonTrail object" />
                                    <param name="maxElements" type="Numeric" desc="maximum number of elements per chains to set" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il massimo numero di elementi per catena in un oggetto RibbonTrail" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto RibbonTrail interessato" />
                                    <param name="maxElements" type="Numeric" desc="massimo numero di elementi per catena da settare" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int ribbonTrailSetMaxChainElements(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "ribbonTrail.setMaxChainElements", 2))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* ribbonTrailObject = EOSG3D::getInstance()->objectByName(objectName, "ribbonTrail.setMaxChainElements");
            if (ribbonTrailObject != NULL)
                {
                if (ribbonTrailObject->getType() == EOSOgreObject::RibbonTrailObjectType)
                    {
                    ribbonTrailObject->getRibbonTrail()->setMaxChainElements((unsigned int) lua_tonumber(luaRuntime, 2));
                    done = true;
                    }
                else
                    LOGERROR("[eos."_MODULE_NAME"][ribbonTrail.setMaxChainElements()] - error: object named '%s' is not of RibbonTrail type.", objectName);
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="ribbonTrailSetInitialColor">
                    <language code="en">
                            <brief text="Sets initial color of a chain in a RibbonTrail object" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="name of the affected RibbonTrail object" />
                                    <param name="chainIndex" type="Numeric" desc="index of affected chain" />
                                    <param name="colorRed"   type="Numeric" desc="Red component of the color to set (0.0 to 1.0)" />
                                    <param name="colorGreen" type="Numeric" desc="Green component of the color to set (0.0 to 1.0)" />
                                    <param name="colorBlue"  type="Numeric" desc="Blue component of the color to set (0.0 to 1.0)" />
                                    <param name="alpha"      type="Numeric" desc="Alpha (transparency) value to set (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il colore iniziale di una catena in un oggetto RibbonTrail" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto RibbonTrail interessato" />
                                    <param name="chainIndex" type="Numeric" desc="indice della catena da modificare" />
                                    <param name="colorRed"   type="Numeric" desc="componente Rosso del colore da settare (tra 0.0 e 1.0)" />
                                    <param name="colorGreen" type="Numeric" desc="componente Verde del colore da settare (tra 0.0 e 1.0)" />
                                    <param name="colorBlue"  type="Numeric" desc="componente Blu del colore da settare (tra 0.0 e 1.0)" />
                                    <param name="alpha"      type="Numeric" desc="valore Alpha (trasparenza) da settare (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int ribbonTrailSetInitialColor(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "ribbonTrail.setInitialColor", 6))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* ribbonTrailObject = EOSG3D::getInstance()->objectByName(objectName, "ribbonTrail.setInitialColor");
            if (ribbonTrailObject != NULL)
                {
                if (ribbonTrailObject->getType() == EOSOgreObject::RibbonTrailObjectType)
                    {
                    ribbonTrailObject->getRibbonTrail()->setInitialColour((unsigned int) lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4), (Ogre::Real)lua_tonumber(luaRuntime, 5), (Ogre::Real)lua_tonumber(luaRuntime, 6));
                    done = true;
                    }
                else
                    LOGERROR("[eos."_MODULE_NAME"][ribbonTrail.setInitialColor()] - error: object named '%s' is not of RibbonTrail type.", objectName);
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="ribbonTrailSetColorChange">
                    <language code="en">
                            <brief text="Enables/disables fading a chain of a RibbonTrail object using an absolute colur value" />
                            <description text="" />
                            <params>
                                    <param name="objectName"    type="String" desc="name of the affected RibbonTrail object" />
                                    <param name="chainIndex"    type="Numeric" desc="index of affected chain" />
                                    <param name="redSubtract"   type="Numeric" desc="the amount to subtract from Red color channel for each second (0.0 to 1.0)" />
                                    <param name="greenSubtract" type="Numeric" desc="the amount to subtract from Green color channel for each second (0.0 to 1.0)" />
                                    <param name="blueSubtract"  type="Numeric" desc="the amount to subtract from Blue color channel for each second (0.0 to 1.0)" />
                                    <param name="alpha"         type="Numeric" desc="the amount to subtract from Alpha (transparency) channel for each second (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Abilita/disabilita la sfumatura di una catena di un oggetto RibbonTrail usando un colore assoluto" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="nome dell'oggetto RibbonTrail interessato" />
                                    <param name="chainIndex" type="Numeric" desc="indice della catena da modificare" />
                                    <param name="redSubtract"   type="Numeric" desc="la quantita' da sottrarre dal canale Rosso per ogni secondo (0.0 to 1.0)" />
                                    <param name="greenSubtract" type="Numeric" desc="la quantita' da sottrarre dal canale Verde per ogni secondo (0.0 to 1.0)" />
                                    <param name="blueSubtract"  type="Numeric" desc="la quantita' da sottrarre dal canale Blu per ogni secondo (0.0 to 1.0)" />
                                    <param name="alpha"         type="Numeric" desc="la quantita' da sottrarre dal canale Alpha (trasparenza) per ogni secondo (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int ribbonTrailSetColorChange(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "ribbonTrail.setColorChange", 6))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* ribbonTrailObject = EOSG3D::getInstance()->objectByName(objectName, "ribbonTrail.setColorChange");
            if (ribbonTrailObject != NULL)
                {
                if (ribbonTrailObject->getType() == EOSOgreObject::RibbonTrailObjectType)
                    {
                    ribbonTrailObject->getRibbonTrail()->setColourChange((unsigned int) lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3), (Ogre::Real)lua_tonumber(luaRuntime, 4), (Ogre::Real)lua_tonumber(luaRuntime, 5), (Ogre::Real)lua_tonumber(luaRuntime, 6));
                    done = true;
                    }
                else
                    LOGERROR("[eos."_MODULE_NAME"][ribbonTrail.setColorChange()] - error: object named '%s' is not of RibbonTrail type.", objectName);
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="ribbonTrailSetInitialWidth">
                    <language code="en">
                            <brief text="Set the starting width (in world units) of a chain in a RibbonTrail object" />
                            <description text="" />
                            <params>
                                    <param name="objectName"   type="String"  desc="name of the affected RibbonTrail object" />
                                    <param name="chainIndex"   type="Numeric" desc="index of affected chain" />
                                    <param name="initialWidth" type="Numeric" desc="the initial width of the affected chain" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la dimensione iniziale (in punti world) di una catena in un oggetto RibbonTrail" />
                            <description text="" />
                            <params>
                                    <param name="objectName"   type="String"  desc="nome dell'oggetto RibbonTrail interessato" />
                                    <param name="chainIndex"   type="Numeric" desc="indice della catena da modificare" />
                                    <param name="initialWidth" type="Numeric" desc="la dimensione iniziale della catena da modificare" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int ribbonTrailSetInitialWidth(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "ribbonTrail.setInitialWidth", 3))
            {
            const char* objectName = luaL_checkstring(luaRuntime, 1);
            EOSOgreObject* ribbonTrailObject = EOSG3D::getInstance()->objectByName(objectName, "ribbonTrail.setInitialWidth");
            if (ribbonTrailObject != NULL)
                {
                if (ribbonTrailObject->getType() == EOSOgreObject::RibbonTrailObjectType)
                    {
                    ribbonTrailObject->getRibbonTrail()->setInitialWidth((unsigned int) lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3));
                    done = true;
                    }
                else
                    LOGERROR("[eos."_MODULE_NAME"][ribbonTrail.setInitialWidth()] - error: object named '%s' is not of RibbonTrail type.", objectName);
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="pathCreate">
                    <language code="en">
                            <brief text="Creates a new Path object" />
                            <description text="A path is a collection of 'tracks' which over time change the position / orientation of Node objects (e.g. to make objects move along a path)." />
                            <params>
                                    <param name="lightName" type="String" desc="name of the new Path to create" />
                                    <param name="duration" type="Numeric" desc="duration in seconds" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Crea un nuovo oggetto Path" />
                            <description text="Un path e' un insieme di 'tracks' le quali cambiano posizione e orientamento di oggetti Node in base al tempo (ad esempio, per fare in modo da far muovere oggetti lungo un percorso)." />
                            <params>
                                    <param name="lightName" type="String" desc="nome del nuovo oggetto Path da creare" />
                                    <param name="duration" type="Numeric" desc="durata in secondi" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int pathCreate(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "path.create", 2))
            {
            // Creates the animation
            std::string pathName = luaL_checkstring(luaRuntime, 1);
            Ogre::Animation* ogreAnim = EOSG3D::getInstance()->getSceneManager()->createAnimation(pathName.c_str(), (Ogre::Real)lua_tonumber(luaRuntime, 2));
            if (ogreAnim != NULL)
                {
                Ogre::AnimationState* animState = EOSG3D::getInstance()->getSceneManager()->createAnimationState(pathName.c_str());
                animState->setEnabled(true);
                EOSG3D::getInstance()->animStates.push_back(animState);
                done = true;
                }
            else
                LOGERROR("[eos."_MODULE_NAME"][path.create()] - unable to create path '%s'.", pathName.c_str());
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="pathSetInterpolationLinear">
                    <language code="en">
                            <brief text="Sets linear interpolation mode for a Path object" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Path object" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta l'interpolazione di un oggetto Path in modalita' lineare" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Path" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int pathSetInterpolationLinear(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "path.setInterpolationLinear", 1))
            {
            // gets the animation
            std::string pathName = luaL_checkstring(luaRuntime, 1);
            Ogre::Animation* ogreAnim = EOSG3D::getInstance()->getSceneManager()->getAnimation(pathName.c_str());
            if (ogreAnim != NULL)
                {
                ogreAnim->setInterpolationMode(Ogre::Animation::IM_LINEAR);
                done = true;
                }
            else
                LOGERROR("[eos."_MODULE_NAME"][path.setInterpolationLinear()] - unable to find path '%s'.", pathName.c_str());
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="pathSetInterpolationSpline">
                    <language code="en">
                            <brief text="Sets curve (spline) interpolation mode for a Path object" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="name of the Path object" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta l'interpolazione di un oggetto Path in modalita' curvilinea (spline)" />
                            <description text="" />
                            <params>
                                    <param name="lightName" type="String" desc="nome dell'oggetto Path" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int pathSetInterpolationSpline(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "path.setInterpolationSpline", 1))
            {
            // gets the animation
            std::string pathName = luaL_checkstring(luaRuntime, 1);
            Ogre::Animation* ogreAnim = EOSG3D::getInstance()->getSceneManager()->getAnimation(pathName.c_str());
            if (ogreAnim != NULL)
                {
                ogreAnim->setInterpolationMode(Ogre::Animation::IM_SPLINE);
                done = true;
                }
            else
                LOGERROR("[eos."_MODULE_NAME"][path.setInterpolationSpline()] - unable to find path '%s'.", pathName.c_str());
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="pathCreateNodeTrack">
                    <language code="en">
                            <brief text="Creates a Track under a Path automatically associated with a given node" />
                            <description text="" />
                            <params>
                                    <param name="pathName" type="String" desc="name of an existing Path object" />
                                    <param name="trackIndex" type="Numeric" desc="index number of the Track to create" />
                                    <param name="trackNodeName" type="String" desc="name of the node to make automatically move along the track" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Crea una Track in un Path automaticamente associata con un dato nodo" />
                            <description text="" />
                            <params>
                                    <param name="pathName" type="String" desc="nome di un oggetto Path esistente" />
                                    <param name="trackIndex" type="Numeric" desc="indice della Track da creare" />
                                    <param name="trackNodeName" type="String" desc="nome del nodo da far muovere automaticamente lungo la Track" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int pathCreateNodeTrack(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "path.createNodeTrack", 3))
            {
            // gets the animation
            std::string pathName = luaL_checkstring(luaRuntime, 1);
            Ogre::Animation* ogreAnim = EOSG3D::getInstance()->getSceneManager()->getAnimation(pathName.c_str());
            if (ogreAnim != NULL)
                {
                unsigned short trackIndex = (unsigned short) lua_tonumber(luaRuntime, 2);
                std::string trackNodeName = luaL_checkstring(luaRuntime, 3);
                Ogre::SceneNode* trackNode = EOSG3D::getInstance()->getSceneManager()->getSceneNode(trackNodeName.c_str());
                if (trackNode != NULL)
                    {
                    Ogre::AnimationTrack* track = ogreAnim->createNodeTrack(trackIndex, trackNode);
                    if (track == NULL)
                        LOGERROR("[eos."_MODULE_NAME"][path.createNodeTrack()] - unable to create track (%d, %s).", trackIndex, trackNodeName.c_str());
                    else
                        done = true;
                    }
                else
                    LOGERROR("[eos."_MODULE_NAME"][path.createNodeTrack()] - unable to find node '%s'.", trackNodeName.c_str());
                }
            else
                LOGERROR("[eos."_MODULE_NAME"][path.createNodeTrack()] - unable to find path '%s'.", pathName.c_str());
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="pathTrackCreateKeyFrame">
                    <language code="en">
                            <brief text="Creates a position/orientation keyFrame in a Track at a given point in time" />
                            <description text="" />
                            <params>
                                    <param name="pathName" type="String" desc="name of the Path object" />
                                    <param name="trackIndex" type="Numeric" desc="index number of the Track" />
                                    <param name="timePos" type="Numeric" desc="point in time of the keyFrame" />
                                    <param name="traslate_x" type="Numeric" desc="X component of the object traslation for this keyframe" />
                                    <param name="traslate_y" type="Numeric" desc="Y component of the object traslation for this keyframe" />
                                    <param name="traslate_z" type="Numeric" desc="Z component of the object traslation for this keyframe" />
                                    <param name="orientation_x" type="Numeric" desc="X component of the orientation quaternion for this keyframe" />
                                    <param name="orientation_y" type="Numeric" desc="Y component of the orientation quaternion for this keyframe" />
                                    <param name="orientation_z" type="Numeric" desc="Z component of the orientation quaternion for this keyframe" />
                                    <param name="orientation_w" type="Numeric" desc="W component of the orientation quaternion for this keyframe" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Crea un keyFrame di posizione/orientamento all'interno di una Track, ad un determinato istante di tempo" />
                            <description text="" />
                            <params>
                                    <param name="pathName" type="String" desc="nome dell'oggetto Path" />
                                    <param name="trackIndex" type="Numeric" desc="indice della Track" />
                                    <param name="timePos" type="Numeric" desc="istante di tempo del  keyFrame" />
                                    <param name="traslate_x" type="Numeric" desc="componente X della traslazione per questo keyframe" />
                                    <param name="traslate_y" type="Numeric" desc="componente Y della traslazione per questo keyframe" />
                                    <param name="traslate_z" type="Numeric" desc="componente Z della traslazione per questo keyframe" />
                                    <param name="orientation_x" type="Numeric" desc="componente X del quaternione di orientamento per questo keyframe" />
                                    <param name="orientation_y" type="Numeric" desc="componente Y del quaternione di orientamento per questo keyframe" />
                                    <param name="orientation_z" type="Numeric" desc="componente Z del quaternione di orientamento per questo keyframe" />
                                    <param name="orientation_w" type="Numeric" desc="componente W del quaternione di orientamento per questo keyframe" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int pathTrackCreateKeyFrame(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "path.trackCreateKeyFrame", 10))
            {
            // gets the animation
            std::string pathName = luaL_checkstring(luaRuntime, 1);
            Ogre::Animation* ogreAnim = EOSG3D::getInstance()->getSceneManager()->getAnimation(pathName.c_str());
            if (ogreAnim != NULL)
                {
                unsigned short trackIndex = (unsigned short) lua_tonumber(luaRuntime, 2);
                Ogre::NodeAnimationTrack* track = ogreAnim->getNodeTrack(trackIndex);
                if (track != NULL)
                    {
                    Ogre::Real keyFrameIndex = (Ogre::Real)lua_tonumber(luaRuntime, 3);
                    Ogre::TransformKeyFrame* keyFrame = track->createNodeKeyFrame(keyFrameIndex);
                    //
                    keyFrame->setTranslate(Ogre::Vector3((Ogre::Real)lua_tonumber(luaRuntime, 4), (Ogre::Real)lua_tonumber(luaRuntime, 5), (Ogre::Real)lua_tonumber(luaRuntime, 6)));
                    //
                    Ogre::Quaternion q;
                    q.x = (Ogre::Real)lua_tonumber(luaRuntime, 7);
                    q.y = (Ogre::Real)lua_tonumber(luaRuntime, 8);
                    q.z = (Ogre::Real)lua_tonumber(luaRuntime, 9);
                    q.w = (Ogre::Real)lua_tonumber(luaRuntime, 10);
                    keyFrame->setRotation(q);
                    //
                    done = true;
                    }
                else
                    LOGERROR("[eos."_MODULE_NAME"][path.trackCreateKeyFrame()] - unable to find track (%s, %d).", pathName.c_str(), trackIndex);
                }
            else
                LOGERROR("[eos."_MODULE_NAME"][path.trackCreateKeyFrame()] - unable to find path '%s'.", pathName.c_str());
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="renderTextureCreate">
                    <language code="en">
                            <brief text="Creates a blank RenderTexture" />
                            <description text="" />
                            <params>
                                    <param name="renderTextureName" type="String" desc="name of the new RenderTexture" />
                                    <param name="width" type="Numeric" desc="width (in pixels) of the new RenderTexture" />
                                    <param name="height" type="Numeric" desc="height (in pixels) of the new RenderTexture" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Crea una RenderTexture vuota" />
                            <description text="" />
                            <params>
                                    <param name="objectName" type="String" desc="nome della nuova RenderTexture" />
                                    <param name="width" type="Numeric" desc="larghezza (in pixel) della nuova RenderTexture" />
                                    <param name="height" type="Numeric" desc="altezza (in pixel) della nuova RenderTexture" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int renderTextureCreate(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "renderTexture.create", 3))
            done = EOSG3D::getInstance()->createRenderTexture(luaL_checkstring(luaRuntime, 1), (unsigned int) lua_tonumber(luaRuntime, 2), (unsigned int) lua_tonumber(luaRuntime, 3));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="renderTextureToMaterial">
                    <language code="en">
                            <brief text="Binds a previously created RenderTexture to a Material" />
                            <description text="" />
                            <params>
                                    <param name="renderTextureName" type="String" desc="name of an existent renderTexture" />
                                    <param name="materialName" type="String" desc="name of the Material where to bind the RenderTexture" />
                                    <param name="materialTechnique" type="Numeric" desc="index of the material technique (zero-based)" />
                                    <param name="materialPass" type="Numeric" desc="index of the material pass (zero-based)" />
                                    <param name="materialTextureUnitState" type="Numeric" desc="index of the texture unit state in the material (zero-based)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Associa una RenderTexture precedentemente creata, ad un Material" />
                            <description text="" />
                            <params>
                                    <param name="renderTextureName" type="String" desc="nome di una RenderTexture esistente" />
                                    <param name="materialName" type="String" desc="nome del Material al quale associare la RenderTexture" />
                                    <param name="materialTechnique" type="Numeric" desc="indice material technique (zero-based)" />
                                    <param name="materialPass" type="Numeric" desc="indice material pass (zero-based)" />
                                    <param name="materialTextureUnitState" type="Numeric" desc="indice texture unit state nel material (zero-based)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
            </function>
     */
    int renderTextureToMaterial(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "renderTexture.toMaterial", 5))
            done = EOSG3D::getInstance()->bindRenderTextureToMaterial(luaL_checkstring(luaRuntime, 1), luaL_checkstring(luaRuntime, 2), (unsigned int) lua_tonumber(luaRuntime, 3), (unsigned int) lua_tonumber(luaRuntime, 4), (unsigned int) lua_tonumber(luaRuntime, 5));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="overlayCreate">
                    <language code="en">
                            <brief text="Creates an overlay" />
                            <description text="" />
                            <params>
                                    <param name="overlayName" type="String" desc="name of the overlay to create" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Crea un overlay" />
                            <description text="" />
                            <params>
                                    <param name="overlayName" type="String" desc="nome dell'overlay da creare" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayCreate(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.create", 1))
            done = EOSG3D::getInstance()->overlayCreate(luaL_checkstring(luaRuntime, 1));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlaySetVisible">
                    <language code="en">
                            <brief text="Shows/hides an overlay" />
                            <description text="" />
                            <params>
                                    <param name="overlayName" type="String" desc="name of the overlay" />
                                    <param name="visibility" type="Boolean" desc="true to show, false to hide" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Mostra/nasconde un overlay" />
                            <description text="" />
                            <params>
                                    <param name="overlayName" type="String" desc="nome dell'overlay" />
                                    <param name="visibility" type="Boolean" desc="true per mostrarlo, false per nasconderlo" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlaySetVisible(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.setVisible", 2))
            done = EOSG3D::getInstance()->overlaySetVisible(luaL_checkstring(luaRuntime, 1), lua_toboolean(luaRuntime, 2));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlaySetZOrder">
                    <language code="en">
                            <brief text="Sets the Z-Order of an overlay" />
                            <description text="" />
                            <params>
                                    <param name="overlayName" type="String" desc="name of the overlay" />
                                    <param name="ZOrder" type="Numeric" desc="ZOrder to give to the overlay" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta lo Z-Order di un overlay" />
                            <description text="" />
                            <params>
                                    <param name="overlayName" type="String" desc="nome dell'overlay" />
                                    <param name="ZOrder" type="Numeric" desc="ZOrder da dare all'overlay" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlaySetZOrder(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.setZOrder", 2))
            done = EOSG3D::getInstance()->overlaySetZOrder(luaL_checkstring(luaRuntime, 1), (unsigned short) lua_tonumber(luaRuntime, 2));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayAdd2DElement">
                    <language code="en">
                            <brief text="Attaches a 2D overlay element to an overlay" />
                            <description text="" />
                            <params>
                                    <param name="overlayName" type="String" desc="name of the overlay" />
                                    <param name="overlayElementName" type="String" desc="name of the overlay element to attach to the overlay" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Inserisce un elemento overlay 2D in un overlay" />
                            <description text="" />
                            <params>
                                    <param name="overlayName" type="String" desc="nome dell'overlay" />
                                    <param name="overlayElementName" type="String" desc="nome dell'overlay element da inserire nell'overlay" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayAdd2DElement(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.add2DElement", 2))
            done = EOSG3D::getInstance()->overlayAdd2D(luaL_checkstring(luaRuntime, 1), luaL_checkstring(luaRuntime, 2));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayAdd3DSceneNode">
                    <language code="en">
                            <brief text="Attaches a 3D scene node to an overlay" />
                            <description text="This function is used to display a 3D Entity in an overlay" />
                            <params>
                                    <param name="overlayName" type="String" desc="name of the overlay" />
                                    <param name="sceneNodeName" type="String" desc="name of the 3D scene node to attach to the overlay" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Inserisce un nodo della scena 3D in un overlay" />
                            <description text="Questa funzione e' usata per visualizzare un Entity 3D in un overlay" />
                            <params>
                                    <param name="overlayName" type="String" desc="nome dell'overlay" />
                                    <param name="sceneNodeName" type="String" desc="nome del nodo della scena 3D da inserire nell'overlay" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayAdd3DSceneNode(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.add3DSceneNode", 2))
            done = EOSG3D::getInstance()->overlaySetZOrder(luaL_checkstring(luaRuntime, 1), (unsigned short) lua_tonumber(luaRuntime, 2));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayContainerCreate">
                    <language code="en">
                            <brief text="Creates a new OverlayContainer" />
                            <description text="An OverlayContainer is a subclass of OverlayElement, and is designed to contain other OverlayElements." />
                            <params>
                                    <param name="overlayContainerName" type="String" desc="name of the new overlay container" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Crea un nuovo OverlayContainer" />
                            <description text="Un OverlayContainer e' una sottoclasse di OverlayElement, ed e' progettato per contenere altri OverlayElements." />
                            <params>
                                    <param name="overlayContainerName" type="String" desc="nome del nuovo overlay container" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayContainerCreate(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.createContainer", 1))
            done = EOSG3D::getInstance()->overlayContainerCreate(luaL_checkstring(luaRuntime, 1));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayContainerAddChild">
                    <language code="en">
                            <brief text="Attaches an overlay element to an overlay container" />
                            <description text="" />
                            <params>
                                    <param name="overlayContainerName" type="String" desc="name of the overlay container" />
                                    <param name="overlayElementName" type="String" desc="name of the overlay element to put into the overlay container" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Inserisce un overlay element in un overlay container" />
                            <description text="" />
                            <params>
                                    <param name="overlayContainerName" type="String" desc="nome dell'overlay container" />
                                    <param name="overlayElementName" type="String" desc="nome dell'overlay element da inserire nell'overlay container" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayContainerAddChild(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.containerAddChild", 2))
            done = EOSG3D::getInstance()->overlayContainerAddChild(luaL_checkstring(luaRuntime, 1), luaL_checkstring(luaRuntime, 2));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayTextAreaElementCreate">
                    <language code="en">
                            <brief text="Creates a new TextArea overlay element" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="name of the text area overlay element to create" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Crea un nuovo elemento TextArea" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="nome dell'overlay element da creare" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayTextAreaElementCreate(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.textAreaElementCreate", 1))
            done = EOSG3D::getInstance()->overlayTextAreaElementCreate(luaL_checkstring(luaRuntime, 1));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayTextAreaSetFontName">
                    <language code="en">
                            <brief text="Tells to the TextArea which font to use" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="name of the text area overlay element" />
                                    <param name="fontName" type="String" desc="name of the font to use" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta ad un elemento TextArea il font da usare" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="nome dell'elemento TextArea" />
                                    <param name="fontName" type="String" desc="nome del font da usare" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayTextAreaSetFontName(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.textAreaSetFontName", 2))
            done = EOSG3D::getInstance()->overlayTextAreaSetFontName(luaL_checkstring(luaRuntime, 1), luaL_checkstring(luaRuntime, 2));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayTextAreaSetFontSize">
                    <language code="en">
                            <brief text="Tells to the TextArea the font size to use" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="name of the text area overlay element" />
                                    <param name="fontSize" type="Numeric" desc="size of the font to use" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta ad un elemento TextArea la dimensione del font da usare" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="nome dell'elemento TextArea" />
                                    <param name="fontSize" type="Numeric" desc="dimensione del font da usare" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayTextAreaSetFontSize(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.textAreaSetFontSize", 2))
            done = EOSG3D::getInstance()->overlayTextAreaSetFontSize(luaL_checkstring(luaRuntime, 1), (float) lua_tonumber(luaRuntime, 2));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayTextAreaSetCaption">
                    <language code="en">
                            <brief text="Sets the text of the TextArea" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="name of the text area overlay element" />
                                    <param name="caption" type="String" desc="caption string" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il testo di una TextArea" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="nome dell'elemento TextArea" />
                                    <param name="caption" type="String" desc="testo da settare" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayTextAreaSetCaption(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.textAreaSetCaption", 2))
            done = EOSG3D::getInstance()->overlayTextAreaSetCaption(luaL_checkstring(luaRuntime, 1), luaL_checkstring(luaRuntime, 2));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayTextAreaGetCaption">
                    <language code="en">
                            <brief text="Returns the current text (caption) of a TextArea" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="name of the text area overlay element" />
                            </params>
                            <returns>
                                    <param name="caption" type="String" desc="caption text" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il testo corrente (caption) di una TextArea" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="nome dell'elemento TextArea" />
                            </params>
                            <returns>
                                    <param name="caption" type="String" desc="testo corrente" />
                            </returns>
                    </language>
            </function>
     */
    int overlayTextAreaGetCaption(lua_State* luaRuntime)
        {
        const char* caption = NULL;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.textAreaGetCaption", 1))
            caption = EOSG3D::getInstance()->overlayTextAreaGetCaption(luaL_checkstring(luaRuntime, 1));
        lua_pushstring(luaRuntime, caption);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayTextAreaSetColor">
                    <language code="en">
                            <brief text="Sets the text color of a TextArea" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="name of the text area overlay element" />
                                    <param name="RedAmount" type="Numeric" desc="Red component of the color (0.0 to 1.0)" />
                                    <param name="GreenAmount" type="Numeric" desc="Green component of color (0.0 to 1.0)" />
                                    <param name="BlueAmount" type="Numeric" desc="Blue component of the color (0.0 to 1.0)" />
                                    <param name="AlphaAmount" type="Numeric" desc="Alpha amount of the color (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il colore del testo di una TextArea" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="nome dell'elemento TextArea" />
                                    <param name="RedAmount" type="Numeric" desc="componente Rosso del colore (0.0 to 1.0)" />
                                    <param name="GreenAmount" type="Numeric" desc="componente Verde del colore (0.0 to 1.0)" />
                                    <param name="BlueAmount" type="Numeric" desc="componente Blu del colore (0.0 to 1.0)" />
                                    <param name="AlphaAmount" type="Numeric" desc="quantit�Alpha del colore (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayTextAreaSetColor(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.textAreaSetColor", 5))
            done = EOSG3D::getInstance()->overlayTextAreaSetColor(luaL_checkstring(luaRuntime, 1), (float) lua_tonumber(luaRuntime, 2), (float) lua_tonumber(luaRuntime, 3), (float) lua_tonumber(luaRuntime, 4), (float) lua_tonumber(luaRuntime, 5));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayTextAreaSetColorTop">
                    <language code="en">
                            <brief text="Sets the upperside text color of a TextArea" />
                            <description text="These functions, overlayTextAreaSetColorTop and overlayTextAreaSetColorBottom are used to set vertical color gradients to the text" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="name of the text area overlay element" />
                                    <param name="RedAmount" type="Numeric" desc="Red component of the color (0.0 to 1.0)" />
                                    <param name="GreenAmount" type="Numeric" desc="Green component of color (0.0 to 1.0)" />
                                    <param name="BlueAmount" type="Numeric" desc="Blue component of the color (0.0 to 1.0)" />
                                    <param name="AlphaAmount" type="Numeric" desc="Alpha amount of the color (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il colore testo (superiore) di una TextArea" />
                            <description text="Queste funzioni, overlayTextAreaSetColorTop e overlayTextAreaSetColorBottom, sono usate per settare gradienti verticali di colore al testo" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="nome dell'elemento TextArea" />
                                    <param name="RedAmount" type="Numeric" desc="componente Rosso del colore (0.0 to 1.0)" />
                                    <param name="GreenAmount" type="Numeric" desc="componente Verde del colore (0.0 to 1.0)" />
                                    <param name="BlueAmount" type="Numeric" desc="componente Blu del colore (0.0 to 1.0)" />
                                    <param name="AlphaAmount" type="Numeric" desc="quantit�Alpha del colore (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayTextAreaSetColorTop(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.textAreaSetColorTop", 5))
            done = EOSG3D::getInstance()->overlayTextAreaSetColorTop(luaL_checkstring(luaRuntime, 1), (float) lua_tonumber(luaRuntime, 2), (float) lua_tonumber(luaRuntime, 3), (float) lua_tonumber(luaRuntime, 4), (float) lua_tonumber(luaRuntime, 5));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayTextAreaSetColorBottom">
                    <language code="en">
                            <brief text="Sets the lowerside text color of a TextArea" />
                            <description text="These functions, overlayTextAreaSetColorTop and overlayTextAreaSetColorBottom, are used to set vertical color gradients to the text" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="name of the text area overlay element" />
                                    <param name="RedAmount" type="Numeric" desc="Red component of the color (0.0 to 1.0)" />
                                    <param name="GreenAmount" type="Numeric" desc="Green component of color (0.0 to 1.0)" />
                                    <param name="BlueAmount" type="Numeric" desc="Blue component of the color (0.0 to 1.0)" />
                                    <param name="AlphaAmount" type="Numeric" desc="Alpha amount of the color (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il colore testo (inferiore) di una TextArea" />
                            <description text="Queste funzioni, overlayTextAreaSetColorTop e overlayTextAreaSetColorBottom, sono usate per settare gradienti verticali di colore al testo" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="nome dell'elemento TextArea" />
                                    <param name="RedAmount" type="Numeric" desc="componente Rosso del colore (0.0 to 1.0)" />
                                    <param name="GreenAmount" type="Numeric" desc="componente Verde del colore (0.0 to 1.0)" />
                                    <param name="BlueAmount" type="Numeric" desc="componente Blu del colore (0.0 to 1.0)" />
                                    <param name="AlphaAmount" type="Numeric" desc="quantit�Alpha del colore (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayTextAreaSetColorBottom(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.textAreaSetColorBottom", 5))
            done = EOSG3D::getInstance()->overlayTextAreaSetColorBottom(luaL_checkstring(luaRuntime, 1), (float) lua_tonumber(luaRuntime, 2), (float) lua_tonumber(luaRuntime, 3), (float) lua_tonumber(luaRuntime, 4), (float) lua_tonumber(luaRuntime, 5));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayTextAreaGetColor">
                    <language code="en">
                            <brief text="Returns current text color of a TextArea" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="name of the text area overlay element" />
                            </params>
                            <returns>
                                    <return name="RedAmount" type="Numeric" desc="Red component of the color (0.0 to 1.0)" />
                                    <return name="GreenAmount" type="Numeric" desc="Green component of color (0.0 to 1.0)" />
                                    <return name="BlueAmount" type="Numeric" desc="Blue component of the color (0.0 to 1.0)" />
                                    <return name="AlphaAmount" type="Numeric" desc="Alpha value of the color (0.0 to 1.0)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il colore corrente del testo di una TextArea" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="nome dell'elemento TextArea" />
                            </params>
                            <returns>
                                    <return name="RedAmount" type="Numeric" desc="componente Rosso del colore (0.0 to 1.0)" />
                                    <return name="GreenAmount" type="Numeric" desc="componente Verde del colore (0.0 to 1.0)" />
                                    <return name="BlueAmount" type="Numeric" desc="componente Blu del colore (0.0 to 1.0)" />
                                    <return name="AlphaAmount" type="Numeric" desc="valore Alpha del colore (0.0 to 1.0)" />
                            </returns>
                    </language>
            </function>
     */
    int overlayTextAreaGetColor(lua_State* luaRuntime)
        {
        Ogre::ColourValue color = Ogre::ColourValue::White;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.textAreaGetColor", 1))
            color = EOSG3D::getInstance()->overlayTextAreaGetColor(luaL_checkstring(luaRuntime, 1));
        //
        lua_pushnumber(luaRuntime, color.r);
        lua_pushnumber(luaRuntime, color.g);
        lua_pushnumber(luaRuntime, color.b);
        lua_pushnumber(luaRuntime, color.a);
        return 4;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayTextAreaGetColorTop">
                    <language code="en">
                            <brief text="Returns current upperside text color of a TextArea" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="name of the text area overlay element" />
                            </params>
                            <returns>
                                    <return name="RedAmount" type="Numeric" desc="Red component of the color (0.0 to 1.0)" />
                                    <return name="GreenAmount" type="Numeric" desc="Green component of color (0.0 to 1.0)" />
                                    <return name="BlueAmount" type="Numeric" desc="Blue component of the color (0.0 to 1.0)" />
                                    <return name="AlphaAmount" type="Numeric" desc="Alpha value of the color (0.0 to 1.0)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il colore corrente della parte superiore del testo di una TextArea" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="nome dell'elemento TextArea" />
                            </params>
                            <returns>
                                    <return name="RedAmount" type="Numeric" desc="componente Rosso del colore (0.0 to 1.0)" />
                                    <return name="GreenAmount" type="Numeric" desc="componente Verde del colore (0.0 to 1.0)" />
                                    <return name="BlueAmount" type="Numeric" desc="componente Blu del colore (0.0 to 1.0)" />
                                    <return name="AlphaAmount" type="Numeric" desc="valore Alpha del colore (0.0 to 1.0)" />
                            </returns>
                    </language>
            </function>
     */
    int overlayTextAreaGetColorTop(lua_State* luaRuntime)
        {
        Ogre::ColourValue color = Ogre::ColourValue::White;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.textAreaGetColorTop", 1))
            color = EOSG3D::getInstance()->overlayTextAreaGetColorTop(luaL_checkstring(luaRuntime, 1));
        //
        lua_pushnumber(luaRuntime, color.r);
        lua_pushnumber(luaRuntime, color.g);
        lua_pushnumber(luaRuntime, color.b);
        lua_pushnumber(luaRuntime, color.a);
        return 4;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayTextAreaGetColorBottom">
                    <language code="en">
                            <brief text="Returns current lowerside text color of a TextArea" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="name of the text area overlay element" />
                            </params>
                            <returns>
                                    <return name="RedAmount" type="Numeric" desc="Red component of the color (0.0 to 1.0)" />
                                    <return name="GreenAmount" type="Numeric" desc="Green component of color (0.0 to 1.0)" />
                                    <return name="BlueAmount" type="Numeric" desc="Blue component of the color (0.0 to 1.0)" />
                                    <return name="AlphaAmount" type="Numeric" desc="Alpha value of the color (0.0 to 1.0)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il colore corrente della parte inferiore del testo di una TextArea" />
                            <description text="" />
                            <params>
                                    <param name="textAreaElementName" type="String" desc="nome dell'elemento TextArea" />
                            </params>
                            <returns>
                                    <return name="RedAmount" type="Numeric" desc="componente Rosso del colore (0.0 to 1.0)" />
                                    <return name="GreenAmount" type="Numeric" desc="componente Verde del colore (0.0 to 1.0)" />
                                    <return name="BlueAmount" type="Numeric" desc="componente Blu del colore (0.0 to 1.0)" />
                                    <return name="AlphaAmount" type="Numeric" desc="valore Alpha del colore (0.0 to 1.0)" />
                            </returns>
                    </language>
            </function>
     */
    int overlayTextAreaGetColorBottom(lua_State* luaRuntime)
        {
        Ogre::ColourValue color = Ogre::ColourValue::White;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.textAreaGetColorBottom", 1))
            color = EOSG3D::getInstance()->overlayTextAreaGetColorBottom(luaL_checkstring(luaRuntime, 1));
        //
        lua_pushnumber(luaRuntime, color.r);
        lua_pushnumber(luaRuntime, color.g);
        lua_pushnumber(luaRuntime, color.b);
        lua_pushnumber(luaRuntime, color.a);
        return 4;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayElementSetVisible">
                    <language code="en">
                            <brief text="Enables/disables the visibility of an OverlayElement" />
                            <description text="" />
                            <params>
                                    <param name="overlayElementName" type="String" desc="name of the overlay element" />
                                    <param name="visible" type="Boolean" desc="visibility of the element" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise (true: visible, false: hidden)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Abilita/disabilita la visibilita' di un OverlayElement" />
                            <description text="" />
                            <params>
                                    <param name="overlayElementName" type="String" desc="nome dell'overlay element" />
                                    <param name="visible" type="Boolean" desc="stato di visibilta' dell'elemento (true: visibile, false: nascosto)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayElementSetVisible(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.elementSetVisible", 2))
            done = EOSG3D::getInstance()->overlayElementSetVisible(luaL_checkstring(luaRuntime, 1), lua_toboolean(luaRuntime, 2));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayElementSetDimensions">
                    <language code="en">
                            <brief text="Sets width and height of an OverlayElement" />
                            <description text="" />
                            <params>
                                    <param name="overlayElementName" type="String" desc="name of the overlay element" />
                                    <param name="width" type="numeric" desc="width to give to the overlay element" />
                                    <param name="height" type="numeric" desc="height to give to the overlay element" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta larghezza e altezza di un OverlayElement" />
                            <description text="" />
                            <params>
                                    <param name="overlayElementName" type="String" desc="nome dell'overlay element" />
                                    <param name="width" type="numeric" desc="larghezza da assegnare all'overlay element" />
                                    <param name="height" type="numeric" desc="altezza da assegnare all'overlay element" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayElementSetDimensions(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.elementSetDimensions", 3))
            done = EOSG3D::getInstance()->overlayElementSetDimensions(luaL_checkstring(luaRuntime, 1), (float) lua_tonumber(luaRuntime, 2), (float) lua_tonumber(luaRuntime, 3));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayElementSetPosition">
                    <language code="en">
                            <brief text="Sets the position of an OverlayElement" />
                            <description text="" />
                            <params>
                                    <param name="overlayElementName" type="String" desc="name of the overlay element" />
                                    <param name="x" type="Numeric" desc="x position of the element" />
                                    <param name="y" type="Numeric" desc="y position of the element" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la posizione di un OverlayElement" />
                            <description text="" />
                            <params>
                                    <param name="overlayElementName" type="String" desc="nome dell'overlay element" />
                                    <param name="x" type="Numeric" desc="position x dell'elemento" />
                                    <param name="y" type="Numeric" desc="position y dell'elemento" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayElementSetPosition(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.elementSetPosition", 3))
            done = EOSG3D::getInstance()->overlayElementSetPosition(luaL_checkstring(luaRuntime, 1), (float) lua_tonumber(luaRuntime, 2), (float) lua_tonumber(luaRuntime, 3));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayElementSetMaterial">
                    <language code="en">
                            <brief text="Sets the material name of an OverlayElement" />
                            <description text="" />
                            <params>
                                    <param name="overlayElementName" type="String" desc="name of the overlay element" />
                                    <param name="materialName" type="String" desc="name of the material to give to the overlay element" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il materiale di un OverlayElement" />
                            <description text="" />
                            <params>
                                    <param name="overlayElementName" type="String" desc="nome dell'overlay element" />
                                    <param name="materialName" type="String" desc="nome del material da associare all'overlay element" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayElementSetMaterial(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.elementSetMaterial", 2))
            done = EOSG3D::getInstance()->overlayElementSetMaterial(luaL_checkstring(luaRuntime, 1), luaL_checkstring(luaRuntime, 2));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayElementSetMetricsAbsolute">
                    <language code="en">
                            <brief text="Sets the overlay element to use ABSOLUTE (pixels) Metrics Mode" />
                            <description text="" />
                            <params>
                                    <param name="overlayElementName" type="String" desc="name of the overlay element" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta l'overlay element in modo che usi la modalita' metrica ASSOLUTA (unita' in pixels)" />
                            <description text="" />
                            <params>
                                    <param name="overlayElementName" type="String" desc="nome dell'overlay element" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayElementSetMetricsAbsolute(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.elementSetMetricsAbsolute", 1))
            done = EOSG3D::getInstance()->overlayElementSetMetricsMode(luaL_checkstring(luaRuntime, 1), "ABSOLUTE");
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="overlayElementSetMetricsAbsolute">
                    <language code="en">
                            <brief text="Sets the overlay element to use RELATIVE (0.0 to 1.0) Metrics Mode" />
                            <description text="" />
                            <params>
                                    <param name="overlayElementName" type="String" desc="name of the overlay element" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta l'overlay element in modo che usi la modalita' metrica RELATIVA (valori compresi tra 0.0 e 1.0)" />
                            <description text="" />
                            <params>
                                    <param name="overlayElementName" type="String" desc="nome dell'overlay element" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int overlayElementSetMetricsRelative(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "overlay.elementSetMetricsRelative", 1))
            done = EOSG3D::getInstance()->overlayElementSetMetricsMode(luaL_checkstring(luaRuntime, 1), "RELATIVE");
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="billboardSetCreate">
                    <language code="en">
                            <brief text="Creates a new BillboardSet" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="name of the new BillboardSet to create" />
                            </params>
                            <returns>
                                    <return name="bbSetSceneNodeName" type="String" desc="Nome of the scene node where new BillboardSet has been attached" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Crea un nuovo BillboardSet" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="nome del nuovo BillboardSet da creare" />
                            </params>
                            <returns>
                                    <return name="bbSetSceneNodeName" type="String" desc="Nome dello scene node dove e' stato attaccato il BillboardSet" />
                            </returns>
                    </language>
            </function>
     */
    int billboardSetCreate(lua_State* luaRuntime)
        {
        const char* sceneNodeName = NULL;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "billboardSet.create", 1))
            {
            const char* bbSetName = luaL_checkstring(luaRuntime, 1);
            Ogre::SceneNode* sceneNode = EOSG3D::getInstance()->billboardSetCreate(bbSetName, NULL);
            if (sceneNode != NULL) sceneNodeName = sceneNode->getName().c_str();
            }
        lua_pushstring(luaRuntime, sceneNodeName);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="billboardSetSetMaterial">
                    <language code="en">
                            <brief text="Sets the material of a BillboardSet" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="name of the BillboardSet" />
                                    <param name="materialName" type="String" desc="name of the material to assign to the BillboardSet" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il materiale di un BillboardSet" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="nome del BillboardSet" />
                                    <param name="materialName" type="String" desc="nome del materiale da assegnare al BillboardSet" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int billboardSetSetMaterial(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "billboardSet.setMaterial", 2))
            done = EOSG3D::getInstance()->billboardSetSetMaterial(luaL_checkstring(luaRuntime, 1), luaL_checkstring(luaRuntime, 2));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="billboardSetSetDefaultDimensions">
                    <language code="en">
                            <brief text="Sets default dimensions of a BillboardSet" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="name of the BillboardSet" />
                                    <param name="defaultWidth" type="Numeric" desc="default width to set to the BillboardSet" />
                                    <param name="defaultHeight" type="Numeric" desc="default height to set to the BillboardSet" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta le dimensioni di default di un BillboardSet" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="name of the BillboardSet" />
                                    <param name="defaultWidth" type="Numeric" desc="default width to set to the BillboardSet" />
                                    <param name="defaultHeight" type="Numeric" desc="default height to set to the BillboardSet" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int billboardSetSetDefaultDimensions(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "billboardSet.setDefaultDimensions", 3))
            done = EOSG3D::getInstance()->billboardSetSetDefaultDimensions(luaL_checkstring(luaRuntime, 1), (float) lua_tonumber(luaRuntime, 2), (float) lua_tonumber(luaRuntime, 3));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="billboardCreate">
                    <language code="en">
                            <brief text="Creates a Billboard under a BillboardSet" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="name of the BillboardSet where to create the new Billboard" />
                                    <param name="XPosition" type="Numeric" desc="X coordinate of the relative position of the new billboard" />
                                    <param name="YPosition" type="Numeric" desc="Y coordinate of the relative position of the new billboard" />
                                    <param name="ZPosition" type="Numeric" desc="Z coordinate of the relative position of the new billboard" />
                                    <param name="RedAmount" type="Numeric" desc="Red component of the new billboard's color (0.0 to 1.0)" />
                                    <param name="GreenAmount" type="Numeric" desc="Green component of the new billboard's color (0.0 to 1.0)" />
                                    <param name="BlueAmount" type="Numeric" desc="Blue component of the new billboard's color (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Crea un nuovo Billboard in un BillboardSet" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="nome del BillboardSet dove creare il nuovo Billboard" />
                                    <param name="XPosition" type="Numeric" desc="Coordinata X della posizione relativa del nuovo billboard" />
                                    <param name="YPosition" type="Numeric" desc="Coordinata Y della posizione relativa del nuovo billboard" />
                                    <param name="ZPosition" type="Numeric" desc="Coordinata Z della posizione relativa del nuovo billboard" />
                                    <param name="RedAmount" type="Numeric" desc="Componente Rosso del colore del nuovo billboard (da 0.0 a 1.0)" />
                                    <param name="GreenAmount" type="Numeric" desc="Componente Verde del colore del nuovo billboard (da 0.0 a 1.0)" />
                                    <param name="BlueAmount" type="Numeric" desc="Componente Blu del colore del nuovo billboard (da 0.0 a 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int billboardCreate(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "billboard.create", 7))
            done = EOSG3D::getInstance()->billboardCreate(luaL_checkstring(luaRuntime, 1), (float) lua_tonumber(luaRuntime, 2), (float) lua_tonumber(luaRuntime, 3), (float) lua_tonumber(luaRuntime, 4), (float) lua_tonumber(luaRuntime, 5), (float) lua_tonumber(luaRuntime, 6), lua_tonumber(luaRuntime, 7));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="billboardGetRotation">
                    <language code="en">
                            <brief text="Returns the current rotation angle (in radians) of a billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="name of the BillboardSet where is the Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="index of the Billboard in the BillboardSet" />
                            </params>
                            <returns>
                                    <return name="rotationAngle" type="numeric" desc="rotation angle (in radians) of the Billboard (-1.0 in case of error)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna l'angolo attuale di rotazione (in radianti) di un billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="nome del BillboardSet dove si trova il Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="Indice del Billboard all'interno del BillboardSet" />
                            </params>
                            <returns>
                                    <return name="rotationAngle" type="numeric" desc="angolo di rotazione (in radianti) del Billboard (-1.0 in caso di errore)" />
                            </returns>
                    </language>
            </function>
     */
    int billboardGetRotation(lua_State* luaRuntime)
        {
        float rotation = -1.0;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "billboard.getRotation", 2))
            rotation = EOSG3D::getInstance()->billboardGetRotation(luaL_checkstring(luaRuntime, 1), (unsigned int) lua_tonumber(luaRuntime, 2));
        lua_pushnumber(luaRuntime, rotation);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="billboardSetRotation">
                    <language code="en">
                            <brief text="Sets the rotation angle (in radians) of a billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="name of the BillboardSet where is the Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="index of the Billboard in the BillboardSet" />
                                    <param name="rotationAngle" type="Numeric" desc="rotation angle (in radians) to give to the Billboard" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta l'angolo di rotazione (in radianti) di un billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="nome del BillboardSet dove si trova il Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="indice del Billboard all'interno del BillboardSet" />
                                    <param name="rotationAngle" type="Numeric" desc="angolo di rotazione (in radianti) da dare al Billboard" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int billboardSetRotation(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "billboard.setRotation", 3))
            done = EOSG3D::getInstance()->billboardSetRotation(luaL_checkstring(luaRuntime, 1), (unsigned int) lua_tonumber(luaRuntime, 2), (float) lua_tonumber(luaRuntime, 3));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="billboardGetPosition">
                    <language code="en">
                            <brief text="Returns the current relative position of a billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="name of the BillboardSet where is the Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="index of the Billboard in the BillboardSet" />
                            </params>
                            <returns>
                                    <return name="XPosition" type="numeric" desc="X coordinate of the Billboard's position" />
                                    <return name="YPosition" type="numeric" desc="Y coordinate of the Billboard's position" />
                                    <return name="ZPosition" type="numeric" desc="Z coordinate of the Billboard's position" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna l'attuale posizione relativa di un billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="nome del BillboardSet dove si trova il Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="indice del Billboard all'interno del BillboardSet" />
                            </params>
                            <returns>
                                    <return name="XPosition" type="numeric" desc="Coordinata X della posizione del Billboard" />
                                    <return name="YPosition" type="numeric" desc="Coordinata Y della posizione del Billboard" />
                                    <return name="ZPosition" type="numeric" desc="Coordinata Z della posizione del Billboard" />
                            </returns>
                    </language>
            </function>
     */
    int billboardGetPosition(lua_State* luaRuntime)
        {
        Ogre::Vector3 position = Ogre::Vector3::ZERO;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "billboard.getPosition", 2))
            position = EOSG3D::getInstance()->billboardGetPosition(luaL_checkstring(luaRuntime, 1), (unsigned int) lua_tonumber(luaRuntime, 2));
        //
        lua_pushnumber(luaRuntime, position.x);
        lua_pushnumber(luaRuntime, position.y);
        lua_pushnumber(luaRuntime, position.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="billboardSetPosition">
                    <language code="en">
                            <brief text="Sets the relative position of a Billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="name of the BillboardSet where is the Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="index of the Billboard in the BillboardSet" />
                                    <param name="XPosition" type="Numeric" desc="X position where to set the Billboard" />
                                    <param name="YPosition" type="Numeric" desc="Y position where to set the Billboard" />
                                    <param name="ZPosition" type="Numeric" desc="Z position where to set the Billboard" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta la posizione relativa di un Billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="nome del BillboardSet dove si trova il Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="indice del Billboard all'interno del BillboardSet" />
                                    <param name="XPosition" type="Numeric" desc="Coordinata X dove posizionare il Billboard" />
                                    <param name="YPosition" type="Numeric" desc="Coordinata Y dove posizionare il Billboard" />
                                    <param name="ZPosition" type="Numeric" desc="Coordinata Z dove posizionare il Billboard" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int billboardSetPosition(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "billboard.setPosition", 5))
            done = EOSG3D::getInstance()->billboardSetPosition(luaL_checkstring(luaRuntime, 1), (unsigned int) lua_tonumber(luaRuntime, 2), (float) lua_tonumber(luaRuntime, 3), (float) lua_tonumber(luaRuntime, 4), (float) lua_tonumber(luaRuntime, 5));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="billboardSetDimensions">
                    <language code="en">
                            <brief text="Sets the dimensions of a Billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="name of the BillboardSet where is the Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="index of the Billboard in the BillboardSet" />
                                    <param name="width" type="Numeric" desc="width of the Billboard" />
                                    <param name="height" type="Numeric" desc="height of the Billboard" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta le dimensioni di un Billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="nome del BillboardSet dove si trova il Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="indice del Billboard all'interno del BillboardSet" />
                                    <param name="width" type="Numeric" desc="larghezza da settare al Billboard" />
                                    <param name="height" type="Numeric" desc="altezza da settare al Billboard" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int billboardSetDimensions(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "billboard.setDimensions", 4))
            done = EOSG3D::getInstance()->billboardSetDimensions(luaL_checkstring(luaRuntime, 1), (unsigned int) lua_tonumber(luaRuntime, 2), (float) lua_tonumber(luaRuntime, 3), (float) lua_tonumber(luaRuntime, 4));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="billboardGetDimensions">
                    <language code="en">
                            <brief text="Returns current width and height of a Billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="name of the BillboardSet where is the Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="index of the Billboard in the BillboardSet" />
                            </params>
                            <returns>
                                    <return name="width" type="Numeric" desc="current width of the billboard" />
                                    <return name="height" type="Numeric" desc="current height of the billboard" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna le dimensioni attuali (larghezza, altezza) di un Billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="name of the BillboardSet where is the Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="index of the Billboard in the BillboardSet" />
                            </params>
                            <returns>
                                    <return name="width" type="Numeric" desc="larghezza attuale del billboard" />
                                    <return name="height" type="Numeric" desc="altezza attuale del billboard" />
                            </returns>
                    </language>
            </function>
     */
    int billboardGetDimensions(lua_State* luaRuntime)
        {
        Ogre::Vector3 dimensions = Ogre::Vector3::ZERO;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "billboard.getDimensions", 2))
            dimensions = EOSG3D::getInstance()->billboardGetDimensions(luaL_checkstring(luaRuntime, 1), (unsigned int) lua_tonumber(luaRuntime, 2));
        //
        lua_pushnumber(luaRuntime, dimensions.x);
        lua_pushnumber(luaRuntime, dimensions.y);
        return 2;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="billboardResetDimensions">
                    <language code="en">
                            <brief text="Resets the Billboard to use the parent BillboardSet's dimensions instead of it's own" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="name of the BillboardSet where is the Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="index of the Billboard in the BillboardSet" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Resetta le dimensioni di un Billboard in modo che usi le dimensioni predefinite dal BillboardSet dove si trova" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="nome del BillboardSet dove si trova il Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="indice del Billboard all'interno del BillboardSet" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int billboardResetDimensions(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "billboard.resetDimensions", 2))
            done = EOSG3D::getInstance()->billboardResetDimensions(luaL_checkstring(luaRuntime, 1), (unsigned int) lua_tonumber(luaRuntime, 2));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="billboardSetColor">
                    <language code="en">
                            <brief text="Sets the color of a Billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="name of the BillboardSet where is the Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="index of the Billboard in the BillboardSet" />
                                    <param name="RedAmount" type="Numeric" desc="Red component of the new billboard color (0.0 to 1.0)" />
                                    <param name="GreenAmount" type="Numeric" desc="Green component of the new billboard color (0.0 to 1.0)" />
                                    <param name="BlueAmount" type="Numeric" desc="Blue component of the new billboard color (0.0 to 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il colore di un Billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="nome del BillboardSet dove si trova il Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="indice del Billboard all'interno del BillboardSet" />
                                    <param name="RedAmount" type="Numeric" desc="Componente Rosso del nuovo colore billboard (da 0.0 a 1.0)" />
                                    <param name="GreenAmount" type="Numeric" desc="Componente Verde del nuovo colore billboard (da 0.0 a 1.0)" />
                                    <param name="BlueAmount" type="Numeric" desc="Componente Blu del nuovo colore del billboard (da 0.0 a 1.0)" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int billboardSetColor(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "billboard.setColor", 5))
            done = EOSG3D::getInstance()->billboardSetColor(luaL_checkstring(luaRuntime, 1), (unsigned int) lua_tonumber(luaRuntime, 2), (float) lua_tonumber(luaRuntime, 3), (float) lua_tonumber(luaRuntime, 4), (float) lua_tonumber(luaRuntime, 5));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="billboardGetColor">
                    <language code="en">
                            <brief text="Returns the current color of a Billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="name of the BillboardSet where is the Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="index of the Billboard in the BillboardSet" />
                            </params>
                            <returns>
                                    <return name="RedAmount" type="Numeric" desc="Red component of the billboard color" />
                                    <return name="GreenAmount" type="Numeric" desc="Green component of the billboard color" />
                                    <return name="BlueAmount" type="Numeric" desc="Blue component of the billboard color" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il colore attuale di un Billboard" />
                            <description text="" />
                            <params>
                                    <param name="billboardSetName" type="String" desc="nome del BillboardSet dove si trova il Billboard" />
                                    <param name="billboardIndex" type="Numeric" desc="indice del Billboard all'interno del BillboardSet" />
                            </params>
                            <returns>
                                    <return name="RedAmount" type="Numeric" desc="Componente Rosso del colore del billboard" />
                                    <return name="GreenAmount" type="Numeric" desc="Componente Verde del colore del billboard" />
                                    <return name="BlueAmount" type="Numeric" desc="Componente Blu del colore del billboard" />
                            </returns>
                    </language>
            </function>
     */
    int billboardGetColor(lua_State* luaRuntime)
        {
        Ogre::ColourValue color = Ogre::ColourValue::White;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "billboard.getColor", 2))
            color = EOSG3D::getInstance()->billboardGetColor(luaL_checkstring(luaRuntime, 1), (unsigned int) lua_tonumber(luaRuntime, 2));
        //
        lua_pushnumber(luaRuntime, color.r);
        lua_pushnumber(luaRuntime, color.g);
        lua_pushnumber(luaRuntime, color.b);
        return 3;
        }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="simpleSplineCreate">
                    <language code="en">
                            <brief text="Creates a new SimpleSpline object" />
                            <description text="(note: AutoCalculate status default set to true)" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the SimpleSpline object to create" />
                                    <param name="maxPoints" type="Numeric" desc="maximum number of points the spline can have" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Crea un nuovo oggetto SimpleSpline" />
                            <description text="(nota: lo stato iniziale di AutoCalculate e' impostato a true)" />
                            <params>
                                    <param name="splineName" type="String" desc="nome del nuovo oggetto SimpleSpline da creare" />
                                    <param name="maxPoints" type="Numeric" desc="massimo numero di punti possibili per la spline" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int simpleSplineCreate(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "simpleSpline.create", 2))
            done = EOSG3D::getInstance()->createSimpleSpline(luaL_checkstring(luaRuntime, 1));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="simpleSplineAddPoint">
                    <language code="en">
                            <brief text="Adds a point to a SimpleSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the SimpleSpline object" />
                                    <param name="x" type="Numeric" desc="X component of the point" />
                                    <param name="y" type="Numeric" desc="Y component of the point" />
                                    <param name="z" type="Numeric" desc="Z component of the point" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Aggiunge un punto ad un oggetto SimpleSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto SimpleSpline" />
                                    <param name="x" type="Numeric" desc="componente X del punto" />
                                    <param name="y" type="Numeric" desc="componente Y del punto" />
                                    <param name="z" type="Numeric" desc="componente Z del punto" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int simpleSplineAddPoint(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "simpleSpline.addPoint", 4))
            {
            EOSOgreSimpleSpline* simpleSpline = EOSG3D::getInstance()->simpleSplineByName(luaL_checkstring(luaRuntime, 1));
            if (simpleSpline != NULL)
                {
                simpleSpline->addPoint(Ogre::Vector3(lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4)));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="simpleSplineGetPoint">
                    <language code="en">
                            <brief text="Returns a point from a SimpleSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the SimpleSpline object" />
                                    <param name="pointIndex" type="Numeric" desc="index of the point to get" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="X component of the point" />
                                    <return name="y" type="Numeric" desc="Y component of the point" />
                                    <return name="z" type="Numeric" desc="Z component of the point" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna un punto da un oggetto SimpleSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto SimpleSpline" />
                                    <param name="pointIndex" type="Numeric" desc="indice del punto da ritornare" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="componente X del punto" />
                                    <return name="y" type="Numeric" desc="componente Y del punto" />
                                    <return name="z" type="Numeric" desc="componente Z del punto" />
                            </returns>
                    </language>
            </function>
     */
    int simpleSplineGetPoint(lua_State* luaRuntime)
        {
        Ogre::Vector3 result = Ogre::Vector3::ZERO;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "simpleSpline.getPoint", 2))
            {
            EOSOgreSimpleSpline* simpleSpline = EOSG3D::getInstance()->simpleSplineByName(luaL_checkstring(luaRuntime, 1));
            if (simpleSpline != NULL)
                result = simpleSpline->getPoint((unsigned short int) lua_tonumber(luaRuntime, 2));
            }
        lua_pushnumber(luaRuntime, result.x);
        lua_pushnumber(luaRuntime, result.y);
        lua_pushnumber(luaRuntime, result.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="simpleSplineGetNumPoints">
                    <language code="en">
                            <brief text="Returns current number of points contained in a SimpleSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the SimpleSpline object" />
                            </params>
                            <returns>
                                    <return name="totalPoints" type="Numeric" desc="actual total points contained in the SimpleSpline (-1 in case of errors)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il numero corrente di punti contenuti in un oggetto SimpleSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto SimpleSpline" />
                            </params>
                            <returns>
                                    <return name="totalPoints" type="Numeric" desc="numero di punti attualmente contenuti nell'oggetto SimpleSpline (-1 in caso di errori)" />
                            </returns>
                    </language>
            </function>
     */
    int simpleSplineGetNumPoints(lua_State* luaRuntime)
        {
        int result = -1;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "simpleSpline.getNumPoints", 1))
            {
            EOSOgreSimpleSpline* simpleSpline = EOSG3D::getInstance()->simpleSplineByName(luaL_checkstring(luaRuntime, 1));
            if (simpleSpline != NULL)
                result = simpleSpline->getNumPoints();
            }
        lua_pushnumber(luaRuntime, result);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="simpleSplineClear">
                    <language code="en">
                            <brief text="Removes all points from a SimpleSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the SimpleSpline object" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Rimuove tutti i punti da un oggetto SimpleSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto SimpleSpline" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int simpleSplineClear(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "simpleSpline.clear", 1))
            {
            EOSOgreSimpleSpline* simpleSpline = EOSG3D::getInstance()->simpleSplineByName(luaL_checkstring(luaRuntime, 1));
            if (simpleSpline != NULL)
                {
                simpleSpline->clear();
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="simpleSplineUpdatePoint">
                    <language code="en">
                            <brief text="Updates a point contained in a SimpleSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the SimpleSpline object" />
                                    <param name="pointIndex" type="Numeric" desc="index of the point to update" />
                                    <param name="x" type="Numeric" desc="new X component of the point" />
                                    <param name="y" type="Numeric" desc="new Y component of the point" />
                                    <param name="z" type="Numeric" desc="new Z component of the point" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Aggiorna un punto contenuto in un oggetto SimpleSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto SimpleSpline" />
                                    <param name="pointIndex" type="Numeric" desc="indice del punto da aggiornare" />
                                    <param name="x" type="Numeric" desc="nuovo valore X del punto" />
                                    <param name="y" type="Numeric" desc="nuovo valore Y del punto" />
                                    <param name="z" type="Numeric" desc="nuovo valore Z del punto" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int simpleSplineUpdatePoint(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "simpleSpline.updatePoint", 5))
            {
            EOSOgreSimpleSpline* simpleSpline = EOSG3D::getInstance()->simpleSplineByName(luaL_checkstring(luaRuntime, 1));
            if (simpleSpline != NULL)
                {
                simpleSpline->updatePoint((unsigned short int) lua_tonumber(luaRuntime, 2), Ogre::Vector3(lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4), lua_tonumber(luaRuntime, 5)));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="simpleSplineInterpolate">
                    <language code="en">
                            <brief text="Gets an interpolated point from a SimpleSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the SimpleSpline object" />
                                    <param name="amount" type="Numeric" desc="amount of the interpolation (value between 0.0 and 1.0)" />
                            </params>
                            <returns>
                                    <return name="point_x" type="Numeric" desc="X component of the interpolated position" />
                                    <return name="point_y" type="Numeric" desc="Y component of the interpolated position" />
                                    <return name="point_z" type="Numeric" desc="Z component of the interpolated position" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna un punto interpolato da un oggetto SimpleSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto SimpleSpline" />
                                    <param name="amount" type="Numeric" desc="livello di interpolazione (valore compreso tra 0.0 e 1.0)" />
                            </params>
                            <returns>
                                    <return name="point_x" type="Numeric" desc="componente X della posizione interpolata" />
                                    <return name="point_y" type="Numeric" desc="componente Y della posizione interpolata" />
                                    <return name="point_z" type="Numeric" desc="componente Z della posizione interpolata" />
                            </returns>
                    </language>
            </function>
     */
    int simpleSplineInterpolate(lua_State* luaRuntime)
        {
        Ogre::Vector3 result = Ogre::Vector3::ZERO;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "simpleSpline.interpolate", 2))
            {
            EOSOgreSimpleSpline* simpleSpline = EOSG3D::getInstance()->simpleSplineByName(luaL_checkstring(luaRuntime, 1));
            if (simpleSpline != NULL)
                {
                // bounds check added on January 9, 2007
                Ogre::Real index = (Ogre::Real)lua_tonumber(luaRuntime, 2);
                if (index < 0)
                    {
                    LOGWARNING("[eos."_MODULE_NAME"][simpleSpline.interpolate()] - index out of bounds: %f - Clamped to 0.0.", index);
                    index = 0.0;
                    }
                if (index > 1.0)
                    {
                    LOGWARNING("[eos."_MODULE_NAME"][simpleSpline.interpolate()] - index out of bounds: %f - Clamped to 1.0.", index);
                    index = 1.0;
                    }
                result = simpleSpline->interpolate(index);

                }
            }
        lua_pushnumber(luaRuntime, result.x);
        lua_pushnumber(luaRuntime, result.y);
        lua_pushnumber(luaRuntime, result.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="simpleSplineInterpolateSegment">
                    <language code="en">
                            <brief text="Gets an interpolated point from a segment of a SimpleSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the SimpleSpline object" />
                                    <param name="fromIndex" type="Numeric" desc="the point index to treat as t=0. fromIndex+1 is deemed to be t=1" />
                                    <param name="amount" type="Numeric" desc="amount of the interpolation (value between 0.0 and 1.0)" />
                            </params>
                            <returns>
                                    <return name="point_x" type="Numeric" desc="X component of the interpolated position" />
                                    <return name="point_y" type="Numeric" desc="Y component of the interpolated position" />
                                    <return name="point_z" type="Numeric" desc="Z component of the interpolated position" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna un punto interpolato da un segmento di un oggetto SimpleSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto SimpleSpline" />
                                    <param name="fromIndex" type="Numeric" desc="l'indice del punto da assumere come t=0. fromIndex+1 e' assunto come t=1" />
                                    <param name="amount" type="Numeric" desc="livello di interpolazione (valore compreso tra 0.0 e 1.0)" />
                            </params>
                            <returns>
                                    <return name="point_x" type="Numeric" desc="componente X della posizione interpolata" />
                                    <return name="point_y" type="Numeric" desc="componente Y della posizione interpolata" />
                                    <return name="point_z" type="Numeric" desc="componente Z della posizione interpolata" />
                            </returns>
                    </language>
            </function>
     */
    int simpleSplineInterpolateSegment(lua_State* luaRuntime)
        {
        Ogre::Vector3 result = Ogre::Vector3::ZERO;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "simpleSpline.interpolateSegment", 3))
            {
            EOSOgreSimpleSpline* simpleSpline = EOSG3D::getInstance()->simpleSplineByName(luaL_checkstring(luaRuntime, 1));
            if (simpleSpline != NULL)
                result = simpleSpline->interpolate((unsigned short int) lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3));
            }
        lua_pushnumber(luaRuntime, result.x);
        lua_pushnumber(luaRuntime, result.y);
        lua_pushnumber(luaRuntime, result.z);
        return 3;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="simpleSplineSetAutoCalculate">
                    <language code="en">
                            <brief text="Activates or deactivates 'autoCalculate' function of a SimpleSpline object" />
                            <description text="When autoCalculate=true, the SimpleSpline object recomputes its inner spline every time a point is added|modified|removed" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the SimpleSpline object" />
                                    <param name="autoCalculateStatus" type="Boolean" desc="the status of autoCalculate property to set" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Attiva o disattiva la funzione 'autoCalculate' di un oggetto SimpleSpline" />
                            <description text="Quando autoCalculate=true, l'oggetto SimpleSpline ricalcola la sua spline interna ogni volta che viene aggiunto|modificato|rimosso un punto" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto SimpleSpline" />
                                    <param name="autoCalculateStatus" type="Boolean" desc="the status of autoCalculate property to set" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int simpleSplineSetAutoCalculate(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "simpleSpline.setAutoCalculate", 2))
            {
            EOSOgreSimpleSpline* simpleSpline = EOSG3D::getInstance()->simpleSplineByName(luaL_checkstring(luaRuntime, 1));
            if (simpleSpline != NULL)
                {
                simpleSpline->setAutoCalculate(lua_toboolean(luaRuntime, 2));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="simpleSplineRecalcTangents">
                    <language code="en">
                            <brief text="Forces a recomputation of the spline tangents of a SimpleSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the SimpleSpline object" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Forza un ricalcolo delle tangenti della spline di un oggetto SimpleSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto SimpleSpline" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int simpleSplineRecalcTangents(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "simpleSpline.recalcTangents", 1))
            {
            EOSOgreSimpleSpline* simpleSpline = EOSG3D::getInstance()->simpleSplineByName(luaL_checkstring(luaRuntime, 1));
            if (simpleSpline != NULL)
                {
                simpleSpline->recalcTangents();
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="rotationalSplineCreate">
                    <language code="en">
                            <brief text="Creates a new RotationalSpline object" />
                            <description text="(note: AutoCalculate status default set to true)" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the RotationalSpline object to create" />
                                    <param name="maxPoints" type="Numeric" desc="maximum number of points the spline can have" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Crea un nuovo oggetto RotationalSpline" />
                            <description text="(nota: lo stato iniziale di AutoCalculate e' impostato a true)" />
                            <params>
                                    <param name="splineName" type="String" desc="nome del nuovo oggetto RotationalSpline da creare" />
                                    <param name="maxPoints" type="Numeric" desc="massimo numero di punti possibili per la spline" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int rotationalSplineCreate(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "rotationalSpline.create", 2))
            done = EOSG3D::getInstance()->createRotationalSpline(luaL_checkstring(luaRuntime, 1));
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="rotationalSplineAddPoint">
                    <language code="en">
                            <brief text="Adds a point to a RotationalSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the RotationalSpline object" />
                                    <param name="x" type="Numeric" desc="X component of the quaternion to add" />
                                    <param name="y" type="Numeric" desc="Y component of the quaternion to add" />
                                    <param name="z" type="Numeric" desc="Z component of the quaternion to add" />
                                    <param name="w" type="Numeric" desc="W component of the quaternion to add" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Aggiunge un punto ad un oggetto RotationalSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto RotationalSpline" />
                                    <param name="x" type="Numeric" desc="componente X del quaternione da aggiungere" />
                                    <param name="y" type="Numeric" desc="componente Y del quaternione da aggiungere" />
                                    <param name="z" type="Numeric" desc="componente Z del quaternione da aggiungere" />
                                    <param name="w" type="Numeric" desc="componente W del quaternione da aggiungere" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int rotationalSplineAddPoint(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "rotationalSpline.addPoint", 5))
            {
            EOSOgreRotationalSpline* rotationalSpline = EOSG3D::getInstance()->rotationalSplineByName(luaL_checkstring(luaRuntime, 1));
            if (rotationalSpline != NULL)
                {
                Ogre::Quaternion newPoint = Ogre::Quaternion::IDENTITY;
                newPoint.x = (Ogre::Real)lua_tonumber(luaRuntime, 2);
                newPoint.y = (Ogre::Real)lua_tonumber(luaRuntime, 3);
                newPoint.z = (Ogre::Real)lua_tonumber(luaRuntime, 4);
                newPoint.w = (Ogre::Real)lua_tonumber(luaRuntime, 5);
                //
                int numPoints = rotationalSpline->getNumPoints();
                int maxPoints = rotationalSpline->getMaxPoints();
                if (maxPoints != 0 && numPoints == maxPoints)
                    {
                    bool autoCalcStatus = rotationalSpline->getAutoCalculateStatus();
                    rotationalSpline->setAutoCalculate(false);
                    //
                    // shifts all points by one
                    for (int i = 1; i < numPoints; i++)
                        rotationalSpline->updatePoint(i - 1, rotationalSpline->getPoint(i));
                    //
                    rotationalSpline->updatePoint(numPoints - 1, newPoint);
                    //
                    rotationalSpline->setAutoCalculate(autoCalcStatus);
                    }
                else
                    rotationalSpline->addPoint(newPoint);
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="rotationalSplineGetPoint">
                    <language code="en">
                            <brief text="Returns a point from a RotationalSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the RotationalSpline object" />
                                    <param name="pointIndex" type="Numeric" desc="index of the point to get" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="X component of the point" />
                                    <return name="y" type="Numeric" desc="Y component of the point" />
                                    <return name="z" type="Numeric" desc="Z component of the point" />
                                    <return name="w" type="Numeric" desc="W component of the point" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna un punto da un oggetto RotationalSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto RotationalSpline" />
                                    <param name="pointIndex" type="Numeric" desc="indice del punto da ritornare" />
                            </params>
                            <returns>
                                    <return name="x" type="Numeric" desc="componente X del punto" />
                                    <return name="y" type="Numeric" desc="componente Y del punto" />
                                    <return name="z" type="Numeric" desc="componente Z del punto" />
                                    <return name="w" type="Numeric" desc="componente W del punto" />
                            </returns>
                    </language>
            </function>
     */
    int rotationalSplineGetPoint(lua_State* luaRuntime)
        {
        Ogre::Quaternion result = Ogre::Quaternion::IDENTITY;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "rotationalSpline.getPoint", 2))
            {
            EOSOgreRotationalSpline* rotationalSpline = EOSG3D::getInstance()->rotationalSplineByName(luaL_checkstring(luaRuntime, 1));
            if (rotationalSpline != NULL)
                result = rotationalSpline->getPoint((unsigned short int) lua_tonumber(luaRuntime, 2));
            }
        lua_pushnumber(luaRuntime, result.x);
        lua_pushnumber(luaRuntime, result.y);
        lua_pushnumber(luaRuntime, result.z);
        lua_pushnumber(luaRuntime, result.w);
        return 4;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="rotationalSplineGetNumPoints">
                    <language code="en">
                            <brief text="Returns current number of points contained in a RotationalSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the RotationalSpline object" />
                            </params>
                            <returns>
                                    <return name="totalPoints" type="Numeric" desc="actual total points contained in the RotationalSpline (-1 in case of errors)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il numero corrente di punti contenuti in un oggetto RotationalSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto RotationalSpline" />
                            </params>
                            <returns>
                                    <return name="totalPoints" type="Numeric" desc="numero di punti attualmente contenuti nell'oggetto RotationalSpline (-1 in caso di errori)" />
                            </returns>
                    </language>
            </function>
     */
    int rotationalSplineGetNumPoints(lua_State* luaRuntime)
        {
        int result = -1;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "rotationalSpline.getNumPoints", 1))
            {
            EOSOgreRotationalSpline* rotationalSpline = EOSG3D::getInstance()->rotationalSplineByName(luaL_checkstring(luaRuntime, 1));
            if (rotationalSpline != NULL)
                result = rotationalSpline->getNumPoints();
            }
        lua_pushnumber(luaRuntime, result);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="rotationalSplineSetMaxPoints">
                    <language code="en">
                            <brief text="Sets the maximum number of points the RotationalSpline can contain" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the RotationalSpline object" />
                                    <param name="maxPoints" type="Number" desc="maximum number of points" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Setta il numero massimo di punti contenibili da un determinato oggetto RotationalSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto RotationalSpline" />
                                    <param name="maxPoints" type="Number" desc="limite massimo di punti" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int rotationalSplineSetMaxPoints(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "rotationalSpline.setMaxPoints", 2))
            {
            EOSOgreRotationalSpline* rotationalSpline = EOSG3D::getInstance()->rotationalSplineByName(luaL_checkstring(luaRuntime, 1));
            if (rotationalSpline != NULL)
                {
                rotationalSpline->setMaxPoints((unsigned int) lua_tonumber(luaRuntime, 2));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="rotationalSplineGetMaxPoints">
                    <language code="en">
                            <brief text="Returns the current maximum number of points the RotationalSpline can contain" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the RotationalSpline object" />
                            </params>
                            <returns>
                                    <return name="maxPoints" type="Numeric" desc="current maximum number of points (-1 in case of errors)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna il numero massimo attuale di punti contenibili da un determinato oggetto RotationalSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto RotationalSpline" />
                            </params>
                            <returns>
                                    <return name="maxPoints" type="Numeric" desc="numero massimo attuale di punti (-1 in caso di errori)" />
                            </returns>
                    </language>
            </function>
     */
    int rotationalSplineGetMaxPoints(lua_State* luaRuntime)
        {
        int result = -1;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "rotationalSpline.getMaxPoints", 1))
            {
            EOSOgreRotationalSpline* rotationalSpline = EOSG3D::getInstance()->rotationalSplineByName(luaL_checkstring(luaRuntime, 1));
            if (rotationalSpline != NULL)
                result = rotationalSpline->getMaxPoints();
            }
        lua_pushnumber(luaRuntime, result);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="rotationalSplineClear">
                    <language code="en">
                            <brief text="Removes all points from a RotationalSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the RotationalSpline object" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Rimuove tutti i punti da un oggetto RotationalSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto RotationalSpline" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int rotationalSplineClear(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "rotationalSpline.clear", 1))
            {
            EOSOgreRotationalSpline* rotationalSpline = EOSG3D::getInstance()->rotationalSplineByName(luaL_checkstring(luaRuntime, 1));
            if (rotationalSpline != NULL)
                {
                rotationalSpline->clear();
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="rotationalSplineUpdatePoint">
                    <language code="en">
                            <brief text="Updates a point contained in a RotationalSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the RotationalSpline object" />
                                    <param name="pointIndex" type="Numeric" desc="index of the point to update" />
                                    <param name="x" type="Numeric" desc="new X component of the point" />
                                    <param name="y" type="Numeric" desc="new Y component of the point" />
                                    <param name="z" type="Numeric" desc="new Z component of the point" />
                                    <param name="w" type="Numeric" desc="new W component of the point" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Aggiorna un punto contenuto in un oggetto RotationalSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto RotationalSpline" />
                                    <param name="pointIndex" type="Numeric" desc="indice del punto da aggiornare" />
                                    <param name="x" type="Numeric" desc="nuovo valore X del punto" />
                                    <param name="y" type="Numeric" desc="nuovo valore Y del punto" />
                                    <param name="z" type="Numeric" desc="nuovo valore Z del punto" />
                                    <param name="w" type="Numeric" desc="nuovo valore W del punto" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int rotationalSplineUpdatePoint(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "rotationalSpline.updatePoint", 6))
            {
            EOSOgreRotationalSpline* rotationalSpline = EOSG3D::getInstance()->rotationalSplineByName(luaL_checkstring(luaRuntime, 1));
            if (rotationalSpline != NULL)
                {
                Ogre::Quaternion newPoint = Ogre::Quaternion::IDENTITY;
                newPoint.x = (Ogre::Real)lua_tonumber(luaRuntime, 2);
                newPoint.y = (Ogre::Real)lua_tonumber(luaRuntime, 3);
                newPoint.z = (Ogre::Real)lua_tonumber(luaRuntime, 4);
                newPoint.w = (Ogre::Real)lua_tonumber(luaRuntime, 5);
                rotationalSpline->updatePoint((unsigned short int) lua_tonumber(luaRuntime, 2), newPoint);
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="rotationalSplineInterpolate">
                    <language code="en">
                            <brief text="Gets an interpolated point from a RotationalSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the RotationalSpline object" />
                                    <param name="amount" type="Numeric" desc="amount of the interpolation (value between 0.0 and 1.0)" />
                            </params>
                            <returns>
                                    <return name="point_x" type="Numeric" desc="X component of the interpolated position" />
                                    <return name="point_y" type="Numeric" desc="Y component of the interpolated position" />
                                    <return name="point_z" type="Numeric" desc="Z component of the interpolated position" />
                                    <return name="point_w" type="Numeric" desc="W component of the interpolated position" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna un punto interpolato da un oggetto RotationalSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto RotationalSpline" />
                                    <param name="amount" type="Numeric" desc="livello di interpolazione (valore compreso tra 0.0 e 1.0)" />
                            </params>
                            <returns>
                                    <return name="point_x" type="Numeric" desc="componente X della posizione interpolata" />
                                    <return name="point_y" type="Numeric" desc="componente Y della posizione interpolata" />
                                    <return name="point_z" type="Numeric" desc="componente Z della posizione interpolata" />
                                    <return name="point_w" type="Numeric" desc="componente W della posizione interpolata" />
                            </returns>
                    </language>
            </function>
     */
    int rotationalSplineInterpolate(lua_State* luaRuntime)
        {
        Ogre::Quaternion result = Ogre::Quaternion::IDENTITY;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "rotationalSpline.interpolate", 2))
            {
            EOSOgreRotationalSpline* rotationalSpline = EOSG3D::getInstance()->rotationalSplineByName(luaL_checkstring(luaRuntime, 1));
            if (rotationalSpline != NULL)
                result = rotationalSpline->interpolate((Ogre::Real)lua_tonumber(luaRuntime, 2));
            }
        lua_pushnumber(luaRuntime, result.x);
        lua_pushnumber(luaRuntime, result.y);
        lua_pushnumber(luaRuntime, result.z);
        lua_pushnumber(luaRuntime, result.w);
        return 4;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="rotationalSplineInterpolateSegment">
                    <language code="en">
                            <brief text="Gets an interpolated point from a segment of a RotationalSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the RotationalSpline object" />
                                    <param name="fromIndex" type="Numeric" desc="the point index to treat as t=0. fromIndex+1 is deemed to be t=1" />
                                    <param name="amount" type="Numeric" desc="amount of the interpolation (value between 0.0 and 1.0)" />
                            </params>
                            <returns>
                                    <return name="point_x" type="Numeric" desc="X component of the interpolated position" />
                                    <return name="point_y" type="Numeric" desc="Y component of the interpolated position" />
                                    <return name="point_z" type="Numeric" desc="Z component of the interpolated position" />
                                    <return name="point_w" type="Numeric" desc="W component of the interpolated position" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna un punto interpolato da un segmento di un oggetto RotationalSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto RotationalSpline" />
                                    <param name="fromIndex" type="Numeric" desc="l'indice del punto da assumere come t=0. fromIndex+1 e' assunto come t=1" />
                                    <param name="amount" type="Numeric" desc="livello di interpolazione (valore compreso tra 0.0 e 1.0)" />
                            </params>
                            <returns>
                                    <return name="point_x" type="Numeric" desc="componente X della posizione interpolata" />
                                    <return name="point_y" type="Numeric" desc="componente Y della posizione interpolata" />
                                    <return name="point_z" type="Numeric" desc="componente Z della posizione interpolata" />
                                    <return name="point_w" type="Numeric" desc="componente W della posizione interpolata" />
                            </returns>
                    </language>
            </function>
     */
    int rotationalSplineInterpolateSegment(lua_State* luaRuntime)
        {
        Ogre::Quaternion result = Ogre::Quaternion::IDENTITY;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "rotationalSpline.interpolateSegment", 3))
            {
            EOSOgreRotationalSpline* rotationalSpline = EOSG3D::getInstance()->rotationalSplineByName(luaL_checkstring(luaRuntime, 1));
            if (rotationalSpline != NULL)
                result = rotationalSpline->interpolate((unsigned short int) lua_tonumber(luaRuntime, 2), (Ogre::Real)lua_tonumber(luaRuntime, 3));
            }
        lua_pushnumber(luaRuntime, result.x);
        lua_pushnumber(luaRuntime, result.y);
        lua_pushnumber(luaRuntime, result.z);
        lua_pushnumber(luaRuntime, result.w);
        return 4;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="rotationalSplineSetAutoCalculate">
                    <language code="en">
                            <brief text="Activates/deactivates 'autoCalculate' function of a RotationalSpline object" />
                            <description text="When autoCalculate=true, the RotationalSpline object recomputes its inner spline every time a point is added|modified|removed" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the RotationalSpline object" />
                                    <param name="autoCalculateStatus" type="Boolean" desc="the status of autoCalculate property to set" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Attiva/disattiva la funzione 'autoCalculate' di un oggetto RotationalSpline" />
                            <description text="Quando autoCalculate=true, l'oggetto RotationalSpline ricalcola la sua spline interna ogni volta che viene aggiunto|modificato|rimosso un punto" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto RotationalSpline" />
                                    <param name="autoCalculateStatus" type="Boolean" desc="the status of autoCalculate property to set" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int rotationalSplineSetAutoCalculate(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "rotationalSpline.setAutoCalculate", 2))
            {
            EOSOgreRotationalSpline* rotationalSpline = EOSG3D::getInstance()->rotationalSplineByName(luaL_checkstring(luaRuntime, 1));
            if (rotationalSpline != NULL)
                {
                rotationalSpline->setAutoCalculate(lua_toboolean(luaRuntime, 2));
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="rotationalSplineRecalcTangents">
                    <language code="en">
                            <brief text="Forces a recomputation of the spline tangents of a RotationalSpline object" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="name of the RotationalSpline object" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Forza un ricalcolo delle tangenti della spline di un oggetto RotationalSpline" />
                            <description text="" />
                            <params>
                                    <param name="splineName" type="String" desc="nome dell'oggetto RotationalSpline" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int rotationalSplineRecalcTangents(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "rotationalSpline.recalcTangents", 1))
            {
            EOSOgreRotationalSpline* rotationalSpline = EOSG3D::getInstance()->rotationalSplineByName(luaL_checkstring(luaRuntime, 1));
            if (rotationalSpline != NULL)
                {
                rotationalSpline->recalcTangents();
                done = true;
                }
            }
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    /**
            <function name="compositorAdd">
                    <language code="en">
                            <brief text="" />
                            <description text="Add a Compositor (postrender effects) to default viewport" />
                            <params>
                                    <param name="compositorName" type="String" desc="name of the Compositor object to add" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in case of success, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="" />
                            <description text="Aggiunge un Compositor (effetti di postrender) alla viewport di default" />
                            <params>
                                    <param name="compositorName" type="String" desc="nome dell'oggetto Compositor da aggiungere" />
                            </params>
                            <returns>
                                    <return name="" type="Boolean" desc="true in caso di successo, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    int compositorAdd(lua_State* luaRuntime)
        {
        bool done = false;
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "compositor.add", 1))
            done = (Ogre::CompositorManager::getSingleton().addCompositor(EOSG3D::getInstance()->getDefaultViewport(), luaL_checkstring(luaRuntime, 1)) != NULL);
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="compositorRemove">
                    <language code="en">
                            <brief text="" />
                            <description text="Removes a Compositor (postrender effects) from default viewport" />
                            <params>
                                    <param name="compositorName" type="String" desc="name of the Compositor object to remove" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="" />
                            <description text="Rimuove un Compositor (effetti di postrender) dalla viewport di default" />
                            <params>
                                    <param name="compositorName" type="String" desc="nome dell'oggetto Compositor da rimuovere" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int compositorRemove(lua_State* luaRuntime)
        {
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "compositor.remove", 1))
            Ogre::CompositorManager::getSingleton().removeCompositor(EOSG3D::getInstance()->getDefaultViewport(), luaL_checkstring(luaRuntime, 1));
        return 0;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="compositorSetActive">
                    <language code="en">
                            <brief text="" />
                            <description text="Sets the activity status of a Compositor (postrender effects) on default viewport" />
                            <params>
                                    <param name="compositorName" type="String" desc="name of the Compositor object to activate/deactivate" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="" />
                            <description text="Setta lo stato di un Compositor (effetti di postrender) sulla viewport di default" />
                            <params>
                                    <param name="compositorName" type="String" desc="nome dell'oggetto Compositor da attivare/disattivare" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int compositorSetActive(lua_State* luaRuntime)
        {
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "compositor.setActive", 2))
            Ogre::CompositorManager::getSingleton().setCompositorEnabled(EOSG3D::getInstance()->getDefaultViewport(), luaL_checkstring(luaRuntime, 1), lua_toboolean(luaRuntime, 2));
        return 0;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="resourceManagerUnloadAllUnreferenced">
                    <language code="en">
                            <brief text="Unload all resources which are not referenced by any other object" />
                            <description text="" />
                            <params>
                                    <param name="reloadableOnly" type="Boolean" desc="If true, only unloads resources which can be subsequently automatically reloaded" />
                            </params>
                            <returns>
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Elimina tutte le risorse non piu' in uso (non referenziate da alcun oggetto)" />
                            <description text="" />
                            <params>
                                    <param name="reloadableOnly" type="Boolean" desc="Se true, elimina solo le risorse che possono essere successivamente ricaricate in modo automatico" />
                            </params>
                            <returns>
                            </returns>
                    </language>
            </function>
     */
    int resourceManagerUnloadAllUnreferenced(lua_State* luaRuntime)
        {
        if (checkModuleInitialized() && EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "resourceManagerUnloadAllUnreferenced", 1))
            {
            bool reloadableOnly = lua_toboolean(luaRuntime, 1);
            EOSG3D::getInstance()->unloadUnreferencedTextures(reloadableOnly);
            EOSG3D::getInstance()->unloadUnreferencedMeshes(reloadableOnly);
            }
        //
        return 0;
        }

    // -----------------------------------------------------------------------------
    // ========================== END LIBRARY FUNCTIONS ============================

    class KeyValue
        {
    public:

        KeyValue(const char* key, int value)
            {
            this->key = strdup(key);
            this->value = value;
            }

        ~KeyValue()
            {
            free(this->key);
            }
        char* key;
        int value;
            };

    std::vector<KeyValue*> keysymbols;

    void loadModuleSymbols()
        {
        // collision shapes symbols:
        keysymbols.push_back(new KeyValue("COLLISIONSHAPE_SPHERE", COLLISIONSHAPE_SPHERE));
        keysymbols.push_back(new KeyValue("COLLISIONSHAPE_BOX", COLLISIONSHAPE_BOX));
        keysymbols.push_back(new KeyValue("COLLISIONSHAPE_MESH", COLLISIONSHAPE_MESH));
        //
        keysymbols.push_back(new KeyValue("LBX_SOURCE1", _LBX_SOURCE1));
        keysymbols.push_back(new KeyValue("LBX_SOURCE2", _LBX_SOURCE2));
        keysymbols.push_back(new KeyValue("LBX_MODULATE", _LBX_MODULATE));
        keysymbols.push_back(new KeyValue("LBX_MODULATE_X2", _LBX_MODULATE_X2));
        keysymbols.push_back(new KeyValue("LBX_MODULATE_X4", _LBX_MODULATE_X4));
        keysymbols.push_back(new KeyValue("LBX_ADD", _LBX_ADD));
        keysymbols.push_back(new KeyValue("LBX_ADD_SIGNED", _LBX_ADD_SIGNED));
        keysymbols.push_back(new KeyValue("LBX_ADD_SMOOTH", _LBX_ADD_SMOOTH));
        keysymbols.push_back(new KeyValue("LBX_SUBTRACT", _LBX_SUBTRACT));
        keysymbols.push_back(new KeyValue("LBX_BLEND_DIFFUSE_ALPHA", _LBX_BLEND_DIFFUSE_ALPHA));
        keysymbols.push_back(new KeyValue("LBX_BLEND_TEXTURE_ALPHA", _LBX_BLEND_TEXTURE_ALPHA));
        keysymbols.push_back(new KeyValue("LBX_BLEND_CURRENT_ALPHA", _LBX_BLEND_CURRENT_ALPHA));
        keysymbols.push_back(new KeyValue("LBX_BLEND_MANUAL", _LBX_BLEND_MANUAL));
        keysymbols.push_back(new KeyValue("LBX_DOTPRODUCT", _LBX_DOTPRODUCT));
        keysymbols.push_back(new KeyValue("LBX_BLEND_DIFFUSE_COLOUR", _LBX_BLEND_DIFFUSE_COLOUR));
        //
        keysymbols.push_back(new KeyValue("LBS_CURRENT", _LBS_CURRENT));
        keysymbols.push_back(new KeyValue("LBS_TEXTURE", _LBS_TEXTURE));
        keysymbols.push_back(new KeyValue("LBS_DIFFUSE", _LBS_DIFFUSE));
        keysymbols.push_back(new KeyValue("LBS_SPECULAR", _LBS_SPECULAR));
        keysymbols.push_back(new KeyValue("LBS_MANUAL", _LBS_MANUAL));
        //
        char tmpstr[120];
        int totalsymbols = keysymbols.size();
        for (int i = 0; i < totalsymbols; i++)
            {
            sprintf(tmpstr, "eos.%s.%s = %d", _MODULE_NAME, keysymbols[i]->key, keysymbols[i]->value);
            EOSLuaRuntime::getInstance()->executeLuaChunk(tmpstr);
            delete keysymbols[i];
            }
        keysymbols.clear();
        }


    // ======================== LUA FUNCTIONS REGISTRATION =========================

    // ==== Here we declare an array with all functions and their respective names.
    // ==== This array has elements of type luaL_reg, which is a structure with
    // ==== two fields: a string and a function pointer.

    const struct luaL_reg fn_table_module [] = {
        {"cleanup", moduleCleanup},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_main [] = {
        {"initialize", initialize},
        {"update", update},
        {"render", render},
        {"shutdown", shutdown},
        {"createRenderSurface", createRenderSurface},
        {"pointToScreenCoordinates", pointToScreenCoordinates},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_world [] = {
        {"setSkyBox", worldSetSkyBox},
        {"setSkyDome", worldSetSkyDome},
        {"setAmbientLight", worldSetAmbientLight},
        {"getAmbientLight", worldGetAmbientLight},
        {"loadDotScene", worldLoadDotScene},
        {"setFog", worldSetFog},
        {"clear", worldClear},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_camera [] = {
        {"setPosition", cameraSetPosition},
        {"getPosition", cameraGetPosition},
        {"getOrientation", cameraGetOrientation},
        {"setOrientation", cameraSetOrientation},
        {"setFOV", cameraSetFOV},
        {"lookAt", cameraLookAt},
        {"translate", cameraTranslate},
        {"rotateByAngles", cameraRotateByAngles},
        {"rotateByAxisAngle", cameraRotateByAxisAngle},
        {"getNodeName", cameraGetNodeName},
        {"setAutoTracking", cameraSetAutoTracking},
        {"disableAutoTracking", cameraDisableAutoTracking},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_node [] = {
        {"exists", nodeExists},
        {"create", nodeCreate},
        {"reparent", nodeReparent},
        {"getParentNodeName", nodeGetParentNodeName},
        {"getPosition", nodeGetPosition},
        {"setPosition", nodeSetPosition},
        {"translate", nodeTranslate},
        {"setScale", nodeSetScale},
        {"setVisible", nodeSetVisible},
        {"toggleVisibility", nodeToggleVisibility},
        {"getOrientation", nodeGetOrientation},
        {"setOrientation", nodeSetOrientation},
        {"resetOrientation", nodeResetOrientation},
        {"rotateByAxisAngle", nodeRotateByAxisAngle},
        {"rotateByQuaternion", nodeRotateByQuaternion},
        {"rotateByAngles", nodeRotateByAngles},
        {"setXAxisDirection", nodeSetXAxisDirection},
        {"setYAxisDirection", nodeSetYAxisDirection},
        {"setZAxisDirection", nodeSetZAxisDirection},
        {"getXAxisDirection", nodeGetXAxisDirection},
        {"getYAxisDirection", nodeGetYAxisDirection},
        {"getZAxisDirection", nodeGetZAxisDirection},
        {"getVectorToNode", nodeGetVectorToNode},
        {"getVectorToPoint", nodeGetVectorToPoint},
        {"getDistanceToNode", nodeGetDistanceToNode},
        {"bindToExternalObject", nodeBindToExternalObject},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_entity [] = {
        {"load", entityLoad},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_particlesystem [] = {
        {"load", particleSystemLoad},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_collision [] = {
            {"createBox",                    collisionBoxCreate},
            {"createSphere",                 collisionSphereCreate},
            {"createRay",                    collisionRayCreate},
            //{"collisionBoxFromEntity",           collisionBoxFromEntity},
            {"sphereToEntity",               collisionSphereToEntity},
            {"trimeshFromEntity",            collisionTrimeshFromEntity},
            {"objectSetActive",              collisionObjectSetActive},
            {"objectIsActive",               collisionObjectIsActive},
            {NULL, NULL}  // <-- end-of-table sentinel
            };

    const struct luaL_reg fn_table_object [] = {
        {"exists", objectExists},
        {"remove", objectRemove},
        {"setCastShadow", objectSetCastShadow},
        {"setVisible", objectSetVisible},
        {"isVisible", objectIsVisible},
        {"getNodeName", objectGetNodeName},
        {"showBoundingBox", objectShowBoundingBox},
        {"setMaterial", objectSetMaterial},
        {"getMinimumSize", objectGetMinimumSize},
        {"getMaximumSize", objectGetMaximumSize},
        {"getScreenBoundingRect", objectGetScreenBoundingRect},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_objectanimation [] = {
        {"enable", objectAnimationEnable},
        {"disable", objectAnimationDisable},
        {"setPosition", objectAnimationSetPosition},
        {"getPosition", objectAnimationGetPosition},
        {"setWeight", objectAnimationSetWeight},
        {"getWeight", objectAnimationGetWeight},
        {"setSpeed", objectAnimationSetSpeed},
        {"getSpeed", objectAnimationGetSpeed},
        {"setDirection", objectAnimationSetDirection},
        {"getDirection", objectAnimationGetDirection},
        {"freeze", objectAnimationFreeze},
        {"isFrozen", objectAnimationIsFrozen},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_objectanimationbone [] = {
        {"getPosition", objectBoneGetPosition},
        {"getOrientation", objectBoneGetOrientation},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_light [] = {
        {"create", lightCreate},
        {"remove", lightRemove},
        {"getNodeName", lightGetNodeName},
        {"setTypePoint", lightSetTypePoint},
        {"setTypeDirectional", lightSetTypeDirectional},
        {"setTypeSpot", lightSetTypeSpot},
        {"getType", lightGetType},
        {"setDiffuseColor", lightSetDiffuseColor},
        {"getDiffuseColor", lightGetDiffuseColor},
        {"setSpecularColor", lightSetSpecularColor},
        {"getSpecularColor", lightGetSpecularColor},
        {"setAttenuation", lightSetAttenuation},
        {"getAttenuation", lightGetAttenuation},
        {"setPosition", lightSetPosition},
        {"getPosition", lightGetPosition},
        {"setDirection", lightSetDirection},
        {"getDirection", lightGetDirection},
        {"setSpotParams", lightSetSpotParams},
        {"getSpotParams", lightGetSpotParams},
        {"setVisible", lightSetVisible},
        {"isVisible", lightIsVisible},
        {"setCastShadows", lightSetCastShadows},
        {"getCastShadows", lightGetCastShadows},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_material [] = {
        {"create", materialCreate},
        {"setAmbient", materialSetAmbient},
        {"getAmbient", materialGetAmbient},
        {"setDiffuse", materialSetDiffuse},
        {"getDiffuse", materialGetDiffuse},
        {"setSpecular", materialSetSpecular},
        {"getSpecular", materialGetSpecular},
        {"setSelfIllumination", materialSetSelfIllumination},
        {"getSelfIllumination", materialGetSelfIllumination},
        {"setShininess", materialSetShininess},
        {"getShininess", materialGetShininess},
        {"setAlphaOperation", materialSetAlphaOperation},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_ribbontrail [] = {
        {"create", ribbonTrailCreate},
        {"setMaterial", ribbonTrailSetMaterial},
        {"addNode", ribbonTrailAddNode},
        {"setLength", ribbonTrailSetLength},
        {"setNumberOfChains", ribbonTrailSetNumberOfChains},
        {"setMaxChainElements", ribbonTrailSetMaxChainElements},
        {"setInitialColor", ribbonTrailSetInitialColor},
        {"setColorChange", ribbonTrailSetColorChange},
        {"setInitialWidth", ribbonTrailSetInitialWidth},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_path [] = {
        {"create", pathCreate},
        {"setInterpolationLinear", pathSetInterpolationLinear},
        {"setInterpolationSpline", pathSetInterpolationSpline},
        {"createNodeTrack", pathCreateNodeTrack},
        {"trackCreateKeyFrame", pathTrackCreateKeyFrame},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_rendertexture [] = {
        {"create", renderTextureCreate},
        {"toMaterial", renderTextureToMaterial},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_overlay [] = {
        {"create", overlayCreate},
        {"setVisible", overlaySetVisible},
        {"setZOrder", overlaySetZOrder},
        {"add2DElement", overlayAdd2DElement},
        {"add3DSceneNode", overlayAdd3DSceneNode},
        {"createContainer", overlayContainerCreate},
        {"containerAddChild", overlayContainerAddChild},
        {"textAreaElementCreate", overlayTextAreaElementCreate},
        {"textAreaSetFontName", overlayTextAreaSetFontName},
        {"textAreaSetFontSize", overlayTextAreaSetFontSize},
        {"textAreaSetCaption", overlayTextAreaSetCaption},
        {"textAreaGetCaption", overlayTextAreaGetCaption},
        {"textAreaSetColor", overlayTextAreaSetColor},
        {"textAreaSetColorTop", overlayTextAreaSetColorTop},
        {"textAreaSetColorBottom", overlayTextAreaSetColorBottom},
        {"textAreaGetColor", overlayTextAreaGetColor},
        {"textAreaGetColorTop", overlayTextAreaGetColorTop},
        {"textAreaGetColorBottom", overlayTextAreaGetColorBottom},
        {"elementSetVisible", overlayElementSetVisible},
        {"elementSetDimensions", overlayElementSetDimensions},
        {"elementSetPosition", overlayElementSetPosition},
        {"elementSetMaterial", overlayElementSetMaterial},
        {"elementSetMetricsAbsolute", overlayElementSetMetricsAbsolute},
        {"elementSetMetricsRelative", overlayElementSetMetricsRelative},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_billboardset [] = {
        {"create", billboardSetCreate},
        {"setMaterial", billboardSetSetMaterial},
        {"setDefaultDimensions", billboardSetSetDefaultDimensions},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_billboard [] = {
        {"create", billboardCreate},
        {"getRotation", billboardGetRotation},
        {"setRotation", billboardSetRotation},
        {"getPosition", billboardGetPosition},
        {"setPosition", billboardSetPosition},
        {"setDimensions", billboardSetDimensions},
        {"getDimensions", billboardGetDimensions},
        {"resetDimensions", billboardResetDimensions},
        {"setColor", billboardSetColor},
        {"getColor", billboardGetColor},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_simplespline [] = {
        {"create", simpleSplineCreate},
        {"addPoint", simpleSplineAddPoint},
        {"getPoint", simpleSplineGetPoint},
        {"getNumPoints", simpleSplineGetNumPoints},
        {"clear", simpleSplineClear},
        {"updatePoint", simpleSplineUpdatePoint},
        {"interpolate", simpleSplineInterpolate},
        {"interpolateSegment", simpleSplineInterpolateSegment},
        {"setAutoCalculate", simpleSplineSetAutoCalculate},
        {"recalcTangents", simpleSplineRecalcTangents},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_rotationalspline [] = {
        {"create", rotationalSplineCreate},
        {"addPoint", rotationalSplineAddPoint},
        {"getPoint", rotationalSplineGetPoint},
        {"getNumPoints", rotationalSplineGetNumPoints},
        {"setMaxPoints", rotationalSplineSetMaxPoints},
        {"getMaxPoints", rotationalSplineGetMaxPoints},
        {"clear", rotationalSplineClear},
        {"updatePoint", rotationalSplineUpdatePoint},
        {"interpolate", rotationalSplineInterpolate},
        {"interpolateSegment", rotationalSplineInterpolateSegment},
        {"setAutoCalculate", rotationalSplineSetAutoCalculate},
        {"recalcTangents", rotationalSplineRecalcTangents},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_compositor [] = {
        {"add", compositorAdd},
        {"remove", compositorRemove},
        {"setActive", compositorSetActive},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_resourcemanager [] = {
        {"unloadAllUnreferenced", resourceManagerUnloadAllUnreferenced},
        {NULL, NULL} // <-- end-of-table sentinel
        };


    // ==== Finally, we declare a main (initialization) function, using luaL_openlib:

    int _INIT_(lua_State* luaRuntime)
        {
        EOSLuaRuntime::getInstance()->setRuntime(luaRuntime);
        //
        //
        // Never touch the manifest structure code below!
        EOSLuaRuntime::getInstance()->executeLuaChunk(
                                                      "eos."_MODULE_NAME" = {}\
				 eos.module._new = eos."_MODULE_NAME"\
				 eos.module._new.module                        = {}\
				 eos.module._new.module.info                   = {}\
				 eos.module._new.module.info.name              = '"_MODULE_NAME".eosmodule'\
				 eos.module._new.module.info.tableName         = 'eos."_MODULE_NAME"'\
				 eos.module._new.module.info.version           = '"_MODULE_VERSION"'\
				 eos.module._new.module.info.date              = '"_MODULE_DATE"'\
				 eos.module._new.module.info.description       = {}\
				 eos.module._new.module.info.description.short = '"_MODULE_DESC_SHORT"'\
				 eos.module._new.module.info.description.long  = '"_MODULE_DESC_LONG"'\
				 eos.module._new.module.info.author            = {}\
				 eos.module._new.module.info.author.name       = '"_MODULE_AUTHOR_NAME"'\
				 eos.module._new.module.info.author.website    = '"_MODULE_AUTHOR_WEBSITE"'\
				 eos.module._new.module.reservedFunctions         = {}\
				 eos.module._new.module.reservedFunctions.init    = ''\
				 eos.module._new.module.reservedFunctions.cleanup = 'eos."_MODULE_NAME".module.cleanup'\
				");

        // registers "eos.*.module" functions
        luaL_register(luaRuntime, "eos."_MODULE_NAME".module", fn_table_module);
        // registers eos.* main functions
        luaL_register(luaRuntime, "eos."_MODULE_NAME, fn_table_main);

        // registers "eos.g3d.world" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".world = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".world", fn_table_world);
        // registers "eos.g3d.camera" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".camera = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".camera", fn_table_camera);
        // registers "eos.g3d.node" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".node = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".node", fn_table_node);
        // registers "eos.g3d.entity" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".entity = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".entity", fn_table_entity);
        // registers "eos.g3d.particlesystem" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".particleSystem = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".particleSystem", fn_table_particlesystem);
        // registers "eos.g3d.collision" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".collision = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".collision", fn_table_collision);
        // registers "eos.g3d.object" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".object = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".object", fn_table_object);
        // registers "eos.g3d.object.animation" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".object.animation = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".object.animation", fn_table_objectanimation);
        // registers "eos.g3d.object.animation.bone" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".object.animation.bone = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".object.animation.bone", fn_table_objectanimationbone);
        // registers "eos.g3d.light" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".light = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".light", fn_table_light);
        // registers "eos.g3d.material" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".material = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".material", fn_table_material);
        // registers "eos.g3d.ribbonTrail" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".ribbonTrail = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".ribbonTrail", fn_table_ribbontrail);
        // registers "eos.g3d.path" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".path = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".path", fn_table_path);
        // registers "eos.g3d.renderTexture" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".renderTexture = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".renderTexture", fn_table_rendertexture);
        // registers "eos.g3d.overlay" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".overlay = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".overlay", fn_table_overlay);
        // registers "eos.g3d.billboardSet" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".billboardSet = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".billboardSet", fn_table_billboardset);
        // registers "eos.g3d.billboard" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".billboard = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".billboard", fn_table_billboard);
        // registers "eos.g3d.simpleSpline" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".simpleSpline = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".simpleSpline", fn_table_simplespline);
        // registers "eos.g3d.rotationalSpline" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".rotationalSpline = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".rotationalSpline", fn_table_rotationalspline);
        // registers "eos.g3d.compositor" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".compositor = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".compositor", fn_table_compositor);
        // registers "eos.g3d.resourceManager" functions
        EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".resourceManager = {}");
        luaL_register(luaRuntime, "eos."_MODULE_NAME".resourceManager", fn_table_resourcemanager);
        //
        // loads module symbols
        loadModuleSymbols();
        //
        return 1;
        }


    // -----------------------------------------------------------------------------

    } // extern "C" closure




/* Changelog:

v. 0.9.5
   Nov 11, 2012:
    .changed "createRenderTarget" function name to "createRenderSurface"
    .changed again module namespace from "3d" to "g3d" (lua doesn't allow variables starting with numbers)
    .changed module namespace from "ogre" to "3d"
    .now module uses TinyXml2 instead of TinyXml
v. 0.9.4
   Oct 10, 2009:
    .completely removed ODE collision code from module
v. 0.9.2
   Sep 26, 2009:
    .changed createRenderTarget: now returns an EOSObject containing a GL_Texture handle
    .removed support for deprecated EOS Datasources export mechanism
   Sep 03, 2009:
    .added support for EOS Datasources export mechanism
    .added support for multipe render targets
   Aug 15, 2009:
    .removed 'initializeOnWindow' function, and moved its code to 'initialize' function. Now is not possible to open OGRE in its window, anymore.
   Aug 14, 2009:
    .first succesful build
    .finished adapting old elf module code to new eos architecture

===============================================================================
 ELF Module History:

v. 0.9.2
        Jan 31, 2008:
    .fixed scene clear code in worldClear() -> EOSOgre::sceneClear()
        Oct 5, 2007:
    .added resourceManagerUnloadAllUnreferenced function
    .added unloadUnreferencedMeshes() method to EOSG3D
    .added unloadUnreferencedTextures() method to EOSG3D
    .changed objects/node destroy order in EOSOgreObject::destroy()
        Jul 26, 2007:
    .added objectAnimationGetPosition function
  Apr 11, 2007:
    .added sceneNode->removeAndDestroyAllChildren() in EOSOgreObject::destroy()
  Mar 12, 2007:
    .added extra return value in overlayTextAreaGetColor, overlayTextAreaGetColorTop, overlayTextAreaGetColorBottom: now returns 4 values, including alpha
    .added extra parameter in overlayTextAreaSetColor, overlayTextAreaSetColorTop, overlayTextAreaSetColorBottom: now requires 5 values, including alpha
  Mar 09, 2007:
    .added function materialGetShininess
    .added function materialSetShininess
    .added function materialGetSEOSIllumination
    .added function materialSetSEOSIllumination
    .added function materialGetSpecular
    .added function materialGetDiffuse
    .added function materialGetAmbient
        Mar 06, 2007:
                .upgraded Ogre to version 1.4.0RC2 (Eihort)
v. 0.9.1
        Feb 28, 2007:
                .added automatic scene node caching and reuse system
        Jan 09, 2007:
                .added bounds check in function simpleSplineInterpolate
        Dec 21, 2006:
                .updated code in the function pointToScreenCoordinates
        Dec 13, 2006:
                .fixed function objectBoneGetPosition. Now returns absolute position
        Nov 30, 2006:
                .added function collisionObjectIsActive
                .added function collisionObjectSetActive to set the activity of the collision objects
        Nov 29, 2006:
                .fixed the #QNAN0 problem in EOSOdeCollisionWorld.cpp (when generating collision messages)
        Nov 08, 2006:
                .fixed a bug in cameraGetOrientation: now really returns absolute camera orientation (was returning relative orientation)
                .fixed a bug in cameraGetPosition: now really returns absolute camera position (was returning relative position)
        Nov 07, 2006:
                .added function objectBoneGetPosition
                .changed function objectAnimationRestart to objectAnimationSetPosition
        Nov 06, 2006:
                .added function objectAnimationGetDirection
                .added function objectAnimationSetDirection
                .added function objectAnimationGetSpeed
                .added function objectAnimationSetSpeed
                .started EOSOgreObject support for multiple skeletal animations
                .added function objectAnimationFreeze
                .added function objectAnimationGetWeight
                .added function objectAnimationSetWeight
                .added function objectAnimationRestart
                .added function objectAnimationDisable
                .added function objectAnimationEnable
                .started new skeletal animation control function set
                .made function objectSetAnimation DEPRECATED
v. 0.9.0.1
        Oct 27, 2006:
                .fixed a small bug in overlayElementSetVisible()
v. 0.9.0
        Oct 25, 2006:
                .added code to recreate default camera and its node after a worldClear() issue
        Oct 16, 2006:
                .added function objectRemove
        Oct 12, 2006:
                .added function materialSetSpecular
                .added function materialSetDiffuse
                .added function materialSetAmbient
                .added function materialCreate
                .started Material framework handling support
        Oct 09, 2006:
                .added function ribbonTrailSetInitialWidth
                .added function ribbonTrailSetColorChange
                .added function ribbonTrailSetInitialColor
                .added function ribbonTrailSetMaxChainElements
                .added function ribbonTrailSetNumberOfChains
                .added function ribbonTrailSetLength
                .added function ribbonTrailAddNode
                .added function ribbonTrailSetMaterial
                .added function ribbonTrailCreate, to create RibbonTrail objects
                .started RibbonTrail support
                .extended EOSOgreObject class, to manage also RibbonTrail object types
                .added function nodeGetParentNodeName, to get the parent name of nodes
                .added missing code in objectSetVisible, for ParticleSystem objects
                .added function nodeToggleVisibility, to toggle the visibility of a node or a node hierarchy
                .added function nodeSetVisible, to set the visibility of a node or a node hierarchy
        Sep 22, 2006:
                .added function worldClear() to clear all scene and loaded objects (entities, particleSystems, collisionShapes, sceneNodes...)
        Sep 19, 2006:
                .added collisionRayCreate function and ode collision ray support EOSOdeCollisionWorld
        Sep  7, 2006:
                .added createEmptyObject method in EOSG3D to automatically create and add to objects vector a new empty EOSOgreObject
                .added requestSceneNode method in EOSOgreObject to make it automatically request a scene node
                .integrated ODE library for collision detection purposes
        Aug 28, 2006:
                .added functions compositorAdd, compositorRemove, compositorSetActive for Postrender effects control
                .added Ogre::Viewport* defaultViewport as class member of EOSG3D
        Aug 24, 2006:
                .Upgraded Ogre to version 1.2.2 (Dagon)
                .updated settings file format
                .Removed obsolete propertySet() function
        Aug 22, 2006:
                .Added methods objectGetMinimumSize and objectGetMaximumSize
                .Fixed a bug in nodeResetOrientation in parameters count check
v. 0.8.9
        Aug 5, 2006:
          .Changed lightCreate method: now returns the name of the new created node where light is attached
        Aug 3, 2006:
                .Added cameraRotateByAxisAngle method, to rotate camera using an axis and an angle
                .Added an extra parameter to cameraTranslate to choose if translate along absolute or relative axes
        Aug 2, 2006:
                .Fixed a probable bug in nodeSetPosition;
                .Fixed a probable bug in nodeSetOrientation;
                .Revisited the accessors to get/set nodes positions/orientations. Now everywhere the boolean parameter
                 means ABSOLUTE (WORLD) coordinate and false means RELATIVE (to parent node) coordinates;
                .added an extra boolean parameter to nodeGetVectorToNode to choose to use absolute/relative node positions;
                .added an extra boolean parameter to nodeGetVectorToPoint to choose to use absolute/relative node position;
                .added an extra boolean parameter to nodeGetDistanceToNode to choose to use absolute/relative node positions;
v. 0.8.8
        Jun 13, 2006:
                .hardened error tracking: now EOSG3D::objectByName and EOSG3D::nodeByName traces errors on the log file
                .fixed "rotateByAngles" functions: now accepts rotations in degrees
        Jun 12, 2006:
                changed function cameraRotate to cameraRotateByAngles
                fixed camera position/orientation functions: now operates on the camera's parentNode
                fixed light position/orientation functions: now operates on the light's parentNode
        Jun 8, 2006
                added function to get Light object node name:
                        lightGetNodeName
          now, by default all Light objects are attached to nodes
                added environmental fog setting function:
                        worldSetFog
                added camera node name get function:
                        cameraGetNodeName
                set default camera default attached to a scene node
                added camera autotracking control functions:
                        cameraSetAutoTracking
                        cameraDisableAutoTracking

        Jun 8, 2006 - added path control functions:
                pathCreate
                pathSetInterpolationLinear
                pathSetInterpolationSpline
                pathCreateNodeTrack
                pathTrackCreateKeyFrame

        May 31, 2006 - added Light control functions:
                lightCreate
                lightRemove
                lightSetTypePoint
                lightSetTypeDirectional
                lightSetTypeSpot
                lightGetType
                lightSetDiffuseColor
                lightGetDiffuseColor
                lightSetSpecularColor
                lightGetSpecularColor
                lightSetAttenuation
                lightGetAttenuation
                lightSetPosition
                lightGetPosition
                lightSetDirection
                lightGetDirection
                lightSetSpotParams
                lightGetSpotParams
                lightSetVisible
                lightIsVisible
                lightSetCastShadows
                lightGetCastShadows
        upgraded Ogre from 1.0.6 to 1.0.7

v. 0.8.2 - February 08, 2006
        First public release (in EOS DevKit 1.0)

 */
