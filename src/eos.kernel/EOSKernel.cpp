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

#include "EOSKernel.h"

// -----------------------------------------------------------------------------

#define _VERSION_NUMBER		"0.9.1"
#define _VERSION_NAME		"Aurora"
#define _VERSION_DATE		"Nov 04, 2009"

// -----------------------------------------------------------------------------

std::ofstream EOSKernel::logStream;
bool EOSKernel::instanceFlag = false;
EOSKernel* EOSKernel::instance = NULL;
//
int EOSKernel::SAFEGUARD_ACTIVE = 1;
int EOSKernel::SAFEGUARD_FLAG_LOADMODULE = 2;

// -----------------------------------------------------------------------------
// ============= LUA INTERFACE - START =========================================
// -----------------------------------------------------------------------------

const struct luaL_reg EOSKernel::lua_eoskernel_main [] = {
    {"shutdown", EOSKernel::shutdown},
    {"sleep", EOSKernel::sleep},
    {"setTargetFrameRate", EOSKernel::setTargetFrameRate},
    {"getTargetFrameRate", EOSKernel::getTargetFrameRate},
    {"getLastFrameTime", EOSKernel::getLastFrameTime},
                                                          //{"getApplicationRoot",   EOSKernel::getApplicationRoot},
    {NULL, NULL} // <-- end-of-table sentinel
    };

const struct luaL_reg EOSKernel::lua_eoskernel_module [] = {
    {"load", EOSKernel::moduleLoad},
    {NULL, NULL} // <-- end-of-table sentinel
    };

const struct luaL_reg EOSKernel::lua_eoskernel_time [] = {
    {"getCurrent", EOSKernel::getCurrentTime},
    {"getCurrentMillis", EOSKernel::getCurrentTimeMillis},
    {NULL, NULL} // <-- end-of-table sentinel
    };

const struct luaL_reg EOSKernel::lua_eoskernel_process [] = {
    {"create", EOSKernel::processCreate},
    {"exists", EOSKernel::processExists},
    {"setActive", EOSKernel::processSetActive},
    {"isActive", EOSKernel::processIsActive},
    {"destroy", EOSKernel::processDestroy},
    {NULL, NULL} // <-- end-of-table sentinel
    };

const struct luaL_reg EOSKernel::lua_eoskernel_message [] = {
    {"init", EOSKernel::messageInit},
    {"pushData", EOSKernel::messagePushData},
    {"send", EOSKernel::messageSend},
    {NULL, NULL} // <-- end-of-table sentinel
    };

const struct luaL_reg EOSKernel::lua_eoskernel_log [] = {
    {"setConsoleLogLevel", EOSKernel::setConsoleLogLevel},
    {"traceError", EOSKernel::traceError},
    {"traceWarning", EOSKernel::traceWarning},
    {"traceDebug", EOSKernel::traceDebug},
    {"traceUser", EOSKernel::traceUser},
    {"traceInfo", EOSKernel::traceInfo},
    {NULL, NULL} // <-- end-of-table sentinel
    };

// -----------------------------------------------------------------------------
// ------------------------- LUA INTERFACE - Start -----------------------------
// -----------------------------------------------------------------------------

/**
 * <i>(Lua binding interface: mapped to <b>eos.module.load</b>)</i><br><br>
 * Loads a module in EOS environment
 * @param moduleName filename (relative to eos executable location) of the EOS Dynamic Module to load
 * @return true in case of success, false otherwise
 */
int EOSKernel::moduleLoad(lua_State* luaRuntime)
    {
    /* standard "loadlib" lua code:
    module_init = loadlib("MyModule.dll", "_INIT_FUNCTION_") -- for Windows
    --module_init = loadlib("MyModule.so", "_INIT_FUNCTION_") -- for Unix/Linux
    assert(module_init) -- name sure it's not nil
    module_init()       -- call the init function of the lib
     */
    bool moduleLoaded = false;

    const char* moduleName = luaL_checkstring(luaRuntime, 1);
    if (moduleName != NULL)
        {
        lua_getglobal(luaRuntime, "package"); // retrieves "package" global table
        lua_pushstring(luaRuntime, "loadlib"); // pushes "loadlib" string
        lua_gettable(luaRuntime, -2); // gets "package.loadlib" function
        lua_pushstring(luaRuntime, moduleName); // pushes 1st argument: the name of the module DSO
        lua_pushstring(luaRuntime, "_INIT_"); // pushes 2nd argument: init function name (always "_INIT_" for EOSModules)

        // do the call (2 arguments, 3 results)
        if (lua_pcall(luaRuntime, 2, 3, 0) == 0)
            {
            if (!lua_isnil(luaRuntime, 3)) // make sure returned value is not nil
                {
                // removes two values not important from stack
                lua_pop(luaRuntime, 2); // pop head stack
                // gets the reference to init function
                int initFunctionRef = lua_ref(luaRuntime, 1);
                // pushes just retrieved function on stack
                lua_getref(luaRuntime, initFunctionRef);
                // and calls (executes) it
                lua_call(luaRuntime, 0, 0); // call the init function of the lib
                lua_pop(luaRuntime, 2); // pop func from stack

                // 14/01/09 module structure and loading improvement:
                char cmdString[256];
                // adds loaded module to the loaded module list
                snprintf(cmdString, 255, "if(eos.module.list == nil) then eos.module.list = {} end eos.module.list[eos.module._new.module.info.name] = eos.module._new");
                EOSKernel::getInstance()->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
                // logs module info...
                snprintf(cmdString, 255, "eos.log.traceInfo('[eos.kernel] >>> loaded module '..eos.module._new.module.info.name)");
                EOSKernel::getInstance()->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
                snprintf(cmdString, 255, "eos.log.traceInfo('              +  '..eos.module._new.module.info.name..' v.'..eos.module._new.module.info.version..' - '..eos.module._new.module.info.date)");
                EOSKernel::getInstance()->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
                snprintf(cmdString, 255, "eos.log.traceInfo('              +  '..eos.module._new.module.info.description.short)");
                EOSKernel::getInstance()->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
                snprintf(cmdString, 255, "eos.log.traceInfo('              +  table name: '..eos.module._new.module.info.tableName)");
                EOSKernel::getInstance()->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
                snprintf(cmdString, 255, "eos.log.traceInfo('              +  author : '..eos.module._new.module.info.author.name)");
                EOSKernel::getInstance()->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
                snprintf(cmdString, 255, "eos.log.traceInfo('              +  website: '.._NEW_MODULE_LOADED_._moduleinfo.author.website)");
                EOSKernel::getInstance()->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
                snprintf(cmdString, 255, "eos.module._new = nil");
                EOSKernel::getInstance()->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));

                moduleLoaded = true;
                }
            else
                {
                EOSKernel::getInstance()->logMessage(LOGLEVEL_ERR, "[eos.kernel] - unable to load module '%s'. Cause: \"%s\"\n", moduleName, lua_tostring(luaRuntime, 4));
                //
                // marks "load module" error in safeguard mechanism
                EOSKernel::getInstance()->safeguardRegistry |= EOSKernel::SAFEGUARD_FLAG_LOADMODULE;
                }
            }
        else
            {
            EOSKernel::getInstance()->logMessage(LOGLEVEL_ERR, "[eos.kernel.loadmodule()] - unable to issue lua 'loadlib' function\n");
            }
        }
    else
        {
        EOSKernel::getInstance()->logMessage(LOGLEVEL_ERR, "[eos.kernel.loadmodule()] - invalid (nil) module name!\n");
        }

    lua_pushboolean(luaRuntime, moduleLoaded);
    return 1;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.shutdown</b>)</i><br><br>
 * Shuts down the kernel (closes the application)
 */
