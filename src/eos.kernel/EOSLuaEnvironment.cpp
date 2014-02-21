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


#include "EOSLuaEnvironment.h"

// -----------------------------------------------------------------------------

EOSLuaEnvironment::EOSLuaEnvironment()
	{
	this->luaState = NULL;
	this->allOk = false;
	}

// -----------------------------------------------------------------------------

EOSLuaEnvironment::~EOSLuaEnvironment()
	{
	if(luaState != NULL)
		lua_close(luaState);
	}

// -----------------------------------------------------------------------------

bool EOSLuaEnvironment::init()
	{
	if(allOk) destroy();

	// let's open Lua!
	luaState = lua_open();

	if(luaState != NULL)
		{
		allOk = true;
		//
		// Load all standard lua libraries (modified for 5.1)
		luaL_openlibs(luaState);
		//
		// probably introduced in newer versions... :S
		lua_atpanic(luaState, (lua_CFunction) EOSLuaEnvironment::vmPanic);
		//
		return true;
		}
	return false;
	}

// -----------------------------------------------------------------------------

void EOSLuaEnvironment::destroy()
	{
	if(this->luaState != NULL)
		{
		lua_close(luaState);
		this->luaState = NULL;
		this->allOk = false;
		}
	}

// -----------------------------------------------------------------------------

bool EOSLuaEnvironment::runFile(const char* filename)
	{
	bool success = false;
	int  err = 0;

	if((err = luaL_loadfile(luaState, filename)) == 0)
		{
		// Call main...
		success = ((err = lua_pcall(luaState, 0, LUA_MULTRET, 0)) == 0);
		}

	return success;
	}

// -----------------------------------------------------------------------------

bool EOSLuaEnvironment::runBuffer(const unsigned char* bufferPtr, size_t bufferSize, const char* bufferName)
	{
	bool success = false;
	int  err = 0;

	if(bufferName == NULL)
		bufferName = "Temp";

	if((err = luaL_loadbuffer(luaState, (const char*)bufferPtr, bufferSize, bufferName)) == 0)
		{
		// Call main...
		success = ((err = lua_pcall (luaState, 0, LUA_MULTRET, 0)) == 0);
		}
	else
		handleError(luaState, err);

	return success;
	}

// -----------------------------------------------------------------------------

bool EOSLuaEnvironment::callFunction(int nArgs, int nReturns /* = 0 */)
	{
	bool success = false;

	if(lua_isfunction(luaState, -nArgs-1))
		{
		int err = lua_pcall(luaState, nArgs, nReturns, 0);

		if(err == 0)
			success = true;
		else
			handleError(luaState, err);
		}

	return success;
	}

// -----------------------------------------------------------------------------

char* EOSLuaEnvironment::getLuaValueAsString(const char* luaVarName)
	{
	char* valueString = NULL;
	if(luaVarName != NULL)
		{
		char luaCommand[256];
		snprintf(luaCommand, 255, "__tmpLuaValue = %s", luaVarName);
		if(runBuffer((const unsigned char*)luaCommand, strlen(luaCommand)))
			{
			lua_getglobal(luaState, "__tmpLuaValue");
			if(!lua_isnil(luaState, -1))
				{
				strncpy(this->tempValueString, (char*)lua_tostring(luaState, -1), 1024);
				valueString = this->tempValueString;
				}
			lua_pop(luaState, 1);
			}
		}
	return valueString;
	}

// -----------------------------------------------------------------------------

void EOSLuaEnvironment::vmPanic(lua_State* lua)
	{
	printf("\n[!!!] WARNING!!! LUA VM PANIC [!!!]\n");
	}

// -----------------------------------------------------------------------------

void EOSLuaEnvironment::handleError(lua_State* lua, int errorCode)
	{
	fprintf(stderr, "\n[EOSLuaEnvironment ---------------------------------------------------------]\n");
	switch(errorCode)
		{
		case LUA_ERRRUN:
			fprintf(stderr, "| Runtime");
			break;
		case LUA_ERRSYNTAX:
			fprintf(stderr, "| Syntax ");
			break;
		case LUA_ERRFILE:
			fprintf(stderr, "| File ");
			break;
		case LUA_ERRMEM:
			fprintf(stderr, "| Fatal ");
			break;
		case LUA_ERRERR:
			fprintf(stderr, "| Fatal ");
			break;
		}

	// Get the error string that appears on top of stack when a function fails to run
	fprintf(stderr, "Error: %s\n", lua_tostring (lua, -1));
	fprintf(stderr, "[ --------------------------------------------------------------------------]\n");
	lua_pop(lua, 1);  // pops error message from the stack
	}

// -----------------------------------------------------------------------------


