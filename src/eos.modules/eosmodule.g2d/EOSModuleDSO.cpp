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

#include "G2D.h"
#include "EOSLuaRuntime.h"
#include "eos_data_containers.h"

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
	#define _MODULE_NAME            "g2d"
	#define _MODULE_VERSION         "0.2.1"
	#define _MODULE_DATE		    "November 05, 2012"
	#define _MODULE_DESC_SHORT      "2D OpenGL-powered sprites module for EOS Scripting Environment"
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
		//
		return 0;
		}

	// ====================== CUSTOM MODULE FUNCTIONS ==============================

	/*- @function eos.g2d.update
	 *  @brief updates picasso module and active render targets
	 *  @return updated (boolean) true or false
	 */
	static int moduleUpdate(lua_State* luaRuntime)
		{
		bool done = true;
		//if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".update", 1))
		G2D::getInstance()->update();
		//
		lua_pushboolean(luaRuntime, done);
		return 1;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.renderTarget.create()
	 * @brief create a new render target for sprites
	 * @param renderTargetName name of the render target
	 * @param width width in pixels
	 * @param height height in pixels
	 * @returns session object
	 */
	static int renderTargetCreate(lua_State* luaRuntime)
		{
		void* userdata = NULL;
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".renderTarget.create", 3))
			{
			if(G2D::getInstance()->createRenderTarget(lua_tostring(luaRuntime, 1), (uint) lua_tonumber(luaRuntime, 2), (uint) lua_tonumber(luaRuntime, 3)))
				userdata = G2D::getInstance()->createSessionObject(lua_tostring(luaRuntime, 1));
			}
		//    
		lua_pushlightuserdata(luaRuntime, userdata);
		return 1;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.renderTarget.setCurrent()
	 * @brief sets the current render target for sprites creation
	 * @param renderTargetName name of the render target
	 * @returns nothing
	 */
	static int renderTargetSetCurrent(lua_State* luaRuntime)
		{
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".renderTarget.setCurrent", 1))
			G2D::getInstance()->setCurrentRenderTarget(lua_tostring(luaRuntime, 1));
		//
		return 0;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.element.setProperty()
	 * @brief sets an element property
	 * @param element name
	 * @param property name
	 * @param property value
	 * @returns true or false
	 */
	static int elementSetProperty(lua_State* luaRuntime)
		{
		bool done = false;
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".element.setProperty", 3))
			{
			G2DElement* e = G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			if(e != NULL)
				{
				e->setProperty(lua_tostring(luaRuntime, 2), lua_tostring(luaRuntime, 3));
				done = true;
				}
			}
		//
		lua_pushboolean(luaRuntime, done);
		return 1;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.element.getProperty()
	 * @brief gets an element property
	 * @param element name
	 * @param property name
	 * @returns property value
	 */
	static int elementGetProperty(lua_State* luaRuntime)
		{
		char *value = NULL;
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".element.getProperty", 2))
			{
			G2DElement* e = G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			if(e != NULL)
				value = e->getProperty(lua_tostring(luaRuntime, 2));
			}
		//
		lua_pushstring(luaRuntime, value);
		return 1;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.element.setZOrder()
	 * @brief sets an element z-order
	 * @param element name
	 * @param value
	 * @returns true or false
	 */
	static int elementSetZOrder(lua_State* luaRuntime)
		{
		bool done = false;
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".element.setZOrder", 2))
			{
			G2DElement* e = G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			if(e != NULL)
				{
				e->setZOrder((int) lua_tonumber(luaRuntime, 2));
				done = true;
				}
			}
		//
		lua_pushboolean(luaRuntime, done);
		return 1;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.element.getZOrder()
	 * @brief gets an element z-order
	 * @param element name
	 * @returns z-order value
	 */
	static int elementGetZOrder(lua_State* luaRuntime)
		{
		int zorder = 0;
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".element.getZOrder", 1))
			{
			G2DElement* e = G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			if(e != NULL)
				zorder = e->getZOrder();
			}
		//
		lua_pushnumber(luaRuntime, zorder);
		return 1;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.sprite.create()
	 * @brief create a sprite
	 * @param sprite name
	 * @returns true or false
	 */
	static int spriteCreate(lua_State* luaRuntime)
		{
		bool done = false;
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sprite.create", 1))
			done = G2D::getInstance()->createSprite(lua_tostring(luaRuntime, 1));
		//
		lua_pushboolean(luaRuntime, done);
		return 1;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.sprite.addAnim()
	 * @brief adds an animation to a sprite
	 * @param sprite name
	 * @param anim name
	 * @param image sessionObject
	 * @param number of frames
	 * @param frame width
	 * @param frame height
	 * @param time delay in milliseconds between frames
	 * @param frame to start animation loop (0 = all animation)
	 * @returns true or false
	 */
	static int spriteAddAnim(lua_State* luaRuntime)
		{
		bool done = false;
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sprite.addAnim", 7))
			{
			G2DSprite* spr = (G2DSprite*) G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			EOSObject *so = (EOSObject*) lua_topointer(luaRuntime, 3);
			if(spr != NULL && so != NULL)
				done = spr->addAnim(lua_tostring(luaRuntime, 2), (eos_imagedata*) so->getUserData(), (uint) lua_tonumber(luaRuntime, 4), (uint) lua_tonumber(luaRuntime, 5), (uint) lua_tonumber(luaRuntime, 6), lua_tonumber(luaRuntime, 7), (uint) lua_tonumber(luaRuntime, 8));
			}
		//
		lua_pushboolean(luaRuntime, done);
		return 1;
		}

	// -----------------------------------------------------------------------------

		/*-
	 * @name function eos.g2d.sprite.setFixedFrame()
	 * @brief adds a single frame animation to a sprite
	 * @param sprite name
	 * @param image sessionObject
	 */
	static int spriteSetFixedFrame(lua_State* luaRuntime)
		{
		bool done = false;
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sprite.setFixedFrame", 2))
			{
			G2DSprite* spr = (G2DSprite*) G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			EOSObject *so = (EOSObject*) lua_topointer(luaRuntime, 2);
			if(spr != NULL && so != NULL)
				{
				done = spr->setFixedFrame((eos_imagedata*) so->getUserData());
				}
			}
		//
		lua_pushboolean(luaRuntime, done);
		return 1;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.sprite.animGoToFrame()
	 * @brief force the current animation to go to the n frame
	 * @param sprite name
	 * @param frame number
	 * @returns true or false
	 */
	static int spriteAnimGoToFrame(lua_State* luaRuntime)
		{
		bool done = false;
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sprite.animGoToFrame", 2))
			{
			G2DSprite* spr = (G2DSprite*) G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			if(spr)
				done = spr->animGoToFrame((uint) lua_tonumber(luaRuntime, 2));
			}
		//
		lua_pushboolean(luaRuntime, done);
		return 1;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.sprite.setCurrentAnim()
	 * @brief sets the current animation
	 * @param sprite name
	 * @param anim name
	 * @returns true or false
	 */
	static int spriteSetCurrentAnim(lua_State* luaRuntime)
		{
		bool done = false;
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sprite.setCurrentAnim", 2))
			{
			G2DSprite* spr = (G2DSprite*) G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			if(spr)
				done = spr->setCurrentAnim(lua_tostring(luaRuntime, 2));
			}
		//
		lua_pushboolean(luaRuntime, done);
		return 1;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.sprite.getCurrentAnim()
	 * @brief gets the current animation
	 * @param sprite name
	 * @returns current anim name
	 */
	static int spriteGetCurrentAnim(lua_State* luaRuntime)
		{
		char *name = NULL;
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sprite.getCurrentAnim", 1))
			{
			G2DSprite* spr = (G2DSprite*) G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			if(spr)
				name = spr->getCurrentAnim();
			}
		//
		if(name)
			lua_pushstring(luaRuntime, name);
		else
			lua_pushstring(luaRuntime, "");
		return 1;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.sprite.setHotSpot()
	 * @brief sets a sprite's hotspot position
	 * @param sprite name
	 * @param hotspot position
	 */
	static int spriteSetHotSpot(lua_State* luaRuntime)
		{
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sprite.setHotSpot", 2))
			{
			G2DSprite* spr = (G2DSprite*) G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			if(spr)
				spr->setHotSpot((uint) lua_tonumber(luaRuntime, 2));
			}
		//
		return 0;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.sprite.setPosition()
	 * @brief sets a sprite's position
	 * @param sprite name
	 * @param x
	 * @param y
	 */
	static int spriteSetPosition(lua_State* luaRuntime)
		{
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sprite.setPosition", 3))
			{
			G2DSprite* spr = (G2DSprite*) G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			if(spr)
				spr->setPosition((uint) lua_tonumber(luaRuntime, 2), (uint) lua_tonumber(luaRuntime, 3));
			}
		//
		return 0;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.sprite.setRotation()
	 * @brief sets sprite rotation angle
	 * @param sprite name
	 * @param angle
	 */
	static int spriteSetRotation(lua_State* luaRuntime)
		{
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sprite.setRotation", 2))
			{
			G2DSprite* spr = (G2DSprite*) G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			if(spr)
				spr->setRotation(lua_tonumber(luaRuntime, 2));
			}
		//
		return 0;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.sprite.bindToObject()
	 * @brief binds the sprite to an object
	 * @param sprite name
	 * @param session object
	 */
	static int spriteBindToObject(lua_State* luaRuntime)
		{
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sprite.bindToObject", 2))
			{
			G2DSprite* spr = (G2DSprite*) G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			if(spr)
				spr->bindToObject((EOSObject*) lua_topointer(luaRuntime, 2));
			}
		//
		return 0;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.sprite.getPosition()
	 * @brief gets sprite's position
	 * @param sprite name
	 * @returns x
	 * @returns y
	 */
	static int spriteGetPosition(lua_State* luaRuntime)
		{
		Vector2f pos(0, 0);
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sprite.getPosition", 1))
			{
			G2DSprite* spr = (G2DSprite*) G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			if(spr)
				pos = spr->getPosition();
			}
		//
		lua_pushnumber(luaRuntime, pos.x());
		lua_pushnumber(luaRuntime, pos.y());
		return 2;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.sprite.getRotation()
	 * @brief gets sprite rotation angle
	 * @param sprite name
	 * @returns angle
	 */
	static int spriteGetRotation(lua_State* luaRuntime)
		{
		float angle = 0;
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sprite.getRotation", 1))
			{
			G2DSprite* spr = (G2DSprite*) G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			if(spr)
				angle = spr->getRotation();
			}
		//
		lua_pushnumber(luaRuntime, angle);
		return 1;
		}

	// -----------------------------------------------------------------------------

	/*-
	 * @name function eos.g2d.sprite.getSize()
	 * @brief gets sprite's current anim size
	 * @param sprite name
	 * @returns width
	 * @returns height
	 */
	static int spriteGetSize(lua_State* luaRuntime)
		{
		uint w = 0,
				  h = 0;
		if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "eos."_MODULE_NAME".sprite.getSize", 1))
			{
			G2DSprite* spr = (G2DSprite*) G2D::getInstance()->getElement(lua_tostring(luaRuntime, 1));
			if(spr)
				{
				w = spr->getWidth();
				h = spr->getHeight();
				}
			}
		//
		lua_pushnumber(luaRuntime, w);
		lua_pushnumber(luaRuntime, h);
		return 2;
		}

	// -----------------------------------------------------------------------------
	// ========================== END LIBRARY FUNCTIONS ============================


	// ======================== LUA FUNCTIONS REGISTRATION =========================

	// ==== Here we declare an array with all functions and their respective names.
	// ==== This array has elements of type luaL_reg, which is a structure with
	// ==== two fields: a string and a function pointer.

	const struct luaL_reg fn_table_module [] = {
		{"cleanup", moduleCleanup},
		{"update", moduleUpdate},
		{NULL, NULL} // <-- end-of-table sentinel
		};

	const struct luaL_reg fn_table_renderTarget [] = {
		{"create", renderTargetCreate},
		{"setCurrent", renderTargetSetCurrent},
		{NULL, NULL} // <-- end-of-table sentinel
		};

	const struct luaL_reg fn_table_element [] = {
		{"setProperty", elementSetProperty},
		{"getProperty", elementGetProperty},
		{"setZOrder", elementSetZOrder},
		{"getZOrder", elementGetZOrder},
		{NULL, NULL} // <-- end-of-table sentinel
		};

	const struct luaL_reg fn_table_sprite [] = {
		{"create", spriteCreate},
		{"addAnim", spriteAddAnim},
		{"setFixedFrame", spriteSetFixedFrame},
		{"animGoToFrame", spriteAnimGoToFrame},
		{"setCurrentAnim", spriteSetCurrentAnim},
		{"getCurrentAnim", spriteGetCurrentAnim},
		{"setHotSpot", spriteSetHotSpot},
		{"setPosition", spriteSetPosition},
		{"setRotation", spriteSetRotation},
		{"bindToObject", spriteBindToObject},
		{"getPosition", spriteGetPosition},
		{"getRotation", spriteGetRotation},
		{"getSize", spriteGetSize},
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

		// registers renderTarget functions
		luaL_register(luaRuntime, "eos."_MODULE_NAME".renderTarget", fn_table_renderTarget);

		// registers element functions
		luaL_register(luaRuntime, "eos."_MODULE_NAME".element", fn_table_element);

		// registers sprite functions
		luaL_register(luaRuntime, "eos."_MODULE_NAME".sprite", fn_table_sprite);

		// set hotspots
		EOSLuaRuntime::getInstance()->executeLuaChunk(
			"eos."_MODULE_NAME".sprite.hotspot = {}\
			eos."_MODULE_NAME".sprite.hotspot.lefttop = 0\
                 eos."_MODULE_NAME".sprite.hotspot.centertop = 1\
                 eos."_MODULE_NAME".sprite.hotspot.righttop = 2\
                 eos."_MODULE_NAME".sprite.hotspot.center = 3\
                 eos."_MODULE_NAME".sprite.hotspot.leftbottom = 4\
                 eos."_MODULE_NAME".sprite.hotspot.centerbottom = 5\
                 eos."_MODULE_NAME".sprite.hotspot.rightbottom = 6\
                ");
		//
		//
		return 1;
		}

	// -----------------------------------------------------------------------------

	} // extern "C" closure