int EOSKernel::shutdown(lua_State* luaRuntime)
    {
    EOSKernel::getInstance()->stop();
    return 0;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.sleep</b>)</i><br><br>
 * Suspends kernel execution for a given number of milliseconds
 * @param milliseconds number of milliseconds to sleep
 */
int EOSKernel::sleep(lua_State* luaRuntime)
    {
    if (!lua_isnoneornil(luaRuntime, 1))
        EOSKernel::getInstance()->pause((unsigned int) lua_tonumber(luaRuntime, 1));
    return 0;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.time.getCurrent</b>)</i><br><br>
 * Returns the time (in seconds) since application started
 * @return elapsed time since application started
 */
int EOSKernel::getCurrentTime(lua_State* luaRuntime)
    {
    lua_pushnumber(luaRuntime, EOSKernel::getInstance()->currentTime());
    return 1;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.time.getCurrentMillis</b>)</i><br><br>
 * Returns the time (in milliseconds) since application started
 * @return elapsed time since application started
 */
int EOSKernel::getCurrentTimeMillis(lua_State* luaRuntime)
    {
    lua_pushnumber(luaRuntime, EOSKernel::getInstance()->currentTimeMillis());
    return 1;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.setTargetFrameRate</b>)</i><br><br>
 * Sets the target frame rate of the kernel (number of "update" executions per second)
 * @param targetFrameRate requested target frame rate
 */
int EOSKernel::setTargetFrameRate(lua_State* luaRuntime)
    {
    if (!lua_isnoneornil(luaRuntime, 1))
        {
        const char* requestedFrameRate = lua_tostring(luaRuntime, 1);
        EOSKernel::getInstance()->_setFrameRate(requestedFrameRate);
        }
    return 0;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.getTargetFrameRate</b>)</i><br><br>
 * Returns the current target frame rate of the kernel
 * @return current frame rate of the kernel
 */
int EOSKernel::getTargetFrameRate(lua_State* luaRuntime)
    {
    lua_pushnumber(luaRuntime, (float) (1.0 / EOSKernel::getInstance()->targetFrameTime));
    return 1;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.getLastFrameTime</b>)</i><br><br>
 * Returns the duration (in seconds) of last kernel iteration (formerly a 'frame')
 * @return duration of last frame, in seconds
 */
int EOSKernel::getLastFrameTime(lua_State* luaRuntime)
    {
    lua_pushnumber(luaRuntime, (EOSKernel::getInstance()->lastFrameTime));
    return 1;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.process.create</b>)</i><br><br>
 * Returns the time (in seconds) since application started
 * @param processName - name of the new Process to create. The name must be unique.
 * @param scriptName - name of the script to use for the process. A script can be used for many processes.
 * @return true in case of success, false otherwise
 */
int EOSKernel::processCreate(lua_State* luaRuntime)
    {
    bool done = false;
    if (!lua_isnoneornil(luaRuntime, 1) && !lua_isnoneornil(luaRuntime, 2))
        done = EOSKernel::getInstance()->createProcess(luaL_checkstring(luaRuntime, 1), luaL_checkstring(luaRuntime, 2));
    lua_pushboolean(luaRuntime, done);
    return 1;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.process.exists</b>)</i><br><br>
 * Tests the existence of a process
 * @param processName - name of Process to test
 * @return true if the process exists, false otherwise
 */
int EOSKernel::processExists(lua_State* luaRuntime)
    {
    bool exists = false;
    if (!lua_isnoneornil(luaRuntime, 1))
        exists = (EOSKernel::getInstance()->processByName(luaL_checkstring(luaRuntime, 1)) != NULL);
    lua_pushboolean(luaRuntime, exists);
    return 1;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.process.setActive</b>)</i><br><br>
 * Sets the activity status of a Process
 * @param processName - name of the Process
 * @param activity - the activity status (true|false) to set
 * @return true if case of success, false otherwise
 */
