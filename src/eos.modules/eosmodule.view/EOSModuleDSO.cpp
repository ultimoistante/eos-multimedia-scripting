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

#include "View.h"
#include "EOSLuaRuntime.h"
#include "EOSObject.h"
#include "eos_data_containers.h"

#include <stdio.h>
#include <string>

extern "C"
    {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"


    /*- @module eos.view
     *  @brief 3D OpenGL-powered Compositing Window Manager module for EOS Scripting Environment
     */

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    //
    // Module manifest structure. Developer must edit just the #define values below.
    //
#define _MODULE_NAME            "view"
#define _MODULE_VERSION         "0.5"
#define _MODULE_DATE            "Novmber 14, 2012"
#define _MODULE_DESC_SHORT      "3D OpenGL-powered Compositing Window Manager module for EOS Scripting Environment"
#define _MODULE_DESC_LONG       ""
#define _MODULE_AUTHOR_NAME     "EOS Scripting Team"
#define _MODULE_AUTHOR_WEBSITE  "http://sourceforge.net/projects/eos-scripting/"
    //
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    // utility functions -----------------------------------------------------
    // end utility functions -------------------------------------------------


    // ====================== COMMON REQUIRED FUNCTIONS ============================
    // -----------------------------------------------------------------------------

    static int moduleCleanup(lua_State* luaRuntime)
        {
        EOSLuaRuntime::getInstance()->logInfoMessage("[eos."_MODULE_NAME"] Shutting down...");
        View::getInstance()->shutdown();
        //
        return 0;
        }


    // ====================== CUSTOM MODULE FUNCTIONS ==============================
    // -----------------------------------------------------------------------------

    /*- @function eos.view.getScreenCount
     *  @brief returns physical screens count
     *  @return count (numeric) number of physical screens
     */
    static int getScreenCount(lua_State* luaRuntime)
        {
        uint screenCount = 0;
        screenCount = View::getInstance()->getScreenCount();
        //
        lua_pushnumber(luaRuntime, screenCount);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.window.create
     *  @brief creates a view output window
     *  @param name (string) screen output screen identifier
     *  @param width (numeric) width of the window, in pixels
     *  @param height (numeric) height of the window, in pixels
     *  @param bpp (numeric) color depth (8-16-24-32)
     *  @param antialiasing (numeric) antialiasing level from 0 (off) to 4 (max)
     *  @param fullscreen (boolean) true for fullscreen, false for windowed mode
     *  @return windowId (numeric) id of the new window
     */
    static int windowCreate(lua_State* luaRuntime)
        {
        long windowId = 0;
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".window.create", 6))
            windowId = View::getInstance()->createWindow((unsigned int) lua_tonumber(luaRuntime, 1), (unsigned int) lua_tonumber(luaRuntime, 2), (unsigned int) lua_tonumber(luaRuntime, 3), (unsigned int) lua_tonumber(luaRuntime, 4), (unsigned int) lua_tonumber(luaRuntime, 5), lua_toboolean(luaRuntime, 6));
        //
        lua_pushnumber(luaRuntime, windowId);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.window.update
     *  @brief updates a view window
     *  @param windowId id of the window
     *  @return updated (boolean) true or false
     */
    static int windowUpdate(lua_State* luaRuntime)
        {
        bool done = false;
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".window.update", 1))
            done = View::getInstance()->updateWindow((ulong) lua_tonumber(luaRuntime, 1));
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.window.setCaption
     *  @brief sets the caption of a window 
     *  @param windowId (numeric) id of the window
     *  @param caption
     */
    static int windowSetCaption(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".window.setCaption", 2))
            {
            ViewWindow *wnd = View::getInstance()->getWindowFromId((ulong) lua_tonumber(luaRuntime, 1));
            wnd->setCaption(lua_tostring(luaRuntime, 2));
            }
        //
        return 0;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.window.setCurrent
     *  @brief sets the window as current target for operations
     *  @param windowId (numeric) id of the window
     */
    static int windowSetCurrent(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".window.setCurrent", 1))
            View::getInstance()->setCurrentWindow((ulong) lua_tonumber(luaRuntime, 1));
        //
        return 0;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.window.getFps
     *  @brief return last fps (frames per second) count made by a given window
     *  @param windowId (numeric) id of the window
     *  @return fps (numeric) last fps count
     */
    static int windowGetFps(lua_State* luaRuntime)
        {
        ViewWindow *wnd = NULL;
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".window.getFps", 1))
            wnd = View::getInstance()->getWindowFromId((ulong) lua_tonumber(luaRuntime, 1));
        if(wnd == NULL)
            lua_pushnumber(luaRuntime, -1);
        else
            lua_pushnumber(luaRuntime, wnd->getFps());
        return 1;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.surface.create
     *  @brief create a new view target surface
     *  @return surfaceId (numeric) id of the new surface
     */
    static int surfaceCreate(lua_State* luaRuntime)
        {
        int surfaceId = View::getInstance()->createSurface();
        //
        lua_pushnumber(luaRuntime, surfaceId);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.surface.createFromObject
     *  @brief create a surface from a shared object
     *  @param sharedObject (eosObject) a shared object
     *  @return surfaceId (numeric) id of the new surface
     */
    static int surfaceCreateFromObject(lua_State* luaRuntime)
        {
        int surfaceId = -1;
        bool done = false;
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".surface.createFromObject", 1))
            {
            surfaceId = View::getInstance()->createSurface();
            ViewSurface* gs = View::getInstance()->getSurfaceFromId(surfaceId);
            EOSObject* sessionObject = (EOSObject*) lua_touserdata(luaRuntime, 1);
            if(sessionObject != NULL)
                {
                eos_imagedata *data = (eos_imagedata *) sessionObject->getUserData();
                done = gs->connectSource(data);
                }
            }
        //
        lua_pushnumber(luaRuntime, surfaceId);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.surface.createSubSurface
     *  @brief creates a new target view subsurface
     *  @return subSurfaceId (numeric) id of the new subsurface
     */
    static int surfaceCreateSubSurface(lua_State* luaRuntime)
        {
        int surfaceId = View::getInstance()->createSubSurface();
        //
        lua_pushnumber(luaRuntime, surfaceId);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.surface.createSubSurfaceFromObject
     *  @brief create a subsurface from a shared object
     *  @param sharedObject (eosObject) a shared object
     *  @return subSurfaceId (numeric) id of the new subsurface
     */
    static int surfaceCreateSubSurfaceFromObject(lua_State* luaRuntime)
        {
        int surfaceId = -1;
        bool done = false;
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".surface.createSubSurfaceFromObject", 1))
            {
            surfaceId = View::getInstance()->createSubSurface();
            ViewSurface* gs = View::getInstance()->getSurfaceFromId(surfaceId);
            EOSObject* sessionObject = (EOSObject*) lua_touserdata(luaRuntime, 1);
            if(sessionObject != NULL)
                {
                eos_imagedata *data = (eos_imagedata *) sessionObject->getUserData();
                done = gs->connectSource(data);
                }
            }
        //
        lua_pushnumber(luaRuntime, surfaceId);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.surface.addSubSurface
     *  @brief adds a subsurface to a surface
     *  @param surfaceId (numeric) surface's id
     *  @param subSurfaceId (numeric) subsurface's id
     *  @return success (boolean) true or false
     */
    static int surfaceAddSubSurface(lua_State* luaRuntime)
        {
        bool done = false;
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".surface.addSubSurface", 2))
            {
            ViewSurface *gs1 = View::getInstance()->getSurfaceFromId(lua_tonumber(luaRuntime, 1));
            ViewSurface *gs2 = View::getInstance()->getSurfaceFromId(lua_tonumber(luaRuntime, 2));
            done = gs1->addSubSurface(gs2);
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.surface.removeSubSurface
     *  @brief removes a subsurface from a surface
     *  @param surfaceId (numeric) surface's id
     *  @param subSurfaceId (numeric) subsurface's id
     *  @return success (boolean) true or false
     */
    static int surfaceRemoveSubSurface(lua_State* luaRuntime)
        {
        bool done = false;
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".surface.removeSubSurface", 2))
            {
            ViewSurface *gs1 = View::getInstance()->getSurfaceFromId(lua_tonumber(luaRuntime, 1));
            ViewSurface *gs2 = View::getInstance()->getSurfaceFromId(lua_tonumber(luaRuntime, 2));
            done = gs1->removeSubSurface(gs2);
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.surface.save
     *  @brief save a surface on a file
     *  @param surfaceId (numeric) surface's id
     *  @param filename
     *  @return success (boolean) true or false
     */
    static int surfaceSave(lua_State* luaRuntime)
        {
        bool done = false;
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".surface.save", 2))
            {
            ViewSurface *gs = View::getInstance()->getSurfaceFromId(lua_tonumber(luaRuntime, 1));
            done = gs->save(lua_tostring(luaRuntime, 2));
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.surface.setPosition
     *  @brief sets surface's position
     *  @param surfaceId (numeric) surface's id
     *  @param x (numeric) x value
     *  @param y (numeric) y value
     *  @param z (numeric) z value
     */
    static int surfaceSetPosition(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".surface.setPosition", 4))
            {
            ViewSurface *gs = View::getInstance()->getSurfaceFromId(lua_tonumber(luaRuntime, 1));
            gs->setPosition(lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4));
            }
        //
        return 0;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.surface.setOrientation
     *  @brief sets surface's orientation
     *  @param surfaceId (numeric) surface's id
     *  @param x (numeric) quaternion x value
     *  @param y (numeric) quaternion y value
     *  @param z (numeric) quaternion z value
     *  @param w (numeric) quaternion w value
     */
    static int surfaceSetOrientation(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".surface.setOrientation", 5))
            {
            ViewSurface *gs = View::getInstance()->getSurfaceFromId(lua_tonumber(luaRuntime, 1));
            gs->setOrientation(lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4), lua_tonumber(luaRuntime, 5));
            }
        //
        return 0;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.surface.setScale
     *  @brief sets surface scale factor
     *  @param surfaceId (numeric) surface's id
     *  @param x (numeric) scale factor for X axis
     *  @param y (numeric) scale factor for Y axis
     *  @param z (numeric) scale factor for Z axis
     */
    static int surfaceSetScale(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".surface.setScale", 4))
            {
            ViewSurface *gs = View::getInstance()->getSurfaceFromId(lua_tonumber(luaRuntime, 1));
            gs->setScale(lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4));
            }
        //
        return 0;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.surface.setColor
     *  @brief sets the surface's color
     *  @param surfaceId (numeric) surface's id
     *  @param r (numeric) red component value (0.0 to 1.0)
     *  @param g (numeric) green component value (0.0 to 1.0)
     *  @param b (numeric) blue component value (0.0 to 1.0)
     *  @param a (numeric) alpha component value (0.0 to 1.0)
     */
    static int surfaceSetColor(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".surface.setColor", 5))
            {
            ViewSurface *gs = View::getInstance()->getSurfaceFromId(lua_tonumber(luaRuntime, 1));
            gs->setColor(lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4), lua_tonumber(luaRuntime, 5));
            }
        //
        return 0;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.surface.rotateByAxisAngle
     *  @brief rotates the surface by a given axis and an angle
     *  @param surfaceId (numeric) surface's id
     *  @param x (numeric) vector X component
     *  @param y (numeric) vector Y component
     *  @param z (numeric) vector Z component
     *  @param a (numeric) rotation angle
     */
    static int surfaceRotateByAxisAngle(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".surface.rotateByAxisAngle", 5))
            {
            ViewSurface *gs = View::getInstance()->getSurfaceFromId(lua_tonumber(luaRuntime, 1));
            gs->rotateByAxisAngle(lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4), lua_tonumber(luaRuntime, 5));
            }
        //
        return 0;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.view.surface.rotateByAngles
     *  @brief rotates the surface by three angles
     *  @param surfaceId (numeric) surface's id
     *  @param x (numeric) rotation angle on X axis
     *  @param y (numeric) rotation angle on Y axis
     *  @param z (numeric) rotation angle on Z axis
     */
    static int surfaceRotateByAngles(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".surface.rotateByAngles", 5))
            {
            ViewSurface *gs = View::getInstance()->getSurfaceFromId(lua_tonumber(luaRuntime, 1));
            gs->rotateByAxisAngle(lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4), lua_tonumber(luaRuntime, 5));
            }
        //
        return 0;
        }

    // ========================== END LIBRARY FUNCTIONS ============================


    // ======================== LUA FUNCTIONS REGISTRATION =========================

    // ==== Here we declare an array with all functions and their respective names.
    // ==== This array has elements of type luaL_reg, which is a structure with
    // ==== two fields: a string and a function pointer.

    const struct luaL_reg fn_table_module [] = {
        {"cleanup", moduleCleanup},        
        {NULL, NULL} // <-- end-of-table sentinel
        };
    
    const struct luaL_reg fn_table_main [] = {
        //{"update", moduleUpdate},
        {"getScreenCount", getScreenCount},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_window [] = {
        {"create", windowCreate},
        {"update", windowUpdate},
        {"setCaption", windowSetCaption},
        {"setCurrent", windowSetCurrent},
        {"getFps", windowGetFps},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_surface [] = {
        {"create", surfaceCreate},
        {"createFromObject", surfaceCreateFromObject},
        {"createSubSurface", surfaceCreateSubSurface},
        {"createSubSurfaceFromObject", surfaceCreateSubSurfaceFromObject},
        {"addSubSurface", surfaceAddSubSurface},
        {"removeSubSurface", surfaceRemoveSubSurface},
        {"save", surfaceSave},
        {"setPosition", surfaceSetPosition},
        {"setOrientation", surfaceSetOrientation},
        {"setScale", surfaceSetScale},
        {"setColor", surfaceSetColor},
        {"rotateByAxisAngle", surfaceRotateByAxisAngle},
        {"rotateByAngles", surfaceRotateByAngles},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    // ==== Finally, we declare a main (initialization) function, using luaL_openlib:

    int _INIT_(lua_State* luaRuntime)
        {
        EOSLuaRuntime::getInstance()->setRuntime(luaRuntime);
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

        // registers main functions
        luaL_register(luaRuntime, "eos."_MODULE_NAME, fn_table_main);
        
        // registers window functions
        luaL_register(luaRuntime, "eos."_MODULE_NAME".window", fn_table_window);

        // registers surface functions
        luaL_register(luaRuntime, "eos."_MODULE_NAME".surface", fn_table_surface);
        //
        //
        return 1;
        }

    // -----------------------------------------------------------------------------

    } // extern "C" closure
