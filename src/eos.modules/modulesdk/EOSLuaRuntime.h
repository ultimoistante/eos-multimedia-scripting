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

#ifndef EOS_MODULE_LUARUNTIME_H
#define EOS_MODULE_LUARUNTIME_H

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}
#include <string>
#include <string.h>

#define LOGINFO     EOSLuaRuntime::getInstance()->logInfoMessage
#define LOGERROR    EOSLuaRuntime::getInstance()->logErrorMessage
#define LOGWARNING  EOSLuaRuntime::getInstance()->logWarningMessage
#define LOGUSER     EOSLuaRuntime::getInstance()->logUserMessage
#define GETLUAVALUE EOSLuaRuntime::getInstance()->getLuaValueAsString


class EOSLuaRuntime
    {
    public:
        static EOSLuaRuntime* getInstance();
        //
        void setRuntime(lua_State* runtimePtr) { this->luaRuntime = runtimePtr; }
        lua_State* getRuntime() { return this->luaRuntime; }
        //
        static bool checkLuaParameters(const char* moduleName, const char* functionName, int parametersRequired);
        //
        bool  runFile(const char* filename);
        //
        bool  executeLuaChunk  (const char* chunkString);
        //
        void  logInfoMessage   (const char* format_string, ...);
        void  logErrorMessage  (const char* format_string, ...);
        void  logWarningMessage(const char* format_string, ...);
        void  logUserMessage   (const char* format_string, ...);
        //
        char* getLuaValueAsString(const char* luaVarName);
        //
    private:
        void  handleError(lua_State* lua, int errorCode, const char* chunkString);
        //
        static bool instanceFlag;
        static EOSLuaRuntime* instance;
        //
        char formatStringBuffer[256];
        char generalPurposeBuffer[1024];
        //
        EOSLuaRuntime() {}
        ~EOSLuaRuntime() {}
        //
        lua_State* luaRuntime;
    };


#endif

/* Changelog:
 14/01/2009:
    .changed "checkLuaParameters" function
    .removed "logModuleInfo" function
*/