int EOSKernel::processSetActive(lua_State* luaRuntime)
    {
    bool done = false;
    if (!lua_isnoneornil(luaRuntime, 1) && !lua_isnoneornil(luaRuntime, 2))
        done = EOSKernel::getInstance()->setProcessActivity(luaL_checkstring(luaRuntime, 1), lua_toboolean(luaRuntime, 2));
    else
        EOSKernel::getInstance()->logMessage(LOGLEVEL_ERR, "[eos.process.setActive()] - Error: 2 parameters (string, boolean) required.\n");
    //
    lua_pushboolean(luaRuntime, done);
    return 1;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.process.isActive</b>)</i><br><br>
 * Sets the activity status of a Process
 * @param processName - name of the Process
 * @return current activity status of the process (true: active, false: inactive)
 */
int EOSKernel::processIsActive(lua_State* luaRuntime)
    {
    bool active = false;
    if (!lua_isnoneornil(luaRuntime, 1))
        {
        const char* processName = luaL_checkstring(luaRuntime, 1);
        EOSProcess* theProcess = EOSKernel::getInstance()->processByName(processName);
        if (theProcess != NULL)
            active = theProcess->isActive();
        else
            {
            EOSKernel::getInstance()->logMessage(LOGLEVEL_WARN, "[eos.process.isActive()] - Warning: process named '%s' doesn't exists.\n", processName);
            }
        }
    else
        EOSKernel::getInstance()->logMessage(LOGLEVEL_ERR, "[eos.process.isActive()] - Error: 1 parameter (string) required.\n");
    //
    lua_pushboolean(luaRuntime, active);
    return 1;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.process.destroy</b>)</i><br><br>
 * Destroys a Process
 * @param processName - name of the Process to destroy
 * @return true if case of success, false otherwise
 */
int EOSKernel::processDestroy(lua_State* luaRuntime)
    {
    bool done = false;
    if (!lua_isnoneornil(luaRuntime, 1))
        done = EOSKernel::getInstance()->destroyProcess(luaL_checkstring(luaRuntime, 1));
    else
        EOSKernel::getInstance()->logMessage(LOGLEVEL_ERR, "[eos.process.destroy()] - Error: 1 parameter (string) required.\n");
    //
    lua_pushboolean(luaRuntime, done);
    return 1;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.message.init</b>)</i><br><br>
 * Initializes a message structure
 * @param messageClass - class of the message (tipically a string that identifies the sender, or the type of the message)
 */
int EOSKernel::messageInit(lua_State* luaRuntime)
    {
    if (!lua_isnoneornil(luaRuntime, 1))
        EOSKernel::getInstance()->prepareMessage(luaL_checkstring(luaRuntime, 1));
    else
        EOSKernel::getInstance()->logMessage(LOGLEVEL_ERR, "[eos.message.init()] - Error: 1 parameter (string) required.\n");
    //
    return 0;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.message.pushData</b>)</i><br><br>
 * Pushes a data pair (key, value) in a previously initialized message (see messageInit)
 * @param keyName - name of the data field to push (the 'key')
 * @param valueData - value to push: it can be a Boolean, a Numeric, or a String
 */
int EOSKernel::messagePushData(lua_State* luaRuntime)
    {
    if (!lua_isnoneornil(luaRuntime, 1) && !lua_isnoneornil(luaRuntime, 2))
        {
        const char* keyName = lua_tostring(luaRuntime, 1);
        if (keyName != NULL)
            {
            if (lua_isboolean(luaRuntime, 2))
                EOSKernel::getInstance()->pushMessageData(keyName, (bool)lua_toboolean(luaRuntime, 2));
            else if (lua_isnumber(luaRuntime, 2))
                EOSKernel::getInstance()->pushMessageData(keyName, (float) lua_tonumber(luaRuntime, 2));
            else if (lua_isstring(luaRuntime, 2))
                EOSKernel::getInstance()->pushMessageData(keyName, lua_tostring(luaRuntime, 2));
            }
        }
    return 0;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.message.send</b>)</i><br><br>
 * Sends a previously initialized and prepared message
 * @param receiverProcessName - name of the receiver process
 */
int EOSKernel::messageSend(lua_State* luaRuntime)
    {
    if (!lua_isnoneornil(luaRuntime, 1))
        EOSKernel::getInstance()->sendPreparedMessage(lua_tostring(luaRuntime, 1));
    return 0;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.log.setConsoleLogLevel</b>)</i><br><br>
 * Sets the verbosity level (on stderr) of the EOS Kernel
 * @param verbosityLevel - max message level to dump on stderr. (A constant between eos.kernel.[ LOGLEVEL_NONE | LOGLEVEL_ERR | LOGLEVEL_WARN | LOGLEVEL_DBUG | LOGLEVEL_USER | LOGLEVEL_INFO ] )
 */
int EOSKernel::setConsoleLogLevel(lua_State* luaRuntime)
    {
    EOSKernel::getInstance()->consoleVerbosityLevel = (unsigned int) lua_tonumber(luaRuntime, 1);
    return 0;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.log.traceError</b>)</i><br><br>
 * Writes a line in eos.log logfile, with 'error' class mark ([E])
 * @param logLine - line to write to log
 */
int EOSKernel::traceError(lua_State* luaRuntime)
    {
    EOSKernel::getInstance()->logMessage(LOGLEVEL_ERR, "%s\n", luaL_checkstring(luaRuntime, 1));
    return 0;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.log.traceWarning</b>)</i><br><br>
 * Writes a line in EOS.log logfile, with 'warning' class mark ([W])
 * @param logLine - line to write to log
 */
int EOSKernel::traceWarning(lua_State* luaRuntime)
    {
    EOSKernel::getInstance()->logMessage(LOGLEVEL_WARN, "%s\n", luaL_checkstring(luaRuntime, 1));
    return 0;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.log.traceDebug</b>)</i><br><br>
 * Writes a line in EOS.log logfile, with 'debug' class mark ([D])
 * @param logLine - line to write to log
 */
int EOSKernel::traceDebug(lua_State* luaRuntime)
    {
    EOSKernel::getInstance()->logMessage(LOGLEVEL_DBUG, "%s\n", luaL_checkstring(luaRuntime, 1));
    return 0;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.log.traceUser</b>)</i><br><br>
 * Writes a line in EOS.log logfile, with 'user' class mark ([U])
 * @param logLine - line to write to log
 */
