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

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <OIS/OIS.h>
#include "EOSLuaRuntime.h"

class InputManager
    {
    public:
        static InputManager* getInstance();
        ~InputManager() { instanceFlag = false;  }
        //
        bool attachToWindow(size_t windowHandleId, unsigned int width, unsigned int height, bool hideMouse, bool grabMouse);
        void detach();
        //
        void update();
        //
        void getMouseCoords(int* x, int* y);
        void getMouseRelativeCoords(int* x, int* y);
        //
        bool isAttached() { return this->attached; }
        //
        bool isKeyPressed(OIS::KeyCode keyCode);
        bool isMouseClicked(OIS::KeyCode mouseBtnCode);
        //
    private:
        InputManager() {  attached = false;  }
        //
        static bool instanceFlag;
        static InputManager* instance;
        //
        //OIS Input devices
        OIS::InputManager* oisInputManager;
        OIS::Mouse*        oisMouse;
        OIS::Keyboard*     oisKeyboard;
        OIS::JoyStick*     oisJoystick;
        //
        void processKeyboardInput();
        void pushKeyboardMessage(unsigned int keyCode);
        void readMouseStatus();
        void pushMouseMessage(int Xabs, int Yabs, int Zabs, int Xrel, int Yrel, int Zrel, int buttons);
        //
        bool attached;
        char cmdBuffer[512];
    };


#endif
