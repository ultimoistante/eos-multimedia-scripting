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

#include "ImageManager.h"

extern "C"
	{
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"

	#include "EOSLuaRuntime.h"


	/*- @module eos.freeimage
	 *  @brief FreeImage-based Multiple Image format I/O Module for EOS
	 */
	// -----------------------------------------------------------------------------
	// -----------------------------------------------------------------------------
	//
	// Module manifest structure. Developer must edit just the #define values below.
	//
	#define _MODULE_NAME            "image"
	#define _MODULE_VERSION         "0.1"
	#define _MODULE_DATE            "September 9, 2009"
	#define _MODULE_DESC_SHORT      "Multiple Image Format I/O utility  module for EOS Scripting Environment"
	#define _MODULE_DESC_LONG       "This software uses the FreeImage open source image library. See http://freeimage.sourceforge.net for details."
	#define _MODULE_AUTHOR_NAME     "EOS Scripting Team"
	#define _MODULE_AUTHOR_WEBSITE  "http://sourceforge.net/projects/eos-scripting/"
	//
	// -----------------------------------------------------------------------------
	// -----------------------------------------------------------------------------


	// -----------------------------------------------------------------------------

	ImageManager imageManager;


	// utility functions -----------------------------------------------------
	// end utility functions -------------------------------------------------


	// ====================== COMMON REQUIRED FUNCTIONS ============================
	// -----------------------------------------------------------------------------

	/*- @function eos.freeimage.module.cleanup
	 *  @brief automatically called from eos kernel on module unloading
	 */
	static int moduleCleanup(lua_State* luaRuntime)
		{
		imageManager.clearAll();
		//
		return 0;
		}

	// ====================== CUSTOM MODULE FUNCTIONS ==============================
	// -----------------------------------------------------------------------------

	/*- @function eos.freeimage.load
	 *  @brief loads an image from file, and returns an EOSbject cointaining the pointer to the decoded image pixel buffer
	 *  @param imageFileName filename of the image to load
	 *  @return loadedImage an EOSbject cointaining the pointer to the decoded image pixel buffer
	 */
	static int loadImage(lua_State* luaRuntime)
		{
		void* userdata = NULL;
		//
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME"loadImage", 1))
			{
			Image* image = imageManager.loadImage(luaL_checkstring(luaRuntime, 1));
			if(image != NULL)
				userdata = imageManager.getSessionObject(image);
			}
		//
		lua_pushlightuserdata(luaRuntime, userdata);
		return 1;
		}

	// -----------------------------------------------------------------------------

	/*static int resizeImage(lua_State* luaRuntime)
		{
		if(EOSLuaRuntime::checkLuaParameters("eos.freeimage", "resizeImage", 3))
			{
			Image* image = imageManager.imageById((unsigned int)lua_tonumber(luaRuntime, 1));
			if(image)
				image->resize((unsigned int)lua_tonumber(luaRuntime, 2), (unsigned int)lua_tonumber(luaRuntime, 3));
			}
		//
		return 0;
		}*/

	// -----------------------------------------------------------------------------

	/*static int imageExportAsDataSource(lua_State* luaRuntime)
		{
		bool done = false;
		//
		if(EOSLuaRuntime::checkLuaParameters("eos.freeimage", "imageExportAsDataSource", 2))
			{
			Image* image = imageManager.imageById((unsigned int)lua_tonumber(luaRuntime, 1));

			if(image != NULL)
				{
				const char* dataSourceName = luaL_checkstring(luaRuntime, 2);
				if(dataSourceName != NULL)
					{
					image->fillExportBuffer();
					EOSLuaRuntime::getInstance()->registerDataSource(dataSourceName, "RGBA_IMAGE_BUFFER", (unsigned long)image->getExportBufferStruct(), sizeof(struct eos_datasource_imagebuffer));
					done = true;
					}
				else
					EOSLuaRuntime::getInstance()->logWarningMessage("[eos.freeimage][image.exportAsDataSource] - error: parameter 'dataSourceName' not valid!");
				}
			else
				EOSLuaRuntime::getInstance()->logWarningMessage("[eos.freeimage][image.exportAsDataSource] - error: image with id '%d' not found!", lua_tonumber(luaRuntime, 1));
			}
		//
		lua_pushboolean(luaRuntime, done);
		return 1;
		}*/


	// ======================== LUA FUNCTIONS REGISTRATION =========================
	// -----------------------------------------------------------------------------

	// ==== Here we declare an array with all functions and their respective names.
	// ==== This array has elements of type luaL_reg, which is a structure with
	// ==== two fields: a string and a function pointer.

	const struct luaL_reg fn_table_module [] = {
		{"cleanup", moduleCleanup},
		{NULL, NULL} // <-- end-of-table sentinel
		};

	const struct luaL_reg fn_table_main [] = {
		{"load", loadImage},
		//{"resize",              resizeImage},
		//{"exportAsDataSource",  imageExportAsDataSource},
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

		// registers main functions
		luaL_register(luaRuntime, "eos."_MODULE_NAME, fn_table_main);

		/*
		// registers "eos.*.module" functions
		EOSLuaRuntime::getInstance()->executeLuaChunk("eos.freeimage.module = {}");
		luaL_register(luaRuntime, "eos.freeimage.module", fn_table_module);
		// registers main functions
		luaL_register(luaRuntime, "eos.freeimage", fn_table_main);
		*/

		//
		return 1;
		}

	// -----------------------------------------------------------------------------

	} // extern "C" closure

