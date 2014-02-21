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

#ifndef VIEW_H
#define VIEW_H

#include <stdio.h>
#include <map>
#include <vector>
#include <GLee.h>
#include "EOSLuaRuntime.h"
#include "ViewDefines.h"
#include "ViewScreenInfo.h"
#include "ViewWindow.h"
#include "ViewSurface.h"

class View
    {
    public:
        View();
        virtual ~View();
        static View* getInstance();
        void shutdown();

        uint getScreenCount();
        bool getScreenInfo(uint num, ViewScreenInfo *si);

        ulong createWindow(uint display, uint width, uint height, uint bpp, uint antialiasing, bool fullscreen);
        bool updateWindow(ulong windowId);
        ViewWindow* getWindowFromId(ulong windowId);

        void setCurrentWindow(ulong windowId);
        ViewWindow* getCurrentWindow();

        uint createSurface();
        uint createSubSurface();
        ViewSurface* getSurfaceFromId(uint surfaceId);

    private:
        static bool instanceFlag;
        static View* instance;

        ViewWindow *currentWindow;
        std::map<ulong, ViewWindow*> windows;

        std::vector<ViewSurface*> surfaces;
    };

#endif
