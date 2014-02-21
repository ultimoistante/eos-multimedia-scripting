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

#ifndef EOS_SKELTRACK_SKELTRACKMANAGER_H
#define EOS_SKELTRACK_SKELTRACKMANAGER_H

#include "EOSObject.h"
#include "eos_data_containers.h"
#include "EOSLuaRuntime.h"
#include <glib.h>
#include <glib-object.h>
#include <gfreenect.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <skeltrack.h>
#include <cairo.h>
#include <glib-object.h>
#include <clutter/clutter.h>
#include <clutter/clutter-keysyms.h>



class SkeltrackManager
	{
	public:
		SkeltrackManager();
		virtual ~SkeltrackManager();
		static SkeltrackManager* getInstance();
		//
		bool powerOn();
		void shutdown();
		void update();
		//

	private:
		static bool instanceFlag;
		static SkeltrackManager* instance;
		//static char luaCmdBuffer[512];
		//
	} ;

#endif
