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

#ifndef EOS_KERNEL_LUASCRIPT_H
#define EOS_KERNEL_LUASCRIPT_H

#include "EOSLuaEnvironment.h"
#include "EOSLuaStackRestorer.h"
#include "EOSLuaThisRestorer.h"
#include <string>

/** This class represents a single EOS Lua source script
 */

class EOSLuaScript
    {
    public:
        /// constructor
        EOSLuaScript(EOSLuaEnvironment* luaEnvPtr, const char* scriptFileName);
        /// destructor
        virtual ~EOSLuaScript();

        /// calls Lua compiler on Lua code stored in a memory buffer
        bool compileBuffer(unsigned char* bufferPtr, size_t bufferSize);

        /// Selects a Lua Script function to call
        bool selectScriptFunction(const char* functionName);

        /// loads integer parameter to the selected function
        void addParam(int intValue);
        /// loads floating point parameter to the selected function
        void addParam(float floatValue);
        /// loads string parameter to the selected function
        void addParam(char* string);

        /// Runs the loaded script
        bool go(int nReturns = 0);

        /// tests if the script contains a function with a given name
        bool scriptHasFunction (const char* functionName);

    protected:
        EOSLuaEnvironment* luaEnv;     ///< pointer to the Lua runtime environment
        int                luaThisRef; ///< variable used to handle the "this" reference in scripts
        int                numArgs;    ///< counts the number of parameters loaded in the stack for the lua function
        const char*        currSelectedFuncName; ///< contains the name of the current Lua function to call
        std::string        scriptName; ///< contains the name of this script
    };


#endif
