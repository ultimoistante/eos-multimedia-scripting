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

#ifndef EOS_KERNEL_LUA_THIS_RESTORER_H
#define EOS_KERNEL_LUA_THIS_RESTORER_H

#include "EOSLuaEnvironment.h"

/** This class is used to load the correct "this" reference each time the execution enters on a new script
 * <i>(based on code from Richard Shephard, published on CodeProject website: http://www.codeproject.com/cpp/luaincpp.asp)</i>
 */
class EOSLuaThisRestorer
    {
    public:
        EOSLuaThisRestorer(EOSLuaEnvironment* luaEnv, int thisRef) : oldThisRef (0), luaEnv (luaEnv)
            {
            lua_State* luaState = luaEnv->getLuaState();
            if(luaEnv->isOk())
                {
                // Save the old "this" table
                lua_getglobal(luaState, "this");
                oldThisRef = luaL_ref(luaState, LUA_REGISTRYINDEX);

                // replace it with our new one
                lua_rawgeti(luaState, LUA_REGISTRYINDEX, thisRef);
                lua_setglobal(luaState, "this");
                }
            }

    virtual ~EOSLuaThisRestorer()
        {
        lua_State* luaState = luaEnv->getLuaState();
        if(oldThisRef != 0 && luaEnv->isOk())
            {
            // Replace the old "this" table
            lua_rawgeti(luaState, LUA_REGISTRYINDEX, oldThisRef);
            lua_setglobal(luaState, "this");
            luaL_unref(luaState, LUA_REGISTRYINDEX, oldThisRef);
            }
        }

    protected:
        int oldThisRef;
        EOSLuaEnvironment* luaEnv;
    };

#endif
