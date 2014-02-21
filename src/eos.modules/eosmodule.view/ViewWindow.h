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

#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

#include <vector>
#include <GLee.h>
#include <GL/glu.h>

#include "../modulesdk/EOSLuaRuntime.h"
#include "../modulesdk/EOSPrecisionTimer.h"
#include "../modulesdk/EOSProfiler.h"
#include "ViewDefines.h"
#include "ViewPlatformSupport.h"

class ViewWindow
    {
    public:
        ViewWindow(uint screen, uint width, uint height, uint bpp, uint antialiasing, bool fullscreen);
        ~ViewWindow();

        bool open();
        void resize(uint width, uint height);
        bool update();
        void setCaption(const char* text);

        ulong getId()           { return (ulong)this->winData.window; }
        ulong getParentId()     { return (ulong)this->winData.parent; }
        float getFps()          { return this->fpsCounter; }
        uint  getWidth()        { return this->width; }
        uint  getHeight()       { return this->height; }
        uint  getScreen()       { return this->screen; }

        void takeContext();
        void releaseContext();

        bool registerSurface(uint surfaceId);
        bool unregisterSurface(uint surfaceId);    

    private:
        bool initGL();
        //
        ViewWindowData winData;
        //
        std::string caption;
        uint        screen,
                    width,
                    height,
                    bpp,
                    antialiasing;
        bool        fullscreen;
        //
        EOSProfiler* prof;
        EOSPrecisionTimer* timer;
        int         framesUpdated;
        float       fpsCounter;
        //
        std::vector<uint> surfaces;
    };

#endif
