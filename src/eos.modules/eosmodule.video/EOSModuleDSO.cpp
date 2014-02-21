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

#include <stdio.h>
#include <string>

#include "VideoManager.h"

extern "C"
    {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "EOSLuaRuntime.h"

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    //
    // Module manifest structure. Developer must edit just the #define values below.
    //
#define _MODULE_NAME            "video"
#define _MODULE_VERSION         "0.1"
#define _MODULE_DATE            "November 16, 2012"
#define _MODULE_DESC_SHORT      "FFmpeg based video decoding module for EOS Scripting Environment"
#define _MODULE_DESC_LONG       "based on FFmpeg library. See http://ffmpeg.org/ for more details."
#define _MODULE_AUTHOR_NAME     "EOS Scripting Team"
#define _MODULE_AUTHOR_WEBSITE  "http://sourceforge.net/projects/eos-scripting/"
    //
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    // -----------------------------------------------------------------------------


    // utility functions -----------------------------------------------------
    // end utility functions -------------------------------------------------


    // ====================== COMMON REQUIRED FUNCTIONS ============================
    // -----------------------------------------------------------------------------

    /*- @function eos.kinect.module.cleanup
     *  @brief automatically called from eos kernel on module unloading
     */
    static int moduleCleanup(lua_State* luaRuntime)
        {
        VideoManager::getInstance()->shutdown();
        //
        return 0;
        }

    // ====================== CUSTOM MODULE FUNCTIONS ==============================
    // -----------------------------------------------------------------------------

    static int initialize(lua_State* luaRuntime)
        {
        bool done = VideoManager::getInstance()->powerOn();
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    static int openStream(lua_State* luaRuntime)
        {
        bool done = false;
        //
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".stream.open", 1))
            {
            done = (VideoManager::getInstance()->openStream(luaL_checkstring(luaRuntime, 1)) == 0) ;
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /*- @function eos.kinect.update
     *  @brief updates module and active render targets
     *  @return updated (boolean) true or false
     */
    static int update(lua_State* luaRuntime)
        {
        VideoManager::getInstance()->update();
        //
        return 0;
        }

    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.kinect.renderTarget.create()
     * @brief create a new render target for output
     * @param renderTargetName name of the render target
     * @param width width in pixels
     * @param height height in pixels
     * @returns session object
     */
    static int getOutputRGB(lua_State* luaRuntime)
        {
        void* userdata = NULL;
        userdata = VideoManager::getInstance()->getRGBFramebufferObject();
        //    
        lua_pushlightuserdata(luaRuntime, userdata);
        return 1;
        }

    // -----------------------------------------------------------------------------
    // ========================== END LIBRARY FUNCTIONS ============================
    // -----------------------------------------------------------------------------

    /*class KeyValue
        {
    public:

        KeyValue(const char* key, int value)
            {
            this->key = strdup(key);
            this->value = value;
            }

        ~KeyValue()
            {
            free(this->key);
            }
        char* key;
        int value;
            };

    std::vector<KeyValue*> keysymbols;

    void loadModuleSymbols()
        {
        keysymbols.push_back(new KeyValue("EVT_NEWUSER", EVT_NEWUSER));
        keysymbols.push_back(new KeyValue("EVT_LOSTUSER", EVT_LOSTUSER));
        keysymbols.push_back(new KeyValue("EVT_POSEDETECTED", EVT_POSEDETECTED));
        keysymbols.push_back(new KeyValue("EVT_CALIBRATIONSTART", EVT_CALIBRATIONSTART));
        keysymbols.push_back(new KeyValue("EVT_CALIBRATIONEND", EVT_CALIBRATIONEND));
        keysymbols.push_back(new KeyValue("EVT_CALIBRATIONFAIL", EVT_CALIBRATIONFAIL));
        //
        char tmpstr[120];
        int totalsymbols = keysymbols.size();
        for (int i = 0; i < totalsymbols; i++)
            {
            sprintf(tmpstr, "eos.%s.%s = %d", _MODULE_NAME, keysymbols[i]->key, keysymbols[i]->value);
            EOSLuaRuntime::getInstance()->executeLuaChunk(tmpstr);
            delete keysymbols[i];
            }
        keysymbols.clear();
        }
     */


    // -----------------------------------------------------------------------------
    // ======================== LUA FUNCTIONS REGISTRATION =========================
    // -----------------------------------------------------------------------------

    // ==== Here we declare an array with all functions and their respective names.
    // ==== This array has elements of type luaL_reg, which is a structure with
    // ==== two fields: a string and a function pointer.

    const struct luaL_reg fn_table_module [] = {
        {"cleanup", moduleCleanup},
        {NULL, NULL} // <-- end-of-table sentinel
        };


    const struct luaL_reg  fn_table_stream[] = {
        {"open", openStream},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_main [] = {
        {"initialize", initialize},
        {"getOutputRGB", getOutputRGB},
        {"update", update},
                                              //{"shutdown",                     shutdown},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    /*const struct luaL_reg fn_table_image [] = {
            {NULL, NULL}  // <-- end-of-table sentinel
            };*/


    // ==== Finally, we declare a main (initialization) function, using luaL_openlib:

    int _INIT_(lua_State* luaRuntime)
        {
        EOSLuaRuntime::getInstance()->setRuntime(luaRuntime);
        //
        // let's fill the module manifest structure. Pay attention: it's in lua language!
        EOSLuaRuntime::getInstance()->executeLuaChunk(
                                                      "eos."_MODULE_NAME" = {}\
				 eos.module._new = eos."_MODULE_NAME"\
				 eos.module._new.module                        = {}\
				 eos.module._new.module.info                   = {}\
				 eos.module._new.module.info.name              = '"_MODULE_NAME".eosmodule'\
				 eos.module._new.module.info.tableName         = 'eos."_MODULE_NAME"'\
				 eos.module._new.module.info.version           = '"_MODULE_VERSION"'\
				 eos.module._new.module.info.date              = '"_MODULE_DATE"'\
				 eos.module._new.module.info.description       = {}\
				 eos.module._new.module.info.description.short = '"_MODULE_DESC_SHORT"'\
				 eos.module._new.module.info.description.long  = '"_MODULE_DESC_LONG"'\
				 eos.module._new.module.info.author            = {}\
				 eos.module._new.module.info.author.name       = '"_MODULE_AUTHOR_NAME"'\
				 eos.module._new.module.info.author.website    = '"_MODULE_AUTHOR_WEBSITE"'\
				 eos.module._new.module.reservedFunctions         = {}\
				 eos.module._new.module.reservedFunctions.init    = ''\
				 eos.module._new.module.reservedFunctions.cleanup = 'eos."_MODULE_NAME".module.cleanup'\
				");


        // registers "eos.*.module" functions
        luaL_register(luaRuntime, "eos."_MODULE_NAME".module", fn_table_module);

        // registers eos.* main functions
        luaL_register(luaRuntime, "eos."_MODULE_NAME, fn_table_main);

        // registers stream functions
        luaL_register(luaRuntime, "eos."_MODULE_NAME".stream", fn_table_stream);
        
        // loads module symbols
        //loadModuleSymbols();

        //
        return 1;
        }

    // -----------------------------------------------------------------------------

    } // extern "C" closure