int EOSKernel::traceUser(lua_State* luaRuntime)
    {
    EOSKernel::getInstance()->logMessage(LOGLEVEL_USER, "%s\n", luaL_checkstring(luaRuntime, 1));
    return 0;
    }

// -----------------------------------------------------------------------------

/** <i>(Lua binding interface: mapped to <b>eos.log.traceInfo</b>)</i><br><br>
 * Writes a line in EOS.log logfile, with 'info' class mark ([I])
 * @param logLine - line to write to log
 */
int EOSKernel::traceInfo(lua_State* luaRuntime)
    {
    EOSKernel::getInstance()->logMessage(LOGLEVEL_INFO, "%s\n", luaL_checkstring(luaRuntime, 1));
    return 0;
    }


// -----------------------------------------------------------------------------

/*
        <function name="serializeTable">
                <language code="en">
                        <brief text="Serializes a LUA table to a given file" />
                        <description text="" />
                        <params>
                                <param name="luaTable" type="LUA Table" desc="the table to serialize" />
                        </params>
                        <returns>
                        </returns>
                </language>
                <language code="it">
                        <brief text="Serializza una Table LUA in un file" />
                        <description text="" />
                        <params>
                                <param name="luaTable" type="LUA Table" desc="la table da serializzare" />
                        </params>
                        <returns>
                        </returns>
                </language>
        </function>
 */
/* the function body is defined in native lua code below, in void EOSKernel::registerNativeLuaFunctions() */


// -----------------------------------------------------------------------------
// ============= LUA INTERFACE - END ===========================================
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------

EOSKernel* EOSKernel::getInstance()
    {
    if (!instanceFlag)
        {
        instance = new EOSKernel();
        instanceFlag = true;
        }
    return instance;
    }

// -----------------------------------------------------------------------------

EOSKernel::EOSKernel()
    {
    this->masterProcess = NULL;
    this->temporaryMessage = NULL;
    this->kernelTimer = NULL;
    this->consoleVerbosityLevel = LOGLEVEL_ERR;
    // safeGuard: default active
    this->safeguardRegistry = SAFEGUARD_ACTIVE;
    //
    this->justCompileScripts = false;
    this->controlStreamsEnabled = false;
    //
    this->socketHandler = NULL;
    }

// -----------------------------------------------------------------------------

void EOSKernel::stop()
    {
    if (this->masterProcess != NULL)
        this->masterProcess->setActive(false);
    }

// -----------------------------------------------------------------------------

