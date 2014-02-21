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
#include <stdlib.h>

#include "EOSLuaRuntime.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"



// -----------------------------------------------------------------------------

// utility functions -----------------------------------------------------------

int justPrintSomething()
   {
   printf("Greetings from eos.example module!\n");
   }

// end utility functions -------------------------------------------------------

// ====================== COMMON REQUIRED FUNCTIONS ============================
// -----------------------------------------------------------------------------

static int moduleCleanup(lua_State* luaRuntime)
	{
	/*
	 * insert here code for resources cleanup
	 * on module unload (automatically done by kernel on close)
	 */
	//
	return 0;
	}

// ====================== CUSTOM MODULE FUNCTIONS ==============================
// -----------------------------------------------------------------------------

// --- First, we define our library functions:

static int printSomething(lua_State* luaRuntime)
	{
   justPrintSomething();
   //
	return 0;
	}

// -----------------------------------------------------------------------------

static int writeToLog(lua_State* luaRuntime)
	{
   bool done = false;
	if(EOSLuaRuntime::checkLuaParameters("eos.example", "writeToLog", 1))
      EOSLuaRuntime::getInstance()->logUserMessage(luaL_checkstring(luaRuntime, 1));
	//
	return 0;
	}


// ======================== LUA FUNCTIONS REGISTRATION =========================

// ==== Here we declare an array with all functions and their respective names.
// ==== This array has elements of type luaL_reg, which is a structure with
// ==== two fields: a string and a function pointer.

static const struct luaL_reg module_table [] = {
	{"printSomething",         printSomething},
	{"writeToLog",             writeToLog},
	{NULL, NULL}  // <-- end-of-table sentinel
	};

// ==== Finally, we declare a main (initialization) function, using luaL_openlib:

int _INIT_(lua_State* luaRuntime)
	{
	#define _MODULE_VERSION  "0.2"
	#define _MODULE_DATE     "August 13, 2009"

	EOSLuaRuntime::getInstance()->setRuntime(luaRuntime);
	//
	// let's fill the module manifest structure. Pay attention: it's in lua language!
	EOSLuaRuntime::getInstance()->executeLuaChunk(
            //
				"eos.example = {}\
				_NEW_MODULE_LOADED_ = eos.example\
				_NEW_MODULE_LOADED_._moduleinfo                   = {}\
				_NEW_MODULE_LOADED_._moduleinfo.name              = 'example.eosmodule'\
				_NEW_MODULE_LOADED_._moduleinfo.tableName         = 'eos.example'\
				_NEW_MODULE_LOADED_._moduleinfo.version           = '"_MODULE_VERSION"'\
				_NEW_MODULE_LOADED_._moduleinfo.date              = '"_MODULE_DATE"'\
				_NEW_MODULE_LOADED_._moduleinfo.description       = {}\
				_NEW_MODULE_LOADED_._moduleinfo.description.short = 'Example Module for EOS environment'\
				_NEW_MODULE_LOADED_._moduleinfo.description.long  = ''\
				_NEW_MODULE_LOADED_._moduleinfo.author            = {}\
				_NEW_MODULE_LOADED_._moduleinfo.author.name       = 'Play-ON.it and EOS Scripting Team'\
				_NEW_MODULE_LOADED_._moduleinfo.author.website    = 'http://sourceforge.net/projects/eos-scripting/ | http://ultimoistante.wordpress.com/'\
				_NEW_MODULE_LOADED_._moduleinfo.reservedFunctions         = {}\
				_NEW_MODULE_LOADED_._moduleinfo.reservedFunctions.init    = ''\
				_NEW_MODULE_LOADED_._moduleinfo.reservedFunctions.cleanup = 'eos.example.module.cleanup'\
				");

	// registers "eos.oisinput" functions
	luaL_register(luaRuntime, "eos.example", module_table);
	//
	return 1;
	}

// -----------------------------------------------------------------------------

} // extern "C" closure

