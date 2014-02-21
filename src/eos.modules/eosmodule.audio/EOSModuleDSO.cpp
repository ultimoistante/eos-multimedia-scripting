/*
-----------------------------------------------------------------------------
This source file is part of EOS Multimedia Scripting System
For the latest info, see https://github.com/ultimoistante/eos-multimedia-scripting

The MIT License (MIT)

Copyright (c) 2014-2019 of Salvatore Carotenuto aka ultimoistante of StartupSolutions
                          (mailto: ultimoistante@gmail.com | www.startupsolutions.it)
                       and Alessandro Rasulo (mailto: alex9978@gmail.com)

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

#include "EOSAudioDevice.h"
#include "EOSLuaRuntime.h"

extern "C"
    {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"

    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    //
    // Module manifest structure. Developer must edit just the #define values below.
    //
    #define _MODULE_NAME            "audio"
    #define _MODULE_VERSION         "0.9.99"
    #define _MODULE_DATE		    "November 06, 2012"
    #define _MODULE_DESC_SHORT      "OpenAL Soft powered audio module for EOS Scripting Environment"
    #define _MODULE_DESC_LONG       ""
    #define _MODULE_AUTHOR_NAME     "EOS Scripting Team"
    #define _MODULE_AUTHOR_WEBSITE  "http://sourceforge.net/projects/eos-scripting/ | http://alex9978.blogspot.it/"
    //
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------

    // utility functions -----------------------------------------------------
    // end utility functions -------------------------------------------------


    // ====================== COMMON REQUIRED FUNCTIONS ============================
    // -----------------------------------------------------------------------------

    static int moduleCleanup(lua_State* luaRuntime)
        {
        /*
         * insert here code for resources cleanup
         * on module unload (automatically done by kernel on close)
         */
        EOSLuaRuntime::getInstance()->logInfoMessage("[eos."_MODULE_NAME"] Shutting down...");
        EOSAudioDevice::instance()->shutdown();
        //
        return 0;
        }

    // ====================== CUSTOM MODULE FUNCTIONS ==============================

   
    static int moduleInitialize(lua_State* luaRuntime)
        {
        bool done = false;
        //if(EOSLuaRuntime::checkLuaParameters(_MODULE_NAME, "initialize", 1))
            done = EOSAudioDevice::instance()->initialize();
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }
    
    /*- @function eos.audio.update
     *  @brief updates audio module status (at this time, this module does not need to be updated in order to work, however use it in order to guarantee future compatibility)
     *  @return updated (boolean) true or false
     */
    static int moduleUpdate(lua_State* luaRuntime)
        {
        bool done = true;
        //if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".update", 1))
        EOSAudioDevice::instance()->update();
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.listener.setPosition()
     * @brief sets absolute listener position
     * @param x
     * @param y
     * @param z
     * @returns nothing
     */
    static int listenerSetPosition(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".listener.setPosition", 3))
            {
            EOSAudioDevice::instance()->listenerSetPosition(lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3));
            }
        //    		
        return 0;
        }

    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.listener.setVelocity()
     * @brief sets absolute listener velocity
     * @param x velocity
     * @param y velocity
     * @param z velocity
     * @returns nothing
     */
    static int listenerSetVelocity(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".listener.setVelocity", 3))
            {
            EOSAudioDevice::instance()->listenerSetVelocity(lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3));
            }
        //    		
        return 0;
        }

    // -----------------------------------------------------------------------------
    
    /*-
     * @name function eos.audio.listener.setOrientation()
     * @brief sets listener orientation
     * @param atx (at vector)
     * @param aty (at vector)
     * @param atz (at vector)
     * @param upx (up vector)
     * @param upy (up vector)
     * @param upz (up vector)
     * @returns nothing
     */
    static int listenerSetOrientation(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".listener.setOrientation", 6))
            EOSAudioDevice::instance()->listenerSetOrientation(lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4), lua_tonumber(luaRuntime, 5), lua_tonumber(luaRuntime, 6));
        //
        return 0;
        }
    
    // -----------------------------------------------------------------------------
    
    /*-
     * @name function eos.audio.listener.setOrientationAt()
     * @brief sets listener AT orientation
     * @param atx (at vector)
     * @param aty (at vector)
     * @param atz (at vector)
     * @returns nothing
     */
    static int listenerSetOrientationAt(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".listener.setOrientationAt", 3))
            EOSAudioDevice::instance()->listenerSetOrientationAt(lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3));
        //
        return 0;
        }
    
    // -----------------------------------------------------------------------------
    
    /*-
     * @name function eos.audio.listener.setOrientationUp()
     * @brief sets listener UP orientation
     * @param upx (up vector)
     * @param upy (up vector)
     * @param upz (up vector)
     * @returns nothing
     */
    static int listenerSetOrientationUp(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".listener.setOrientationUp", 3))
            EOSAudioDevice::instance()->listenerSetOrientationUp(lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3));
        //
        return 0;
        }
    
    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.listener.setGain()
     * @brief sets listener volume gain
     * @param volume gain (default: 1.0)
     * @returns true or false
     */
    static int listenerSetGain(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".listener.setGain", 1))
            {
            EOSAudioDevice::instance()->listenerSetGain(lua_tonumber(luaRuntime, 1));
            }
        //
        return 0;
        }

    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.setDistanceFactor()
     * @brief sets distance factor (to adjust distances to the scale adopted in your application)
     * @param value (default 1.0)
     * @returns nothing
     */
    static int setDistanceFactor(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".setDistanceFactor", 1))
            {
            EOSAudioDevice::instance()->setDistanceFactor(lua_tonumber(luaRuntime, 1));
            }
        //
        return 0;
        }
    
    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.setDopplerFactor()
     * @brief sets doppler factor
     * @param value (default 1.0)
     * @returns nothing
     */
    static int setDopplerFactor(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".setDopplerFactor", 1))
            {
            EOSAudioDevice::instance()->setDopplerFactor(lua_tonumber(luaRuntime, 1));
            }
        //
        return 0;
        }

    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.setRolloffFactor()
     * @brief sets doppler factor
     * @param value (default 1.0)
     * @returns nothing
     */
    static int setRolloffFactor(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".setRolloffFactor", 1))
            {
            EOSAudioDevice::instance()->setRolloffFactor(lua_tonumber(luaRuntime, 1));
            }
        //
        return 0;
        }
    
    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.setMaxDistance()
     * @brief sets the distance where there will no longer be any attenuation of the source
     * @param value (default 4000.0)
     * @returns nothing
     */
    static int setMaxDistance(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".setMaxDistance", 1))
            {
            EOSAudioDevice::instance()->setMaxDistance(lua_tonumber(luaRuntime, 1));
            }
        //
        return 0;
        }
    
    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.setReferenceDistance()
     * @brief sets the distance under which the volume for the source would normally drop by half
     * @param value (default 3000.0)
     * @returns nothing
     */
    static int setReferenceDistance(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".setReferenceDistance", 1))
            {
            EOSAudioDevice::instance()->setReferenceDistance(lua_tonumber(luaRuntime, 1));
            }
        //
        return 0;
        }

    // -----------------------------------------------------------------------------
    
    /*-
     * @name function eos.audio.sound.load()
     * @brief loads a sound (supported: OGG, WAV)
     * @param filename
     * @returns soundId (-1 if there is an error)
     */
    static int soundLoad(lua_State* luaRuntime)
        {
        int id = -1;
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.load", 1))
            {
            id = EOSAudioDevice::instance()->soundLoad((char *)luaL_checkstring(luaRuntime, 1));
            }
        //
        lua_pushinteger(luaRuntime, id);
        return 1;
        }
    
    // -----------------------------------------------------------------------------
    
    /*-
     * @name function eos.audio.sound.unload()
     * @brief unloads a sound from memory
     * @param soundId
     * @returns nothing
     */
    static int soundUnload(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.unload", 1))
            {
            EOSAudioDevice::instance()->soundUnload((int)lua_tonumber(luaRuntime, 1));
            }
        //        
        return 0;
        }

    // -----------------------------------------------------------------------------
    
    /*-
     * @name function eos.audio.sound.play()
     * @brief plays a sound
     * @param soundId
     * @param gain (default: 1.0)
     * @param isRelative (true if sound position is relative to listener)
     * @param loop
     * @returns nothing
     */
    static int soundPlay(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.play", 4))
            {
            EOSAudioDevice::instance()->soundPlay((int)lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_toboolean(luaRuntime, 3), lua_toboolean(luaRuntime, 4));
            }
        //        
        return 0;
        }
    
    // -----------------------------------------------------------------------------
    
    /*-
     * @name function eos.audio.sound.replay()
     * @brief plays a sound with the last params
     * @param soundId
     * @returns nothing
     */
    static int soundReplay(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.replay", 4))
            {
            EOSAudioDevice::instance()->soundReplay((int)lua_tonumber(luaRuntime, 1));
            }
        //        
        return 0;
        }
    
    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.sound.crossLap()
     * @brief crosslaps current sound with another one (supported: OGG)
     * @param a valid soundId of a previously loaded sound
     * @param a valid soundId, to crosslap with current sound
     * @returns true if success
     */
    static int soundCrossLap(lua_State* luaRuntime)
        {
        bool done = false;
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.crossLap", 2))
            {
            done = EOSAudioDevice::instance()->soundCrossLap((int)lua_tonumber(luaRuntime, 1), (int)lua_tonumber(luaRuntime, 2));
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }
    
    // -----------------------------------------------------------------------------
    
    /*-
     * @name function eos.audio.sound.pause()
     * @brief pauses a currently playing sound
     * @param soundId
     * @returns nothing
     */
    static int soundPause(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.pause", 1))
            {
            EOSAudioDevice::instance()->soundPause((int)lua_tonumber(luaRuntime, 1));
            }
        //        
        return 0;
        }    

    // -----------------------------------------------------------------------------
    
    /*-
     * @name function eos.audio.sound.unpause()
     * @brief resumes a paused sound
     * @param soundId
     * @returns nothing
     */
    static int soundUnpause(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.unpause", 1))
            {
            EOSAudioDevice::instance()->soundUnpause((int)lua_tonumber(luaRuntime, 1));
            }
        //        
        return 0;
        }    
    
    // -----------------------------------------------------------------------------
    
    /*-
     * @name function eos.audio.sound.mute()
     * @brief mute a currently playing sound
     * @param soundId
     * @returns nothing
     */
    static int soundMute(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.mute", 1))
            {
            EOSAudioDevice::instance()->soundMute((int)lua_tonumber(luaRuntime, 1));
            }
        //        
        return 0;
        }    
    
    // -----------------------------------------------------------------------------
    
    /*-
     * @name function eos.audio.sound.unmute()
     * @brief Resumes a muted sound
     * @param soundId
     * @returns nothing
     */
    static int soundUnmute(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.unmute", 1))
            {
            EOSAudioDevice::instance()->soundUnmute((int)lua_tonumber(luaRuntime, 1));
            }
        //        
        return 0;
        }    
    
    // -----------------------------------------------------------------------------
    
    /*-
     * @name function eos.audio.sound.stop()
     * @brief Stops a currently playing sound
     * @param soundId
     * @returns nothing
     */
    static int soundStop(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.stop", 1))
            {
            EOSAudioDevice::instance()->soundStop((int)lua_tonumber(luaRuntime, 1));
            }
        //        
        return 0;
        }       
    
    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.sound.setPosition()
     * @brief sets absolute sound position
     * @param soundId
     * @param px - new position
     * @param py
     * @param pz
     * @returns nothing
     */
    static int soundSetPosition(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.setPosition", 4))
            {
            EOSAudioDevice::instance()->soundSetPosition((int)lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4));
            }
        //    		
        return 0;
        }    
    
    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.sound.translate()
     * @brief translates sound position
     * @param soundId
     * @param px - new position
     * @param py 
     * @param pz
     * @param vx - velocity
     * @param vy
     * @param vz
     * @returns nothing
     */
    static int soundTranslate(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.translate", 7))
            {
            EOSAudioDevice::instance()->soundTranslate((int)lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4), lua_tonumber(luaRuntime, 5), lua_tonumber(luaRuntime, 6), lua_tonumber(luaRuntime, 7));
            }
        //    		
        return 0;
        }    
        
    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.sound.setPitch()
     * @brief sets the pitch of a sound
     * @param soundId
     * @param pitch (default 1.0)
     * @returns nothing
     */
    static int soundSetPitch(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.setPitch", 2))
            {
            EOSAudioDevice::instance()->soundSetPitch((int)lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2));
            }
        //    		
        return 0;
        }        
    
    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.sound.setGain()
     * @brief sets the gain of a sound
     * @param soundId
     * @param gain (default 1.0)
     * @returns nothing
     */
    static int soundSetGain(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.setGain", 2))
            {
            EOSAudioDevice::instance()->soundSetPitch((int)lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2));
            }
        //    		
        return 0;
        }         
 
    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.sound.flush()
     * @brief flushes queued buffers of a sound
     * @param soundId
     * @returns nothing
     */
    static int soundFlush(lua_State* luaRuntime)
        {
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.flush", 1))
            {
            EOSAudioDevice::instance()->soundFlush((int)lua_tonumber(luaRuntime, 1));
            }
        //    		
        return 0;
        }       
    
    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.sound.isPlaying()
     * @brief queries current play status of a sound
     * @param soundId
     * @returns true if playing
     */
    static int soundIsPlaying(lua_State* luaRuntime)
        {
        bool res = false;
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.isPlaying", 1))
            {
            res = EOSAudioDevice::instance()->soundIsPlaying((int)lua_tonumber(luaRuntime, 1));
            }
        //
        lua_pushboolean(luaRuntime, res);
        return 1;
        }    
 
    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.sound.isLooping()
     * @brief queries current loop status of a sound
     * @param soundId
     * @returns true if looping
     */
    static int soundIsLooping(lua_State* luaRuntime)
        {
        bool res = false;
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.isLooping", 1))
            {
            res = EOSAudioDevice::instance()->soundIsLooping((int)lua_tonumber(luaRuntime, 1));
            }
        //
        lua_pushboolean(luaRuntime, res);
        return 1;
        }    
    
    // -----------------------------------------------------------------------------

    /*-
     * @name function eos.audio.sound.isMuted()
     * @brief queries current mute status of a sound
     * @param soundId
     * @returns true if muted
     */
    static int soundIsMuted(lua_State* luaRuntime)
        {
        bool res = false;
        if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sound.isMuted", 1))
            {
            res = EOSAudioDevice::instance()->soundIsMuted((int)lua_tonumber(luaRuntime, 1));
            }
        //
        lua_pushboolean(luaRuntime, res);
        return 1;
        }    

    // -----------------------------------------------------------------------------
    // ========================== END LIBRARY FUNCTIONS ============================


    // ======================== LUA FUNCTIONS REGISTRATION =========================

    // ==== Here we declare an array with all functions and their respective names.
    // ==== This array has elements of type luaL_reg, which is a structure with
    // ==== two fields: a string and a function pointer.

    const struct luaL_reg fn_table_module [] = {
        {"cleanup", moduleCleanup},
        {"initialize", moduleInitialize},
        {"update", moduleUpdate},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_listener [] = {
        {"setPosition", listenerSetPosition},
        {"setVelocity", listenerSetVelocity},
        {"setOrientation", listenerSetOrientation},
        {"setOrientationAt", listenerSetOrientationAt},
        {"setOrientationUp", listenerSetOrientationUp},
        {"setGain", listenerSetGain},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_main [] = {
        {"setDistanceFactor", setDistanceFactor},
        {"setDopplerFactor", setDopplerFactor},
        {"setRolloffFactor", setRolloffFactor},
        {"setMaxDistance", setMaxDistance},
        {"setReferenceDistance", setReferenceDistance},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_sound [] = {
        {"load", soundLoad},
        {"unload", soundUnload},
        {"play", soundPlay},
        {"replay", soundReplay},
        {"crossLap", soundCrossLap},
        {"pause", soundPause},
        {"unpause", soundUnpause},
        {"mute", soundMute},
        {"unmute", soundUnmute},
        {"stop", soundStop},
        {"setPosition", soundSetPosition},
        {"translate", soundTranslate},
        {"setPitch", soundSetPitch},
        {"setGain", soundSetGain},
        {"flush", soundFlush},
        {"isPlaying", soundIsPlaying},
        {"isLooping", soundIsLooping},
        {"isMuted", soundIsMuted},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    // ==== Finally, we declare a main (initialization) function, using luaL_openlib:

    int _INIT_(lua_State* luaRuntime)
        {
        EOSLuaRuntime::getInstance()->setRuntime(luaRuntime);
        //
        // Never touch the manifest structure code below!
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

        // registers listener functions
        luaL_register(luaRuntime, "eos."_MODULE_NAME".listener", fn_table_listener);

        // registers main functions
        luaL_register(luaRuntime, "eos."_MODULE_NAME, fn_table_main);

        // registers sound functions
        luaL_register(luaRuntime, "eos."_MODULE_NAME".sound", fn_table_sound);
  
        //
        //
        return 1;
        }

    // -----------------------------------------------------------------------------

    } // extern "C" closure