int EOSKernel::init()
    {
    std::string tmpPath = this->applicationPath;
    //
    // checks if 'logs' directory exists under current application tree. If not, creates it.
    tmpPath += "/logs/";
    struct stat st;
    if (stat(tmpPath.c_str(), &st) != 0)
        mkdir(tmpPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    //
    tmpPath += "eos.log";
    this->logStream.open(tmpPath.c_str(), std::ios::out);
    if (this->logStream.is_open())
        {
        char datestr[32];
        char timestr[32];
        //
        time_t now;
        struct tm* currenttime;
        time(&now);
        currenttime = localtime(&now);
        sprintf(datestr, "%02d/%02d/%02d", currenttime->tm_mday, currenttime->tm_mon + 1, 1900 + currenttime->tm_year);
        sprintf(timestr, "%02d:%02d:%02d", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
        //
        logStream << "EOS Application started on " << datestr << " - " << timestr << "." << std::endl;
        }
    //
    logMessage(LOGLEVEL_INFO, "EOS Integrated Scripting System - Kernel v.%s (%s), %s\n", _VERSION_NUMBER, _VERSION_NAME, _VERSION_DATE);
    logMessage(LOGLEVEL_INFO, "(C) 2009-2013 Salvatore Carotenuto, aka ultimoistante (mailto: ultimoistante@gmail.com)\n");
    //
    logMessage(LOGLEVEL_INFO, "[eos.kernel.init()] - Instancing Lua environment... ");
    this->luaEnvironment = new EOSLuaEnvironment();
    if (this->luaEnvironment->init())
        {
        logMessage(LOGLEVEL_NOHEADER, "done.\n");
        }
    else
        { // FATAL error!
        logMessage(LOGLEVEL_ERR, "\n\t\tFATAL: Can't instance Lua environment!\n");
        return -1;
        }
    //
    char cmdString[256];

    // registers main environent table
    snprintf(cmdString, 255, "eos = { }");
    this->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
    //
    logMessage(LOGLEVEL_INFO, "[eos.kernel.init()] - Registering functions groups in Lua environment...\n");
    logMessage(LOGLEVEL_INFO, "   + [ main        ]\n");
    luaL_openlib(this->luaEnvironment->getLuaState(), "eos", lua_eoskernel_main, 0);
    logMessage(LOGLEVEL_INFO, "   + [ module      ]\n");
    snprintf(cmdString, 255, "eos.module = {}");
    this->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
    luaL_openlib(this->luaEnvironment->getLuaState(), "eos.module", lua_eoskernel_module, 0);
    logMessage(LOGLEVEL_INFO, "   + [ time        ]\n");
    snprintf(cmdString, 255, "eos.time = {}");
    this->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
    luaL_openlib(this->luaEnvironment->getLuaState(), "eos.time", lua_eoskernel_time, 0);
    logMessage(LOGLEVEL_INFO, "   + [ process     ]\n");
    snprintf(cmdString, 255, "eos.process = {}");
    this->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
    luaL_openlib(this->luaEnvironment->getLuaState(), "eos.process", lua_eoskernel_process, 0);
    logMessage(LOGLEVEL_INFO, "   + [ message     ]\n");
    snprintf(cmdString, 255, "eos.message = {}");
    this->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
    luaL_openlib(this->luaEnvironment->getLuaState(), "eos.message", lua_eoskernel_message, 0);
    logMessage(LOGLEVEL_INFO, "   + [ log         ]\n");
    snprintf(cmdString, 255, "eos.log = {}");
    this->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
    luaL_openlib(this->luaEnvironment->getLuaState(), "eos.log", lua_eoskernel_log, 0);
    //
    /* <symbol name="LOGLEVEL_NONE"><language code="en"><description text="NONE console verbosity level"/></language><language code="it"><description text="livello NONE di output di log su console"/></language></symbol> */
    sprintf(cmdString, "eos.kernel.LOGLEVEL_NONE = %d", LOGLEVEL_NONE);
    this->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
    /* <symbol name="LOGLEVEL_ERR"><language code="en"><description text="ERR console verbosity level"/></language><language code="it"><description text="livello ERR di output di log su console"/></language></symbol> */
    sprintf(cmdString, "eos.kernel.LOGLEVEL_ERR  = %d", LOGLEVEL_ERR);
    this->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
    /* <symbol name="LOGLEVEL_WARN"><language code="en"><description text="WARN console verbosity level"/></language><language code="it"><description text="livello WARN di output di log su console"/></language></symbol> */
    sprintf(cmdString, "eos.kernel.LOGLEVEL_WARN = %d", LOGLEVEL_WARN);
    this->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
    /* <symbol name="LOGLEVEL_DBUG"><language code="en"><description text="DEBUG console verbosity level"/></language><language code="it"><description text="livello DEBUG di output di log su console"/></language></symbol> */
    sprintf(cmdString, "eos.kernel.LOGLEVEL_DBUG = %d", LOGLEVEL_DBUG);
    this->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
    /* <symbol name="LOGLEVEL_USER"><language code="en"><description text="USER console verbosity level"/></language><language code="it"><description text="livello USER di output di log su console"/></language></symbol> */
    sprintf(cmdString, "eos.kernel.LOGLEVEL_USER = %d", LOGLEVEL_USER);
    this->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
    /* <symbol name="LOGLEVEL_INFO"><language code="en"><description text="INFO console verbosity level"/></language><language code="it"><description text="livello INFO di output di log su console"/></language></symbol> */
    sprintf(cmdString, "eos.kernel.LOGLEVEL_INFO = %d", LOGLEVEL_INFO);
    this->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
    //
    registerNativeLuaFunctions();
    //
    //
    //# initializes 'eos.application' table and fills it with current application properties
    std::string luaCommand = "eos.application = {}";
    this->luaEnvironment->runBuffer((const unsigned char*) luaCommand.c_str(), luaCommand.length());
    luaCommand = "eos.application.path = \"";
    luaCommand += this->applicationPath;
    luaCommand += "\"";
    this->luaEnvironment->runBuffer((const unsigned char*) luaCommand.c_str(), luaCommand.length());
    //
    //
    std::string scriptsPath = this->applicationPath;
    scriptsPath += "/scripts/";
    std::string settingsPath = "./";
    //
    //
    if (this->makeScripts(scriptsPath.c_str()) != 0)
        {
        logMessage(LOGLEVEL_ERR, "[eos.kernel.init()] - FATAL: error(s) in script(s). Exiting.\n");
        return -1;
        }
    //
    if (justCompileScripts == true)
        {
        logMessage(LOGLEVEL_INFO, "[eos.kernel.init()] - Kernel launched in --norun mode. Initialization stops here.\n");
        return 1;
        }
    //
    // enables control streams, if requested
    if (this->controlStreamsEnabled == true)
        {
        logMessage(LOGLEVEL_INFO, "[eos.kernel.init()] - enabling control streams (.eos-input and .eos-output)\n");
        //
        removeNamedFifos();
        createNamedFifos();
        }
    //
    // enables debug socket, if requested
    if (this->debugSocketEnabled == true)
        {
        logMessage(LOGLEVEL_INFO, "[eos.kernel.init()] - enabling debug socket\n");
        //
        this->socketHandler = new UDPSocketHandler(DEBUG_SOCKET_PORT);
        this->remoteCommandParser = new EOSRemoteCommandParser();
        }

    // # Kernel Timer initialization
    this->kernelTimer = new EOSPrecisionTimer();
    this->kernelTimer->reset();
    //
    //
    // # ScriptManager initialization
    logMessage(LOGLEVEL_INFO, "[eos.kernel.init()] - Initializing ScriptManager... ");
    this->scriptManager = new EOSScriptManager(scriptsPath.c_str());
    if (this->scriptManager == NULL)
        {
        // FATAL!!
        logMessage(LOGLEVEL_ERR, "\n[eos.kernel.init()] - Abnormal program conditions. Exiting.\n");
        return -1;
        }
    else
        logMessage(LOGLEVEL_NOHEADER, "done.\n");
    //
    //
    // # MasterProcess initialization
    logMessage(LOGLEVEL_INFO, "[eos.kernel.init()] - Loading MasterProcess script...\n");
    this->masterProcess = new EOSProcess(this->luaEnvironment, this->scriptManager, "MasterProcess", "MasterProcess");
    if (!masterProcess->setActive(true))
        {
        // FATAL!!
        logMessage(LOGLEVEL_ERR, "[eos.kernel.init()] - FATAL: unable to activate MasterProcess. Exiting.\n");
        return -1;
        }
    else
        logMessage(LOGLEVEL_INFO, "[eos.kernel.init()] - MasterProcess init() executed.\n");


    this->lastFrameTime = kernelTimer->getMilliseconds();
    this->frameStart = 0;
    //

    logMessage(LOGLEVEL_INFO, "[eos.kernel.init()] - initialization complete.\n");

    return 0;
    }

// -----------------------------------------------------------------------------

void EOSKernel::removeNamedFifos()
    {
    unlink(CONTROL_FIFO_INPUT);
    unlink(CONTROL_FIFO_OUTPUT);
    }

// -----------------------------------------------------------------------------

void EOSKernel::createNamedFifos()
    {
    (void) umask(S_IRWXO);
    mknod(CONTROL_FIFO_INPUT, S_IFIFO | S_IWUSR | S_IWGRP, 0);
    mknod(CONTROL_FIFO_OUTPUT, S_IFIFO | S_IRUSR | S_IRGRP, 0);
    }

// -----------------------------------------------------------------------------

int EOSKernel::makeScripts(const char* scriptsPath)
    {
    int errorStatus = 0;

    char* startWorkingDir = getcwd(NULL, 0);

    if (chdir(startWorkingDir) == 0 && chdir(scriptsPath) == 0)
        {
        DIR *dp;
        struct dirent *ep;

        dp = opendir("./");
        if (dp != NULL)
            {
            printf("\n***** eos.kernel - compiling scripts... *******************************\n");
            while ((ep = readdir(dp)) && (errorStatus == 0))
                {
                std::string filename = ep->d_name;
                if (filename.find("eos", filename.length() - 3) != std::string::npos)
                    {
                    std::string infilename = filename;
                    std::string outfilename = filename;

                    outfilename[outfilename.length() - 2] = 'c'; // .eos -> .ecs
                    char systemcmd[512];
                    sprintf(systemcmd, "%s/luac -o \"%s\" \"%s\"", startWorkingDir, outfilename.c_str(), infilename.c_str());

                    printf(">> luac: compiling '%s'...\n", infilename.c_str());
                    int luacrc = system(systemcmd);
                    if (luacrc == 0)
                        printf("[OK] '%s' compiled successfully\n", infilename.c_str());
                    else
                        {
                        errorStatus = -1;
                        printf("[!!] There were errors in '%s' script\n", infilename.c_str());
                        }
                    }
                }
            closedir(dp);
            printf("***********************************************************************\n");
            }
        }
    //
    chdir(startWorkingDir);
    return errorStatus;
    }

// -----------------------------------------------------------------------------

bool EOSKernel::safeguardCheck()
    {
    if (this->safeguardRegistry & EOSKernel::SAFEGUARD_ACTIVE)
        {
        if (this->safeguardRegistry & EOSKernel::SAFEGUARD_FLAG_LOADMODULE)
            {
            logMessage(LOGLEVEL_ERR, "[eos.kernel] - Warning: SafeGuard function has detected an error during a module loading. EOS Kernel will now stopped.\n");
            return true;
            }
        }
    return false;
    }

// -----------------------------------------------------------------------------

void EOSKernel::update()
    {
    //
    // stops execution if safeguard mechanism has detected a fatal error
    if (safeguardCheck())
        {
        masterProcess->setActive(false);
        return;
        }
    //
    if (this->socketHandler != NULL)
        {
        //this->socketHandler->listen();
        int dataLength = 0;
        char* data = this->socketHandler->listen(&dataLength);
        if (data != NULL && dataLength > 0)
            {
            //printf("SocketHandler - data read: %s\n", data);
            if (this->remoteCommandParser != NULL)
                {
                const char* commandCode = this->remoteCommandParser->parseCommand(data);
                if (commandCode != NULL)
                    {
                    this->luaEnvironment->runBuffer((const unsigned char*) commandCode, strlen(commandCode), "eos:RemoteCommand");
                    this->socketHandler->answer("message received");
                    }

                }
            }
        }
    //
    unsigned long now = kernelTimer->getMilliseconds();
    lastFrameTime = now - frameStart;
    frameStart = now;
    if (lastFrameTime < this->targetFrameTime)
        usleep((long) ((this->targetFrameTime - lastFrameTime) * 1000));
    //
    // message queue dispatching
    EOSMessage* message = NULL;
    while (!messageQueue.empty())
        {
        message = messageQueue.front();
        messageQueue.pop();
        // message processing:
        if (message->getReceiver() == "") // no receiver: default processed by masterProcess
            {
            masterProcess->processMessage(message);
            }
        else
            {
            EOSProcess* process = processByName(message->getReceiver().c_str());
            if (process != NULL && process->isActive())
                {
                //printf("# Sending message to: %s\n", message->getReceiver().c_str());
                process->processMessage(message);
                }
            }
        // message object destruction:
        delete message;
        }
    //
    // user processes update:
    std::map<std::string, EOSProcess*>::iterator it;
    for (it = processes.begin(); it != processes.end(); /* empty */)
        {
        EOSProcess* process = it->second;
        if (process != NULL)
            {
            if (process->isGarbaged()) // removes garbaged processes
                {
                logMessage(LOGLEVEL_INFO, "[eos.kernel] - Process '%s' deleted.\n", it->first.c_str());
                delete process;
                processes.erase(it++); // must use post-increment here
                }
            else
                {
                this->currentProcessName = process->getName();
                if (process->isActive())
                    process->update();
                //
                ++it;
                }
            }
        }
    //
    //
    // master process update:
    this->currentProcessName = masterProcess->getName();
    if (masterProcess->selectScriptFunction("update"))
        masterProcess->go(); // executes 'update' function
    else
        {
        logMessage(LOGLEVEL_ERR, "[eos.kernel.update()] - FATAL: unable to find/execute 'update' function of MasterProcess.\n");
        masterProcess->setActive(false);
        }
    //
    }

// -----------------------------------------------------------------------------

void EOSKernel::run()
    {
    logMessage(LOGLEVEL_INFO, "--- [eos.kernel.run()] - Running...\n");
    while (masterProcess->isActive())
        {
        this->update();
        }
    logMessage(LOGLEVEL_INFO, "--- [eos.kernel.run()] - End Running.\n");
    }

// -----------------------------------------------------------------------------

void EOSKernel::cleanup()
    {
    logMessage(LOGLEVEL_INFO, "[eos.cleanup()] - Cleaning up...\n");
    //
    // deletes all processes
    std::map<std::string, EOSProcess*>::iterator it;
    for (it = processes.begin(); it != processes.end(); it++)
        delete it->second;
    //
    delete masterProcess;
    //
    // calls cleanup on all loaded modules
    char cmdString[120];
    sprintf(cmdString, "for key,value in pairs(eos.module.list) do loadstring(value.module.reservedFunctions.cleanup..'()')() end");
    this->luaEnvironment->runBuffer((const unsigned char*) cmdString, strlen(cmdString));
    //
    if (this->kernelTimer != NULL)
        delete this->kernelTimer;
    //
    if (this->luaEnvironment != NULL)
        delete this->luaEnvironment;
    //
    logMessage(LOGLEVEL_INFO, "[eos.cleanup()] - Cleanup complete.\n");
    //
    if (this->logStream.is_open())
        {
        logStream << "Application closed." << std::endl;
        logStream.close();
        }
    //
    this->removeNamedFifos();
    //
    }

// -----------------------------------------------------------------------------

bool EOSKernel::createProcess(const char* processName, const char* scriptName)
    {
    if (processByName(processName) == NULL)
        {
        EOSProcess* newProcess = new EOSProcess(this->luaEnvironment, this->scriptManager, processName, scriptName);
        processes.insert(std::pair<std::string, EOSProcess*>(processName, newProcess));
        logMessage(LOGLEVEL_INFO, "[eos.process.create()] - Process '%s' created from script '%s'.\n", processName, scriptName);
        return true;
        }
    else
        {
        logMessage(LOGLEVEL_WARN, "[eos.process.create()] - Warning: a process named '%s' already exists. Ignoring.\n", processName);
        }
    return false;
    }

// -----------------------------------------------------------------------------

EOSProcess* EOSKernel::processByName(const char* processName)
    {
    EOSProcess* process = NULL;
    if (processName != NULL)
        {
        std::map<std::string, EOSProcess*>::iterator it = processes.find(processName);
        if (it != processes.end())
            process = (*it).second;
        }
    return process;
    }

// -----------------------------------------------------------------------------

bool EOSKernel::setProcessActivity(const char* processName, bool activity)
    {
    EOSProcess* process = processByName(processName);
    if (process != NULL)
        {
        process->setActive(activity);
        return true;
        }
    return false;
    }

// -----------------------------------------------------------------------------

bool EOSKernel::destroyProcess(const char* processName)
    {
    if (processName != NULL)
        {
        std::map<std::string, EOSProcess*>::iterator it = processes.find(processName);
        if (it != processes.end())
            {
            EOSProcess* process = (*it).second;
            process->garbage();
            // note: the process is put in garbaged state, and is removed in next update cycle
            return true;
            }
        }
    return false;
    }

// -----------------------------------------------------------------------------

void EOSKernel::_setFrameRate(const char* requestedFrameRate)
    {
    int targetFrameRate = 1000;
    if (requestedFrameRate != NULL)
        {
        targetFrameRate = atoi(requestedFrameRate);
        if (targetFrameRate <= 0) targetFrameRate = 1000;
        }
    this->targetFrameTime = (unsigned long) (1000 / targetFrameRate);
    logMessage(LOGLEVEL_INFO, "[eos.kernel] - Target kernel framerate set to %d iterations/sec\n", targetFrameRate);
    }

// -----------------------------------------------------------------------------

float EOSKernel::currentTime()
    {
    return kernelTimer->getSeconds();
    }

// -----------------------------------------------------------------------------

unsigned long EOSKernel::currentTimeMillis()
    {
    return kernelTimer->getMilliseconds();
    }

// -----------------------------------------------------------------------------

void EOSKernel::logMessage(int level, const char* format_string, ...)
    {
    if (!this->logStream.is_open() || format_string == NULL)
        return;
    //
    char timestr[32] = "";
    char lvlStr[4] = "";
    //
    if (level != LOGLEVEL_NOHEADER)
        {
        time_t now;
        struct tm* currenttime;
        time(&now);
        currenttime = localtime(&now);
        sprintf(timestr, "%02d:%02d:%02d", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
        sprintf(lvlStr, "[ ]");
        //
        unsigned int messageLevel = 1;
        switch (level)
            {
            case LOGLEVEL_ERR:
                lvlStr[1] = 'E';
                messageLevel = 1;
                break;
            case LOGLEVEL_WARN:
                lvlStr[1] = 'W';
                messageLevel = 2;
                break;
            case LOGLEVEL_DBUG:
                lvlStr[1] = 'D';
                messageLevel = 3;
                break;
            case LOGLEVEL_USER:
                lvlStr[1] = 'U';
                messageLevel = 4;
                break;
            case LOGLEVEL_INFO:
                lvlStr[1] = 'I';
                messageLevel = 5;
                break;
            }
        }
    //
    char formatStringBuffer[256];
    va_list vlist;
    va_start(vlist, format_string);
    vsnprintf(formatStringBuffer, 255, format_string, vlist);
    va_end(vlist);
    //
    // writes line on the logstream
    if (level != LOGLEVEL_NOHEADER)
        logStream << "[" << timestr << "]" << lvlStr << " " << formatStringBuffer;
    else
        logStream << formatStringBuffer;
    logStream.flush();
    //
    // writes line on the console
    if (level <= this->consoleVerbosityLevel)
        if (level != LOGLEVEL_NOHEADER)
            fprintf(stderr, ">> %s - %s\n", lvlStr, formatStringBuffer);
        else
            fprintf(stderr, ">> %s\n", formatStringBuffer);
    }

// -----------------------------------------------------------------------------

void EOSKernel::prepareMessage(const char* messageClass)
    {
    if (messageClass != NULL)
        {
        if (this->temporaryMessage != NULL)
            delete temporaryMessage;
        this->temporaryMessage = new EOSMessage(messageClass);
        //this->temporaryMessage->setSender(this->luaEnvironment->getLuaValueAsString("this.processName"));
        this->temporaryMessage->setSender(this->currentProcessName);
        }
    }

// -----------------------------------------------------------------------------

void EOSKernel::pushMessageData(const char* messageDataKey, bool messageDataValue)
    {
    if (this->temporaryMessage != NULL && messageDataKey != NULL)
        this->temporaryMessage->addValue(messageDataKey, messageDataValue);
    }

// -----------------------------------------------------------------------------

void EOSKernel::pushMessageData(const char* messageDataKey, float messageDataValue)
    {
    if (this->temporaryMessage != NULL && messageDataKey != NULL)
        this->temporaryMessage->addValue(messageDataKey, messageDataValue);
    }

// -----------------------------------------------------------------------------

void EOSKernel::pushMessageData(const char* messageDataKey, const char* messageDataValue)
    {
    if (this->temporaryMessage != NULL && messageDataKey != NULL && messageDataValue != NULL)
        this->temporaryMessage->addValue(messageDataKey, messageDataValue);
    }

// -----------------------------------------------------------------------------

void EOSKernel::sendPreparedMessage(const char* messageReceiver)
    {
    if (this->temporaryMessage != NULL && messageReceiver != NULL)
        {
        this->temporaryMessage->setReceiver(messageReceiver);
        this->messageQueue.push(this->temporaryMessage);
        this->temporaryMessage = NULL;
        }
    }

// -----------------------------------------------------------------------------

void EOSKernel::registerNativeLuaFunctions()
    {
    char luaCode[1024];
    //
    sprintf(luaCode, "function serializeItem(i,ofile)\n if type(i) == \"number\" then\n ofile:write(i)\n elseif type(i) == \"string\" then\n ofile:write(string.format(\"%%q\", i)) \n elseif type(i) == \"table\" then \n ofile:write(\"{\\n\") \n for k,v in pairs(i) do \n ofile:write(\"\\t[\") serializeItem(k, ofile) ofile:write(\"] = \") \n serializeItem(v, ofile) \n ofile:write(\",\\n\") \n end \n ofile:write(\"}\\n\") \n else \n error(\"serializeItem: cannot serialize a \" .. type(i)) \n end \n end");
    this->luaEnvironment->runBuffer((const unsigned char*) luaCode, strlen(luaCode), "eos.kernel::registerNativeLuaFunctions()");
    }

// -----------------------------------------------------------------------------

/* Changelog:

 **EOS** kernel:
v. 0.9.1
        Nov 05, 2009
           .fixed loading "sender" message name in message initialization
           .added a char* pointer to the "current executing process" in the kernel
           .added class EOSRemoteCommandParser, to manage and filter incoming remote commands
           .added class UDPSocketHandler, to manage a simple UDP non blocking socket read
           .added '--enable-debug-socket' command line option, to enable remote debug control socket (default on port 2009)
        Nov 03, 2009
           .added functions to create and remove named FIFos used as control streams
           .added '--enable-ctrl-streams' command line option to enable input and output named FIFOs (used for debugging and remote control)
           .added '--norun' command line option to make kernel just compile scripts without running them
      .implemented decent but minimalistic command line option parsing
v. 0.9.0
        Mar 25, 2009
      .removed mandatory settings file loading
        Mar 19, 2009
      .cleaned ScriptManager code
      .removed legacy ELF cryptographic system for scripts from ScriptManager
        Mar 17, 2009
      .kernel is not a shared object anymore. Made it the main application (including EOSLauncher.cpp in project)
        Jan 14, 2009
                .now kernel logs in 'eos.log' instead of 'EOS.log'
                .added module cleanup function calling in kernel cleanup function: now kernel can automatically call cleanup code for each loaded module
                .adapted kernel moduleLoad function to new module data structure
                .added new table eos.module.list in lua environment to keep track of all loaded modules
                .created new module data structure (module data now is stored in a lua table: struct EOSModuleInfo is now deprecated)
                .replaced a lot of "EOSKernel" string occurrencies with "eos.kernel" in output and message logging
v. 0.8.0
        Sep 11, 2008
                .simplified function naming: removed "kernel" table (now, functions previously stored under eos.kernel.time are now more simply accessible via eos.time hierarchy)
        Sep  3, 2008
                .first EOS Kernel release (based on ELF Kernel v. 2.0.0)
                .removed and replaced all old "ELF" references in code
        Mar 28, 2008
                .subdivided all kernel functions in "group of functions" (elf.kernel.log, elf.kernel.module, elf.kernel.time, etc.)
                .adopted new code semantic (no more "ELFKernel.xxYyyy", but something like "elf.kernel.time")


=======================================================================

 **ELF** kernel:
v. 2.0.0
        Apr 17, 2007:
            .improved logMessage function (now accepts printf-style variable arguments)
        Apr 2, 2007:
            .new hierarchical naming convention for functions
        Mar 25, 2007:
            .started new code rewriting

v. 1.0.5
        Sep 20, 2007:
                .added "norun" parameter to launch ELFKernel without running application (only compiles .els script)
        Sep 07, 2007:
                .added double quotes in luac arguments (now works on paths containing spaces)
        Jun 07, 2007:
                .added better debugging support to scripts compile process (better output in console)
                .integrated BlackCoder code in ELFKernel (in the makeScripts() private method)
                .fixed ELFKernel.loadmodule() error messages
                .added _VERSION_NUMBER, _VERSION_NAME, _VERSION_DATE defines in kernel id log message
        Feb 26, 2007:
                .fixed a dangerous bug in process lua table deallocation (was not using luaL_unref)
        Feb 24, 2007:
                .fixed temporary script buffer deallocation (ELFScriptManager.cpp)
v. 1.0.4
        Jan 12, 2007:
                .added native lua function serializeItem(i,ofile)
v. 1.0.3.2
        Oct 25, 2006:
                .added setConsoleLogLevel function
                .added traceDebug function
                .added code to output log lines also to console (stderr), and added verbosity level switch
v. 1.0.3
        Sep 13, 2006:
                .added messagePushLuaCode function, and message code injection facility (useful to create multi-level structured messages)
        Sep 9, 2006:
                .fixed a bug in processes update and garbage system
v. 1.0.2
        Aug 24, 2006:
                .Modified Process destruction/garbaging system

v. 1.0.1

 */
