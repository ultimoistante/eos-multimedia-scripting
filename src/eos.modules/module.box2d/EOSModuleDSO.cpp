/*
-----------------------------------------------------------------------------
This source file is part of EOS Multimedia Scripting System
For the latest info, see https://github.com/ultimoistante/eos-multimedia-scripting

The MIT License (MIT)

Copyright (c) 2014-2019 of Salvatore Carotenuto aka ultimoistante of StartupSolutions
                          (mailto: ultimoistante@gmail.com | www.startupsolutions.it)
                       and Alessandro Rasulo (mailto: alex9978@gmail.com)

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

#include "Box2d.h"
#include "EOSLuaRuntime.h"
#include "eos_data_containers.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

// -----------------------------------------------------------------------------

// utility functions -----------------------------------------------------
// end utility functions -------------------------------------------------


// ====================== COMMON REQUIRED FUNCTIONS ============================
// -----------------------------------------------------------------------------

static int moduleCleanup(lua_State* luaRuntime)
	{
	/*
	 * insert here code for resources cleanup
	 * on module unload (automatically done by kernel on close)
	 */
    EOSLuaRuntime::getInstance()->logInfoMessage("[eos.box2d] Shutting down...");
	//
	return 0;
	}

// ====================== CUSTOM MODULE FUNCTIONS ==============================

/*- @function eos.picasso.update
 *  @brief updates picasso module and active render targets
 *  @return updated (boolean) true or false
 */
static int moduleUpdate(lua_State* luaRuntime)
	{
	bool done = true;
	if(EOSLuaRuntime::checkLuaParameters("eos.box2d", "eos.box2d.update", 1))
		Box2d::getInstance()->update(lua_tonumber(luaRuntime, 1));
	//
	lua_pushboolean(luaRuntime, done);
	return 1;
	}

// -----------------------------------------------------------------------------

/*-
 * @name function eos.box2d.body.create()
 * @brief create a 2d rigid body
 * @param body name
 * @param friction is used to make objects slide along each other realistically (0..1)
 * @param restitution is used to make objects bounce (0..1)
 * @param density needed to calculate body mass
 * @returns session object
 */
static int bodyCreate(lua_State* luaRuntime)
	{
    void* userdata = NULL;
	if(EOSLuaRuntime::checkLuaParameters("eos.box2d", "eos.box2d.body.create", 4))
		userdata = Box2d::getInstance()->createBody(lua_tostring(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4));
	//
    lua_pushlightuserdata(luaRuntime, userdata);
	return 1;
	}

// -----------------------------------------------------------------------------

/*-
 * @name function eos.box2d.body.addCircleShape()
 * @brief adds a circular shape to a body
 * @param body name
 * @param x
 * @param y
 * @param radius
 * @returns true or false
 */
static int bodyAddCircleShape(lua_State* luaRuntime)
	{
	if(EOSLuaRuntime::checkLuaParameters("eos.box2d", "eos.box2d.body.addCircleShape", 4))
        {
        Box2dBody *body = Box2d::getInstance()->getBody(lua_tostring(luaRuntime, 1));
        if(body != NULL)
            body->addCircleShape(lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4));
        }
	//
	return 0;
	}

// -----------------------------------------------------------------------------

/*-
 * @name function eos.box2d.body.addBoxShape()
 * @brief adds a box shape to a body
 * @param body name
 * @param x
 * @param y
 * @param width
 * @param height
 * @returns true or false
 */
static int bodyAddBoxShape(lua_State* luaRuntime)
	{
	if(EOSLuaRuntime::checkLuaParameters("eos.box2d", "eos.box2d.body.addBoxShape", 5))
        {
        Box2dBody *body = Box2d::getInstance()->getBody(lua_tostring(luaRuntime, 1));
        if(body != NULL)
            body->addBoxShape(lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4), lua_tonumber(luaRuntime, 5));
        }
	//
	return 0;
	}

// -----------------------------------------------------------------------------

