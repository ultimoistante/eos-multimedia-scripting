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

#include "BulletPhysics.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "EOSLuaRuntime.h"


/*- @module eos.bullet
 *  @brief Bullet Collision Detection and Physics for EOS Scripting Environment
 */


// -----------------------------------------------------------------------------

BulletPhysics bulletPhysics;

// utility functions -----------------------------------------------------
// end utility functions -------------------------------------------------


// ====================== COMMON REQUIRED FUNCTIONS ============================
// -----------------------------------------------------------------------------

/*- @function eos.bullet.module.cleanup
 *  @brief automatically called from eos kernel on module unloading
 */
static int moduleCleanup(lua_State* luaRuntime)
	{
	bulletPhysics.cleanup();
	//
	return 0;
	}

// ====================== CUSTOM MODULE FUNCTIONS ==============================
// -----------------------------------------------------------------------------

/*- @function eos.bullet.body.createBox
 *  @brief creates a new physic object
 *  @param imageFileName filename of the image to load
 *  @return loadedImage an EOSbject cointaining the pointer to the decoded image pixel buffer
 */
static int update(lua_State* luaRuntime)
	{
	if(EOSLuaRuntime::checkLuaParameters("eos.bullet", "update", 1))
		bulletPhysics.update((float)lua_tonumber(luaRuntime, 1));
	//
	return 0;
	}

// -----------------------------------------------------------------------------

/*- @function eos.bullet.body.createBox
 *  @brief creates a new box-shaped physic object
 *  @param bodyName unique name of the body to create
 *  @param xSize object size on the x axis
 *  @param ySize object size on the y axis
 *  @param zSize object size on the z axis
 *  @param mass object mass
 *  @return success true if body created, false otherwise
 */
static int createBox(lua_State* luaRuntime)
	{
	bool done = false;
	//
	if(EOSLuaRuntime::checkLuaParameters("eos.bullet", "body.createBox", 5))
		done = bulletPhysics.createBody(luaL_checkstring(luaRuntime, 1), BulletPhysics::SHAPE_BOX, (btScalar)lua_tonumber(luaRuntime, 2), (btScalar)lua_tonumber(luaRuntime, 3), (btScalar)lua_tonumber(luaRuntime, 4), (btScalar)lua_tonumber(luaRuntime, 5));
	//
	lua_pushboolean(luaRuntime, done);
	return 1;
	}

// -----------------------------------------------------------------------------

/*- @function eos.bullet.body.createSphere
 *  @brief creates a new spheric-shaped physic object
 *  @param bodyName unique name of the body to create
 *  @param radius sphere radius
 *  @param mass object mass
 *  @return success true if body created, false otherwise
 */
static int createSphere(lua_State* luaRuntime)
	{
	bool done = false;
	//
	if(EOSLuaRuntime::checkLuaParameters("eos.bullet", "body.createSphere", 3))
		done = bulletPhysics.createBody(luaL_checkstring(luaRuntime, 1), BulletPhysics::SHAPE_SPHERE, (btScalar)lua_tonumber(luaRuntime, 2), 0, 0, (btScalar)lua_tonumber(luaRuntime, 3));
	//
	lua_pushboolean(luaRuntime, done);
	return 1;
	}

// -----------------------------------------------------------------------------

/*- @function eos.bullet.body.exportToObject
 *  @brief exports object data in an EOSObject
 *  @param bodyName unique name of the body to export
 *  @return sessionObject an EOSbject cointaining the pointer to the physic body data
 */
static int bodyExportToObject(lua_State* luaRuntime)
	{
	EOSObject* sessionObject = NULL;
	//
	if(EOSLuaRuntime::checkLuaParameters("eos.bullet", "body.exportToObject", 1))
		{
		RigidBody* body = bulletPhysics.bodyByName(luaL_checkstring(luaRuntime, 1));
		if(body)
			sessionObject = body->getSessionObject();
		}
	//
	lua_pushlightuserdata(luaRuntime, sessionObject);
	return 1;
	}

// -----------------------------------------------------------------------------

/*- @function eos.bullet.body.setMass
 *  @brief sets the mass of a physic object
 *  @param bodyName (string) name of the requested body
 *  @param bodyMass (numeric) mass value to give to the object
 *  @return success (boolean) true if success, false otherwise
 */
