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

#ifndef EOS_KERNEL_PROCESS_H
#define EOS_KERNEL_PROCESS_H

#include "EOSLuaScript.h"
#include "EOSScriptManager.h"
#include "EOSLuaThisRestorer.h"
#include "EOSMessage.h"

#define  MAXCHUNKLENGTH  512


/** Class used to model an EOS Process (a single script instance). Many processes can share the same script source.
 */
class EOSProcess : public EOSLuaScript
    {
    public:
        /// constructor
        EOSProcess(EOSLuaEnvironment* luaEnvPtr, EOSScriptManager* scriptManagerPtr, const char* processName, const char* scriptName) : EOSLuaScript(luaEnvPtr, scriptName)
            {
            this->scriptManager = scriptManagerPtr;
            luaScriptBuffer* scriptBuffer = scriptManagerPtr->requestScript(scriptName);
            if(scriptBuffer != NULL)
                {
                this->valid = compileBuffer((unsigned char*)scriptBuffer->bufferPtr, scriptBuffer->bufferSize);
                }
            else
                printf("EOSProcess: '%s': NULL scriptBuffer!!\n", processName);
            this->thisProcessName = processName;
            this->active      = false;
            this->initialized = false;
            this->garbaged    = false;
            pushProcessName();
            }

        /// destructor
        ~EOSProcess()
            {
            if(initialized) cleanup();
            clearProcessName();
            }

        /// executes "init" function from script
        bool init()    { if(valid && selectScriptFunction("init"))    { go(); return true; } return false; }
        /// executes "update" function from script
        bool update()  { if(valid && selectScriptFunction("update"))  { go(); return true; } return false; }
        /// executes "cleanup" function from script
        bool cleanup() { if(valid && selectScriptFunction("cleanup")) { go(); return true; } return false; }
        /// executes "processMessage" function from script
        void processMessage(EOSMessage* message);

        /// returns the current activity status of this process
        bool isActive()             { return this->active; }
        /// sets the current activity status of this process
        bool setActive(bool status);
        /// returns the current garbage status of this process
        bool isGarbaged()           { return this->garbaged; }
        /// puts this process in "garbaged" status (a way to kill and deallocate it)
        void garbage()              { this->garbaged = true; this->active = false; }

        /// returns process name
        const char*  getName()      { return this->thisProcessName; }

    private:
        /// puts this process name in the Lua environment, under "this.processName"
        void pushProcessName();
        /// clears "this.processName" variable in the Lua environment
        void clearProcessName();
        //
        char luaChunk[MAXCHUNKLENGTH + 1]; ///< holds a temporary lua source buffer
        const char* thisProcessName;       ///< holds the name of this process
        bool active;                       ///< holds the current activity status of this process
        bool valid;                        ///< used to trace internally the validity of this process
        bool initialized;                  ///< used to trace internally the initialization status of this process
        bool garbaged;                     ///< holds the current "garbage" status of this process
        EOSScriptManager* scriptManager;   ///< pointer to the ScriptManager class
    };


#endif
