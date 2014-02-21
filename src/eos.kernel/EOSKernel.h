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

#ifndef EOS_KERNEL_KERNEL_H
#define EOS_KERNEL_KERNEL_H

#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <queue>
#include <string>
#include <dirent.h>
#include <sys/stat.h>   // For stat().
//#include <linux/stat.h>
#include <errno.h>


#include "EOSLuaEnvironment.h"
#include "EOSScriptManager.h"
#include "EOSPrecisionTimer.h"
#include "EOSProcess.h"
#include "EOSMessage.h"
#include "UDPSocketHandler.h"
#include "EOSRemoteCommandParser.h"

#define  LOGLEVEL_NONE			0
#define  LOGLEVEL_ERR    		1
#define  LOGLEVEL_WARN			2
#define  LOGLEVEL_DBUG			3
#define  LOGLEVEL_USER			4
#define  LOGLEVEL_INFO			5
#define  LOGLEVEL_NOHEADER      9

#define  CONTROL_FIFO_INPUT   ".eos-input"
#define  CONTROL_FIFO_OUTPUT  ".eos-output"
#define  DEBUG_SOCKET_PORT    2009

class EOSRemoteCommandParser;

/** \mainpage
 * EOS Kernel is the main software component of the EOS Framework.
 */

/** @class EOSKernel
 * Main class of the EOS Kernel module
 */
