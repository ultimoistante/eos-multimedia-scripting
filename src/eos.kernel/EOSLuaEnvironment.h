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

#ifndef EOS_KERNEL_LUAENVIRONMENT_H
#define EOS_KERNEL_LUAENVIRONMENT_H

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include <assert.h>
#include <string.h>
#include <stdio.h>

/** This class wraps the Lua interpreter environment instance
 */

class EOSLuaEnvironment
    {
    public:
        /// constructor
        EOSLuaEnvironment();
        /// destructor
        virtual ~EOSLuaEnvironment();

        /// initializes Lua interpreter environment
        bool init();
        /// closes Lua interpreter environment
        void destroy();

        /// executes a Lua source, from a regular file
        bool runFile(const char* filename);
        /// executes a Lua source, from a memory buffer
        bool runBuffer(const unsigned char* bufferPtr, size_t bufferSize, const char* bufferName = NULL);

        /// calls a Lua function loaded in Lua stack
        bool callFunction(int nArgs, int nReturns = 0);

        /// returns a value from Lua runtime, in string format
        char* getLuaValueAsString(const char* luaVarName);
        /// Gets the pointer to the Lua interpreter environment
        lua_State* getLuaState() { return luaState; }

        /// emergency method called in case of Lua interpreter panic
        static void vmPanic(lua_State* lua);

        /// Checks if the VM is OK and can be still used
        virtual bool isOk() { return allOk; }

        // For debugging
        //void AttachDebugger (LuaDebugger *dbg) { m_pDbg = dbg; }

    protected:
        lua_State* luaState; ///< Lua interpreter environment
        bool       allOk;    ///< sanity-status flag
        //LuaDebugger *luaDbg;

    private:
        /// this function dumps detailed runtime Lua errors on stderr
        void handleError(lua_State* lua, int errorCode);
        //
        char tempValueString[1024]; ///< temporary buffer used by getLuaValueAsString
    };


#endif // EOS_KERNEL_LUAENVIRONMENT_H

