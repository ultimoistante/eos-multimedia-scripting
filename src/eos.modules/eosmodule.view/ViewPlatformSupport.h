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

#ifndef VIEWPLATFORMSUPPORT_H
#define	VIEWPLATFORMSUPPORT_H

#include <stdio.h>
#include <string.h>
#include <GLee.h>
#include "ViewDefines.h"

#if defined linux
    #include <GL/glx.h>
    #include <X11/Xlib.h>
    #include <X11/keysym.h>
    namespace X11
        {
        #include <X11/extensions/xf86vmode.h>
        }
#elif defined _WIN32
    #include <windows.h>
#endif

// -----------------------------------------------------------------------------

typedef struct view_window_data
    {    
    char*       windowTitle;
    int         xpos;
    int         ypos;
    int         width;
    int         height;
    int         pixelFormatFlags;
    bool        fullscreen;
    bool        doubleBuffered;
    bool        toBeResized;
    bool        toBeClosed;         // TODO: to remove (only for development)

    #if defined linux

    Display*    display;
    Window      window;
    Window      parent;
    int         screen;
    GLXContext  context_pri;
    GLXContext  context_sec;
    X11::XF86VidModeModeInfo desktopMode;
    XSetWindowAttributes     attributes;

    #elif defined _WIN32

    HWND         hWnd;

    #endif

    } ViewWindowData;

// -----------------------------------------------------------------------------

typedef struct view_surface_data
    {
    uint    pbo_ids[2];
    uchar*  buffer;
    uint    bufferSize;    
    bool    staticBuffer;

    } ViewSurfaceData;

// -----------------------------------------------------------------------------

class ViewPlatformSupport
    {
    public:
        // screen related
        static int getScreenCount();
        // window related
        static bool makeWindow(int screenNum, const char *name, int x, int y, int width, int height, bool fullscreen, ViewWindowData *winData);
        static void destroyWindow(ViewWindowData *winData);
        static void processWindowEvents(ViewWindowData *winData);
        static void setWindowCaption(ViewWindowData *winData);
        static void swapBuffers(ViewWindowData *winData, bool waitVSync);
        static void setContext(ViewWindowData *winData, int contextId);
        // surface related
        static bool makePBO(ViewSurfaceData *srfData);
        static void destroyPBO(ViewSurfaceData *srfData);
    };

#endif

