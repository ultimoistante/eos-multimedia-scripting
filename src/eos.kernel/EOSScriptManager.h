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

#ifndef EOS_KERNEL_SCRIPTMANAGER_H
#define EOS_KERNEL_SCRIPTMANAGER_H

#include <map>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/timeb.h>


/** Simple structure to hold a source script buffer in memory
 */
struct luaScriptBuffer
    {
    void* bufferPtr;
    long  bufferSize;
    };


/** Class used to load and manage source scripts. Implements caching to avoid source buffer duplication in case of multiple processes with the same script.
 */
class EOSScriptManager
    {
    public:
        /// constructor
        EOSScriptManager(const char* scriptDirectory);
        /// destructor
        ~EOSScriptManager();
        //
        /// requests a script buffer from the cache
        luaScriptBuffer* requestScript(const char* scriptBaseName);
        //
    private:
        /// effectively loads a source script, putting it in the cache
        luaScriptBuffer* loadScript(const char* scriptBaseName);

        std::string scriptPath;   ///< path where to find scripts
        std::map<std::string, luaScriptBuffer*> scriptBuffers; ///< hashmap used to implement script cache
    };


#endif
