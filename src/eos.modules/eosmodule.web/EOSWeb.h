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

/* 
 * File:   EOSWeb.h
 * Author: alex9978
 *
 * Created on 10 novembre 2012, 1.03
 */

#ifndef EOSWEB_H
	#define	EOSWEB_H

	#include "EOSObject.h"
	#include "eos_data_containers.h"

	#include <gtk/gtk.h>
	#include <webkit/webkit.h>
	#include <cairo/cairo.h>
	#include "tinythread.h"


class EOSWeb
	{
public:
	static EOSWeb* getInstance();

	~EOSWeb()
		{
		instanceFlag = false;
		}

	bool initialize();
	void update();

	EOSObject* load_webpage(const char *uri);

private:

	EOSWeb()
		{
		initialized = false;
		}
	//
	static bool instanceFlag;
	static EOSWeb* instance;
	bool initialized;
	eos_imagedata sharedImagedata;
	//
	GtkWidget* offscreenWindow;
	WebKitWebView* webView;
	//
	unsigned char *buffer;
	GdkPixbuf *pixbuffer;

    static void threadRun(void* params);


	GtkWidget* create_offscreen_window(int width, int height);
	EOSObject* createSessionObject(GdkPixbuf *pixbuf);
	void convertBuffer(GdkPixbuf *pixbuf);
	static void loadStatusCb(WebKitWebView *web_view, GParamSpec *pspec, void* context);
	} ;

#endif	/* EOSWEB_H */