/*-
 * @name function eos.box2d.body.setPosition()
 * @brief sets body position
 * @param body name
 * @param x
 * @param y
  */
static int bodySetPosition(lua_State* luaRuntime)
	{
 	if(EOSLuaRuntime::checkLuaParameters("eos.box2d", "eos.box2d.body.bodySetPosition", 3))
        {
        Box2dBody *body = Box2d::getInstance()->getBody(lua_tostring(luaRuntime, 1));
        if(body != NULL)
            body->setPosition(lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3));
        }
	//
 	return 0;
	}

// -----------------------------------------------------------------------------

/*-
 * @name function eos.box2d.body.applyForce()
 * @brief apply a force to a body
 * @param body name
 * @param force vector x
 * @param force vector y
 * @param world application point x
 * @param world application point y
 */
static int bodyApplyForce(lua_State* luaRuntime)
	{
 	if(EOSLuaRuntime::checkLuaParameters("eos.box2d", "eos.box2d.body.bodyApplyForce", 5))
        {
        Box2dBody *body = Box2d::getInstance()->getBody(lua_tostring(luaRuntime, 1));
        if(body != NULL)
            body->applyForce(lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4), lua_tonumber(luaRuntime, 5));
        }
	//
 	return 0;
	}

// -----------------------------------------------------------------------------

/*-
 * @name function eos.box2d.body.applyImpulse()
 * @brief apply an impulse to a body
 * @param body name
 * @param impulse vector x
 * @param impulse vector y
 * @param world application point x
 * @param world application point y
 */
static int bodyApplyImpulse(lua_State* luaRuntime)
	{
 	if(EOSLuaRuntime::checkLuaParameters("eos.box2d", "eos.box2d.body.bodyApplyImpulse", 5))
        {
        Box2dBody *body = Box2d::getInstance()->getBody(lua_tostring(luaRuntime, 1));
        if(body != NULL)
            body->applyImpulse(lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4), lua_tonumber(luaRuntime, 5));
        }
	//
 	return 0;
	}

// -----------------------------------------------------------------------------
// ========================== END LIBRARY FUNCTIONS ============================


// ======================== LUA FUNCTIONS REGISTRATION =========================

// ==== Here we declare an array with all functions and their respective names.
// ==== This array has elements of type luaL_reg, which is a structure with
// ==== two fields: a string and a function pointer.

const struct luaL_reg fn_table_module [] = {
	{"cleanup",         moduleCleanup},
    {"update",          moduleUpdate},
	{NULL, NULL}        // <-- end-of-table sentinel
	};

const struct luaL_reg fn_table_body [] = {
	{"create",          bodyCreate},
    {"addCircleShape",  bodyAddCircleShape},
    {"addBoxShape",     bodyAddBoxShape},
    {"setPosition",     bodySetPosition},
    {"applyForce",      bodyApplyForce},
    {"applyImpulse",    bodyApplyImpulse},
	{NULL, NULL}        // <-- end-of-table sentinel
	};


// ==== Finally, we declare a main (initialization) function, using luaL_openlib:

int _INIT_(lua_State* luaRuntime)
	{
	EOSLuaRuntime::getInstance()->setRuntime(luaRuntime);
	//
	// Module manifest structure. Developer must edit just the #define values below.
	//
	#define _MODULE_NAME            "box2d"
	#define _MODULE_VERSION         "0.1"
	#define _MODULE_DATE		    "October 10, 2009"
	#define _MODULE_DESC_SHORT      "2d physics module for EOS Scripting Environment"
	#define _MODULE_DESC_LONG       ""
	#define _MODULE_AUTHOR_NAME     "Play-ON.it and EOS Scripting Team"
	#define _MODULE_AUTHOR_WEBSITE  "http://sourceforge.net/projects/eos-scripting/ | http://ultimoistante.wordpress.com/"
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

  	// registers "eos.box2d.body" functions
	luaL_register(luaRuntime, "eos."_MODULE_NAME".body", fn_table_body);

	//
	//
	return 1;
	}

// -----------------------------------------------------------------------------

} // extern "C" closure