static int bodySetMass(lua_State* luaRuntime)
	{
	bool done = false;
	//
	if(EOSLuaRuntime::checkLuaParameters("eos.bullet", "body.setMass", 2))
		{
		RigidBody* body = bulletPhysics.bodyByName(luaL_checkstring(luaRuntime, 1));
		if(body)
			{
			body->setMass((btScalar)lua_tonumber(luaRuntime, 2));
			done = true;
			}
		}
	//
	lua_pushboolean(luaRuntime, done);
	return 1;
	}

// -----------------------------------------------------------------------------

/*- @function eos.bullet.body.getPosition
 *  @brief returns current position of a body
 *  @param bodyName (string) name of the requested body
 *  @return
 */
static int bodyGetPosition(lua_State* luaRuntime)
	{
	if(EOSLuaRuntime::checkLuaParameters("eos.bullet", "body.getPosition", 1))
		{
		RigidBody* body = bulletPhysics.bodyByName(luaL_checkstring(luaRuntime, 1));
		if(body)
			{
			btVector3* pos = body->getPosition();
			if(pos != NULL)
				{
				lua_pushnumber(luaRuntime, (float)pos->getX());
				lua_pushnumber(luaRuntime, (float)pos->getY());
				lua_pushnumber(luaRuntime, (float)pos->getZ());
				return 3;
				}
			}
		}
	//
	return 0;
	}

// -----------------------------------------------------------------------------

/*- @function eos.bullet.body.setPosition
 *  @brief sets position of a body
 *  @param bodyName (string) name of the requested body
 *  @param x position on the x axis
 *  @param y position on the y axis
 *  @param z position on the z axis
 *  @return
 */
static int bodySetPosition(lua_State* luaRuntime)
	{
	if(EOSLuaRuntime::checkLuaParameters("eos.bullet", "body.setPosition", 4))
		{
		RigidBody* body = bulletPhysics.bodyByName(luaL_checkstring(luaRuntime, 1));
		if(body)
			body->setPosition((btScalar)lua_tonumber(luaRuntime, 2), (btScalar)lua_tonumber(luaRuntime, 3), (btScalar)lua_tonumber(luaRuntime, 4));
		}
	//
	return 0;
	}

// ======================== LUA FUNCTIONS REGISTRATION =========================
// -----------------------------------------------------------------------------

// ==== Here we declare an array with all functions and their respective names.
// ==== This array has elements of type luaL_reg, which is a structure with
// ==== two fields: a string and a function pointer.

const struct luaL_reg fn_table_module [] = {
	{"cleanup",         moduleCleanup},
	{NULL, NULL}  // <-- end-of-table sentinel
	};

const struct luaL_reg fn_table_main [] = {
	{"update",          update},
	{NULL, NULL}  // <-- end-of-table sentinel
	};

const struct luaL_reg fn_table_body [] = {
	{"createBox",       createBox},
	{"createSphere",    createSphere},
	{"exportToObject",  bodyExportToObject},
	{"setMass",         bodySetMass},
	{"getPosition",     bodyGetPosition},
	{"setPosition",     bodySetPosition},
	{NULL, NULL}  // <-- end-of-table sentinel
	};


// ==== Finally, we declare a main (initialization) function, using luaL_openlib:

int _INIT_(lua_State* luaRuntime)
	{
	EOSLuaRuntime::getInstance()->setRuntime(luaRuntime);
	//
	// Module manifest structure. Developer must edit just the #define values below.
	//
	#define _MODULE_NAME            "bullet"
	#define _MODULE_VERSION         "0.1"
	#define _MODULE_DATE            "September 26, 2009"
	#define _MODULE_DESC_SHORT      "Bullet Collision Detection and Physics for EOS Scripting Environment"
	#define _MODULE_DESC_LONG       "This module uses Bullet Collision Detection and Physics Library (www.bulletphysics.com)"
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

   // registers main functions
	luaL_register(luaRuntime, "eos."_MODULE_NAME, fn_table_main);

	// registers "eos.*.body" functions
	EOSLuaRuntime::getInstance()->executeLuaChunk("eos."_MODULE_NAME".body = {}");
	luaL_register(luaRuntime, "eos."_MODULE_NAME".body", fn_table_body);

	//
	return 1;
	}

// -----------------------------------------------------------------------------

} // extern "C" closure

