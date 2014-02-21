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


#include "EOSProcess.h"

// -----------------------------------------------------------------------------

void EOSProcess::processMessage(EOSMessage* message)
	{
	if(this->valid && message != NULL)
		{
		message->toLuaMessageBoardTable(this->luaEnv->getLuaState());
		if(selectScriptFunction("processMessage")) // selects 'processMessage' function...
			go();                                  // ...and executes it
		}
	}

// -----------------------------------------------------------------------------

bool EOSProcess::setActive(bool status)
	{
	if(!this->garbaged)
		{
		if(initialized)
			this->active = status;
		else
			{
			this->active = init();
			initialized = true;
			}
		}
	//
	return this->active;
	}

// -----------------------------------------------------------------------------

void EOSProcess::pushProcessName()
	{
	lua_State* luaRuntime = this->luaEnv->getLuaState();
	EOSLuaThisRestorer ltr(this->luaEnv, this->luaThisRef);
	snprintf(this->luaChunk, MAXCHUNKLENGTH, "this.processName = \"%s\"", this->thisProcessName);
	int error = luaL_loadbuffer(luaRuntime, this->luaChunk, strlen(this->luaChunk), "pushProcessName@EOSProcess") || lua_pcall(luaRuntime, 0, 0, 0);
	if(error)
		{
		fprintf(stderr, "%s", lua_tostring(luaRuntime, -1));
		lua_pop(luaRuntime, 1);  // pops error message from the stack
		}
	}

// -----------------------------------------------------------------------------

void EOSProcess::clearProcessName()
	{
	lua_State* luaRuntime = this->luaEnv->getLuaState();
	EOSLuaThisRestorer ltr(this->luaEnv, this->luaThisRef);
	sprintf(this->luaChunk, "this.processName = nil");
	int error = luaL_loadbuffer(luaRuntime, this->luaChunk, strlen(this->luaChunk), "cleanProcessName@EOSProcess") ||	lua_pcall(luaRuntime, 0, 0, 0);
	if(error)
		{
		fprintf(stderr, "%s", lua_tostring(luaRuntime, -1));
		lua_pop(luaRuntime, 1);  // pops error message from the stack
		}
	}

// -----------------------------------------------------------------------------
