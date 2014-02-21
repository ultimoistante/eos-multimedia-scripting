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

#include "EOSLuaRuntime.h"

// -----------------------------------------------------------------------------

EOSLuaRuntime* EOSLuaRuntime::instance = NULL;
bool EOSLuaRuntime::instanceFlag = false;

// -----------------------------------------------------------------------------

EOSLuaRuntime* EOSLuaRuntime::getInstance()
	{
	if(!instanceFlag)
		{
		instance = new EOSLuaRuntime();
		instanceFlag = true;
		}
	return instance;
	}

// -----------------------------------------------------------------------------

//bool EOSLuaRuntime::checkLuaParameters(EOSModuleInfo* moduleInfo, int parametersRequired, const char* functionName)
bool EOSLuaRuntime::checkLuaParameters(const char* moduleName, const char* functionName, int parametersRequired)
	{
	if(lua_isnoneornil(EOSLuaRuntime::getInstance()->luaRuntime, parametersRequired))
		{
		EOSLuaRuntime::getInstance()->logErrorMessage("[%s] Error: too few or bad parameters to function '%s' (required: %d).", moduleName, functionName, parametersRequired);
		return false;
		}
	return true;
	}

// -----------------------------------------------------------------------------

bool EOSLuaRuntime::runFile(const char* filename)
	{
	bool success = false;
	int  err = 0;

	if((err = luaL_loadfile(this->luaRuntime, filename)) == 0)
		{
		// Call main...
		success = ((err = lua_pcall(this->luaRuntime, 0, LUA_MULTRET, 0)) == 0);
		}

	return success;
	}

// -----------------------------------------------------------------------------

bool EOSLuaRuntime::executeLuaChunk(const char* chunkString)
	{
	bool done = false;
	int error;
	if(this->luaRuntime != NULL && chunkString != NULL)
		{
		error = luaL_loadbuffer(luaRuntime, chunkString, strlen(chunkString), "executeLuaChunk@EOSLuaRuntime");
		if(error == 0)
			{
			error = lua_pcall(luaRuntime, 0, 0, 0);
			done = (error == 0);
			if(!done)
				handleError(luaRuntime, error, chunkString);
			else
				done = true;
			}
		else
			handleError(luaRuntime, error, chunkString);
		}
	return done;
	}

// -----------------------------------------------------------------------------

void EOSLuaRuntime::logInfoMessage(const char* format_string, ...)
	{
	if(this->luaRuntime != NULL)
		{
		va_list vlist;
		va_start(vlist, format_string);
		#ifdef _WIN32
		_vsnprintf(this->formatStringBuffer, 255, format_string, vlist);
		#else
		#ifdef linux
		vsnprintf(this->formatStringBuffer, 255, format_string, vlist);
		#endif
		#endif
		va_end(vlist);
		//
		std::string luaChunk = "eos.log.traceInfo(\"";
		luaChunk += formatStringBuffer;
		luaChunk += "\")";
		executeLuaChunk(luaChunk.c_str());
		}
	}

// -----------------------------------------------------------------------------

void EOSLuaRuntime::logErrorMessage(const char* format_string, ...)
	{
	if(this->luaRuntime != NULL)
		{
		va_list vlist;
		va_start(vlist, format_string);
		#ifdef _WIN32
		_vsnprintf(this->formatStringBuffer, 255, format_string, vlist);
		#else
		#ifdef linux
		vsnprintf(this->formatStringBuffer, 255, format_string, vlist);
		#endif
		#endif
		va_end(vlist);
		//
		std::string luaChunk = "eos.log.traceError(\"";
		luaChunk += formatStringBuffer;
		luaChunk += "\")";
		executeLuaChunk(luaChunk.c_str());
		}
	}

// -----------------------------------------------------------------------------

void EOSLuaRuntime::logWarningMessage(const char* format_string, ...)
	{
	if(this->luaRuntime != NULL)
		{
		va_list vlist;
		va_start(vlist, format_string);
		#ifdef _WIN32
		_vsnprintf(this->formatStringBuffer, 255, format_string, vlist);
		#else
		#ifdef linux
		vsnprintf(this->formatStringBuffer, 255, format_string, vlist);
		#endif
		#endif
		va_end(vlist);
		//
		std::string luaChunk = "eos.log.traceWarning(\"";
		luaChunk += formatStringBuffer;
		luaChunk += "\")";
		executeLuaChunk(luaChunk.c_str());
		}
	}

// -----------------------------------------------------------------------------

void EOSLuaRuntime::logUserMessage(const char* format_string, ...)
	{
	if(this->luaRuntime != NULL)
		{
		va_list vlist;
		va_start(vlist, format_string);
		#ifdef _WIN32
		_vsnprintf(this->formatStringBuffer, 255, format_string, vlist);
		#else
		#ifdef linux
		vsnprintf(this->formatStringBuffer, 255, format_string, vlist);
		#endif
		#endif
		va_end(vlist);
		//
		std::string luaChunk = "eos.log.traceUser(\"";
		luaChunk += formatStringBuffer;
		luaChunk += "\")";
		executeLuaChunk(luaChunk.c_str());
		}
	}

// -----------------------------------------------------------------------------

char* EOSLuaRuntime::getLuaValueAsString(const char* luaVarName)
	{
	//char* valueString = NULL;
	this->generalPurposeBuffer[0] = '\0';
	if(luaVarName != NULL)
		{
		char luaCommand[256];
		snprintf(luaCommand, 255, "__tmpLuaValue = %s", luaVarName);
		if(executeLuaChunk(luaCommand))
			{
			lua_getglobal(luaRuntime, "__tmpLuaValue");
			if(!lua_isnil(luaRuntime, -1))
				{
				strncpy(this->generalPurposeBuffer, (char*)lua_tostring(luaRuntime, -1), 1024);
				//valueString = this->tempValueString;
				}
			lua_pop(luaRuntime, 1);
			}
		}
	return this->generalPurposeBuffer;
	}

// -----------------------------------------------------------------------------

void EOSLuaRuntime::handleError(lua_State* lua, int errorCode, const char* chunkString)
	{
	printf("\n[EOSLuaRuntime -------------------------------------------------------------]\n");
  switch(errorCode)
		{
		case LUA_ERRRUN:
			printf ("| Runtime");
			break;
		case LUA_ERRSYNTAX:
			printf ("| Syntax ");
			break;
		case LUA_ERRFILE:
			printf ("| File ");
			break;
		case LUA_ERRMEM:
			printf ("| Fatal ");
			break;
		case LUA_ERRERR:
			printf ("| Fatal ");
			break;
		}
	// Get the error string that appears on top of stack when a function fails to run
	printf("Error: %s\n", lua_tostring (lua, -1));
	if(chunkString != NULL)
		printf("| Failed code chunk: '%s'\n", chunkString);
	printf("[ --------------------------------------------------------------------------]\n");
	lua_pop(luaRuntime, 1);  /* pop error message from the stack */
	}

// -----------------------------------------------------------------------------