class EOSKernel
    {
    public:
        /// returns the static instance of the class
        static EOSKernel* getInstance();
        /// destructor
        ~EOSKernel() { instanceFlag = false; }
        //
        /// starts the kernel
        //void start(const char* appPath, bool norun) { if(init(appPath, norun) == 0) run(); cleanup(); }
        void start() { if(init() == 0) run(); cleanup(); }
        /// stops the kernel
        void stop();
        //
        /// returns a pointer to the current EOSLuaEnvironment wrapper class
        EOSLuaEnvironment* getLuaEnvironment() { return this->luaEnvironment; }
        //
        void setApplicationPath(const char* appPath) { if(appPath != NULL) this->applicationPath = appPath; }
        void enableControlStreams()                  { this->controlStreamsEnabled = true; }
        void enableDebugSocket()                     { this->debugSocketEnabled = true; }
        void enableJustCompileScriptsMode()          { this->justCompileScripts = true; }
        //
		void setExecParamsCount(int *argc) { this->execParamsCount = argc; }
		int*  getExecParamsCount() { return this->execParamsCount; }
		void setExecParamsValues(char** argv) { this->execParamsValues = argv; }
		char** getExecParamsValues() { return this->execParamsValues; }
        int        consoleVerbosityLevel; ///< verbosity level for the console (stderr) output
        //
        static int SAFEGUARD_ACTIVE;          ///< safeguard mechanism: activity flag
        static int SAFEGUARD_FLAG_LOADMODULE; ///< safeguard mechanism: module load check
        //
        // ==== LUA INTERFACE
        static const struct luaL_reg lua_eoskernel_main        []; ///< contains top level lua binding functions (eos.*)
        static const struct luaL_reg lua_eoskernel_module      []; ///< contains module handling lua binding functions (eos.module.*)
        static const struct luaL_reg lua_eoskernel_time        []; ///< contains time related lua binding functions (eos.time.*)
        static const struct luaL_reg lua_eoskernel_process     []; ///< contains process handling lua binding functions (eos.process.*)
        static const struct luaL_reg lua_eoskernel_message     []; ///< contains message handling lua binding functions (eos.message.*)
        static const struct luaL_reg lua_eoskernel_log         []; ///< contains log handling lua binding functions (eos.log.*)
        //
        /*- \module luabinding
         * eos.module.load()
         */
        static int moduleLoad(lua_State* luaRuntime);
        static int shutdown(lua_State* luaRuntime);
        static int sleep(lua_State* luaRuntime);
        static int getCurrentTime(lua_State* luaRuntime);
        static int getCurrentTimeMillis(lua_State* luaRuntime);
        static int setTargetFrameRate(lua_State* luaRuntime);
        static int getTargetFrameRate(lua_State* luaRuntime);
        static int getLastFrameTime(lua_State* luaRuntime);
        static int getApplicationRoot(lua_State* luaRuntime);
        static int processCreate(lua_State* luaRuntime);
        static int processExists(lua_State* luaRuntime);
        static int processSetActive(lua_State* luaRuntime);
        static int processIsActive(lua_State* luaRuntime);
        static int processDestroy(lua_State* luaRuntime);
        static int messageInit(lua_State* luaRuntime);
        static int messagePushData(lua_State* luaRuntime);
        static int messageSend(lua_State* luaRuntime);
        static int setConsoleLogLevel(lua_State* luaRuntime);
        static int traceError(lua_State* luaRuntime);
        static int traceWarning(lua_State* luaRuntime);
        static int traceDebug(lua_State* luaRuntime);
        static int traceUser(lua_State* luaRuntime);
        static int traceInfo(lua_State* luaRuntime);
        //
        //
    private:
        /// private constructor
        EOSKernel();
        /// static instance flag
        static bool instanceFlag;
        /// static instance
        static EOSKernel* instance;
        //
        /// runs early initialization code of the kernel
        int  init();
        /// function used to create and remove control FIFOs
        void removeNamedFifos();
        void createNamedFifos();
        /// compiles given application source scripts
        int  makeScripts(const char* scriptsPath);
        /// safeguard mechanism: exception check
        bool safeguardCheck();
        /// executes an update cycle of the kernel (formerly a "frame")
        void update();
        /// starts kernel execution cycle
        void run();
        /// pauses kernel for a given number of milliseconds
        void pause(unsigned int milliseconds) { usleep(milliseconds * 1000); }
        /// kernel resources cleanup. This function also calls cleanup on all loaded extension modules.
        void cleanup();
        //
        /// creates an EOS Process
        bool          createProcess(const char* processName, const char* scriptName);
        /// returns the pointer to an EOS Process, by its name
        EOSProcess*   processByName(const char* processName);
        /// sets the activity status of an EOS Process
        bool          setProcessActivity(const char* processName, bool activity);
        /// destroys an EOS Process
        bool          destroyProcess(const char* processName);
        //
        /// sets the target execution frame rate of the kernel
        void          _setFrameRate(const char* requestedFrameRate);
        //
        /// returns elapsed time since kernel start, in seconds
        float         currentTime();
        /// returns elapsed time since kernel start, in milliseconds
        unsigned long currentTimeMillis();
        //
        /// writes a line in the application log file
        void logMessage(int level, const char* format_string, ...);
        //
        /// prepares (initializes) a new EOSMessage structure
        void prepareMessage(const char* messageClass);
        /// pushes a boolean data field in the current EOSMessage structure
        void pushMessageData(const char* messageDataKey, bool messageDataValue);
        /// pushes a numeric data field in the current EOSMessage structure
        void pushMessageData(const char* messageDataKey, float messageDataValue);
        /// pushes a string data field in the current EOSMessage structure
        void pushMessageData(const char* messageDataKey, const char* messageDataValue);
        /// sends current EOSMessage structure to the receiver Process
        void sendPreparedMessage(const char* messageReceiver);
        //
        void registerNativeLuaFunctions();
        //
        static std::ofstream    logStream;        ///< application's output log stream
        //
        EOSPrecisionTimer*      kernelTimer;      ///< precision timer to handle execution timings
        //
        EOSLuaEnvironment*      luaEnvironment;   ///< instance of the class that manages Lua Runtime Environment
        //
        EOSScriptManager*       scriptManager;    ///< instance of the class that manages source scripts
        //
        EOSProcess*             masterProcess;    ///< pointer to the application's main process (MasterProcess is the only process automatically instanced by the kernel)
        //
        EOSMessage*             temporaryMessage; ///< pointer to the temporary EOSMessage structure
        std::queue<EOSMessage*> messageQueue;     ///< message queue
        //
        const char*             currentProcessName; ///< pointer to the name of current executing process
        //
        unsigned long           frameStart, lastFrameTime, targetFrameTime; ///< fields used for timing purposes
        //
        std::map<std::string, EOSProcess*> processes;  ///< hashmap containing EOSProcess* instances
        //
        unsigned int            safeguardRegistry;     ///< safeguard mechanism: flag registry
        //
        std::string             applicationPath;       ///< current application path
        //
        bool                    justCompileScripts;    ///< set to 'true' when in --norun mode (just compile scripts and exits)
        //
        bool                    controlStreamsEnabled; ///< set to 'true' when kernel launched with '--enable-ctrl-streams' command line option
        //
        bool                    debugSocketEnabled;    ///< set to 'true' when kernel launched with '--enable-debug-socket' command line option
        //
        UDPSocketHandler*       socketHandler;
        EOSRemoteCommandParser* remoteCommandParser;
        //
        int* execParamsCount;
        char** execParamsValues;
    };



#endif

