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


#include "EOSLuaScript.h"

// -----------------------------------------------------------------------------

EOSLuaScript::EOSLuaScript(EOSLuaEnvironment* luaEnvPtr, const char* scriptFileName)
	{
	this->luaEnv = luaEnvPtr;
	this->luaThisRef = 0;
	this->currSelectedFuncName = NULL;
	this->scriptName = (scriptFileName != NULL) ? scriptFileName : "generic_lua_script"; // <-- generic name if not specified
	this->numArgs = 0;
	//
	lua_State* luaState = luaEnv->getLuaState();
	if(luaEnv->isOk())
		{
		// Creates a reference to the "this" table. Each reference is unique:
		// lua_newtable function creates an empty table and pushes it on the stack
		lua_newtable(luaState);
		// luaL_ref pops the empty table from the stack, stores it into the registry
		// with a fresh integer key, and returns that key
		this->luaThisRef = luaL_ref(luaState, LUA_REGISTRYINDEX);
		// Saves the "this" table to index 0 of the "this" table:
		// lua_rawgeti pushes the value associated with the reference luaThisRef onto the stack
		lua_rawgeti(luaState, LUA_REGISTRYINDEX, luaThisRef);
		// pushes *this pointer to the stack
		lua_pushlightuserdata(luaState, (void*)this);
		// sets *this pointer at the index 0 of the table present at position -2 of the stack
		lua_rawseti (luaState, -2, 0);
		}
	}

// -----------------------------------------------------------------------------

EOSLuaScript::~EOSLuaScript()
	{
	EOSLuaStackRestorer lsr(luaEnv);
	lua_State* luaState = this->luaEnv->getLuaState();
	if(this->luaEnv->isOk())
		luaL_unref(luaState, LUA_REGISTRYINDEX, this->luaThisRef);
    // forces lua garbage collection to free unused memory
	lua_gc(luaState, LUA_GCCOLLECT, 0);
	}

// -----------------------------------------------------------------------------

bool EOSLuaScript::compileBuffer(unsigned char* bufferPtr, size_t bufferSize)
	{
	bool done = false;
	if(this->luaEnv->isOk() && bufferPtr != NULL && bufferSize > 0)
		{
		// Make sure we have the correct "this" table
		EOSLuaThisRestorer ltr(this->luaEnv, this->luaThisRef);
		done = this->luaEnv->runBuffer(bufferPtr, bufferSize, this->scriptName.c_str());
		}
	return done;
	}

// -----------------------------------------------------------------------------

bool EOSLuaScript::selectScriptFunction(const char* functionName)
	{
  bool success = true;
	lua_State* luaState = this->luaEnv->getLuaState();
	if(this->luaEnv->isOk())
		{
		// Look up function name
		lua_rawgeti(luaState, LUA_REGISTRYINDEX, this->luaThisRef);
		lua_pushstring(luaState, functionName);
		lua_rawget(luaState, -2);
		lua_remove(luaState, -2);

		// Put the "this" table back
		lua_rawgeti(luaState, LUA_REGISTRYINDEX, this->luaThisRef);

		// Check that we have a valid function
		if(!lua_isfunction(luaState, -2))
			{
			success = false;
			lua_pop(luaState, 2);
			}
		else
			{
			this->numArgs = 0;
			this->currSelectedFuncName = functionName;
			}
		}

	return success;
	}

// -----------------------------------------------------------------------------

void EOSLuaScript::addParam(int intValue)
	{
	lua_State* luaState = this->luaEnv->getLuaState();
	if(this->luaEnv->isOk())
		{
		lua_pushnumber(luaState, (lua_Number)intValue);
		numArgs++;
		}
	}

// -----------------------------------------------------------------------------

void EOSLuaScript::addParam(float floatValue)
	{
	lua_State* luaState = this->luaEnv->getLuaState();
	if(this->luaEnv->isOk())
		{
		lua_pushnumber(luaState, (lua_Number)floatValue);
		numArgs++;
		}
	}

// -----------------------------------------------------------------------------

void EOSLuaScript::addParam(char* string)
	{
	lua_State* luaState = this->luaEnv->getLuaState();
	if(this->luaEnv->isOk() && string != NULL)
		{
		lua_pushstring(luaState, string);
		numArgs++;
		}
	}

// -----------------------------------------------------------------------------

bool EOSLuaScript::go(int numReturns)
	{
	// At this point there should be parameters and a function on the
	// Lua stack. Each function get a "this" parameter as default and is
	// pushed onto the stack when the method is selected
	EOSLuaThisRestorer ltr(this->luaEnv, this->luaThisRef);
	bool success = this->luaEnv->callFunction(numArgs + 1, numReturns);
	return success;
	}

// -----------------------------------------------------------------------------

bool EOSLuaScript::scriptHasFunction(const char* functionName)
	{
	EOSLuaStackRestorer lsr(luaEnv);
	//
	bool functionFound = false;
	lua_State* luaState = this->luaEnv->getLuaState();
	if(this->luaEnv->isOk() && functionName != NULL)
		{
		lua_rawgeti(luaState, LUA_REGISTRYINDEX, this->luaThisRef);
		lua_pushstring(luaState, functionName);
		lua_rawget(luaState, -2);
		lua_remove(luaState, -2);
		//
		functionFound = lua_isfunction(luaState, -1);
		}
	return functionFound;
	}

// -----------------------------------------------------------------------------

