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


#include "InputManager.h"

// -----------------------------------------------------------------------------

bool InputManager::instanceFlag = false;
InputManager* InputManager::instance = NULL;

// -----------------------------------------------------------------------------

InputManager* InputManager::getInstance()
    {
    if (!instanceFlag)
        {
        instance = new InputManager();
        instanceFlag = true;
        }
    return instance;
    }

// -----------------------------------------------------------------------------

bool InputManager::attachToWindow(size_t windowHandleId, unsigned int width, unsigned int height, bool hideMouse, bool grabInput)
    {
    if (this->attached) return true;
    //
    OIS::ParamList pl;

    char windowHndStr[40];
    sprintf(windowHndStr, "%u", windowHandleId);

    pl.insert(std::make_pair(std::string("WINDOW"), std::string(windowHndStr)));

#if defined OIS_WIN32_PLATFORM    
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string(hideMouse ? "DISCL_EXCLUSIVE" : "DISCL_NONEXCLUSIVE")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
    pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string(grabInput ? "true" : "false")));
    pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string(hideMouse ? "true" : "false")));
    pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string(grabInput ? "true" : "false")));
    pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

    this->oisInputManager = OIS::InputManager::createInputSystem(pl);

    // Creates all devices (We only catch joystick exceptions here, as, most people have Key/Mouse)
    this->oisKeyboard = static_cast<OIS::Keyboard*> (this->oisInputManager->createInputObject(OIS::OISKeyboard, false));
    this->oisMouse = static_cast<OIS::Mouse*> (this->oisInputManager->createInputObject(OIS::OISMouse, false));
    try
        {
        this->oisJoystick = static_cast<OIS::JoyStick*> (this->oisInputManager->createInputObject(OIS::OISJoyStick, false));
        }
    catch (...)
        {
        this->oisJoystick = NULL;
        }

    // Sets initial mouse clipping size
    if (this->oisMouse)
        {
        const OIS::MouseState &ms = this->oisMouse->getMouseState();
        ms.width = width;
        ms.height = height;
        }

    this->attached = true;
    EOSLuaRuntime::getInstance()->logInfoMessage("[eos.oisinput] OIS Input attached to window whith id %u.", windowHandleId);
    //
    return true;
    }

// -----------------------------------------------------------------------------

void InputManager::detach()
    {
    if (this->oisInputManager != NULL)
        {
        this->oisInputManager->destroyInputObject(this->oisMouse);
        this->oisInputManager->destroyInputObject(this->oisKeyboard);
        this->oisInputManager->destroyInputObject(this->oisJoystick);
        OIS::InputManager::destroyInputSystem(this->oisInputManager);
        this->oisInputManager = NULL;
        this->attached = false;
        }
    }

// -----------------------------------------------------------------------------

void InputManager::update()
    {
    if (attached)
        {
        //Need to capture/update each device
        if (this->oisKeyboard) processKeyboardInput();
        if (this->oisMouse) readMouseStatus();
        //
        //if( mJoy ) mJoy->capture();
        }
    }

// -----------------------------------------------------------------------------

void InputManager::processKeyboardInput()
    {
    this->oisKeyboard->capture();
    //
    if (this->oisKeyboard->isKeyDown(OIS::KC_ESCAPE)) pushKeyboardMessage(OIS::KC_ESCAPE);
    if (this->oisKeyboard->isKeyDown(OIS::KC_1)) pushKeyboardMessage(OIS::KC_1);
    if (this->oisKeyboard->isKeyDown(OIS::KC_2)) pushKeyboardMessage(OIS::KC_2);
    if (this->oisKeyboard->isKeyDown(OIS::KC_3)) pushKeyboardMessage(OIS::KC_3);
    if (this->oisKeyboard->isKeyDown(OIS::KC_4)) pushKeyboardMessage(OIS::KC_4);
    if (this->oisKeyboard->isKeyDown(OIS::KC_5)) pushKeyboardMessage(OIS::KC_5);
    if (this->oisKeyboard->isKeyDown(OIS::KC_6)) pushKeyboardMessage(OIS::KC_6);
    if (this->oisKeyboard->isKeyDown(OIS::KC_7)) pushKeyboardMessage(OIS::KC_7);
    if (this->oisKeyboard->isKeyDown(OIS::KC_8)) pushKeyboardMessage(OIS::KC_8);
    if (this->oisKeyboard->isKeyDown(OIS::KC_9)) pushKeyboardMessage(OIS::KC_9);
    if (this->oisKeyboard->isKeyDown(OIS::KC_0)) pushKeyboardMessage(OIS::KC_0);
    if (this->oisKeyboard->isKeyDown(OIS::KC_MINUS)) pushKeyboardMessage(OIS::KC_MINUS);
    if (this->oisKeyboard->isKeyDown(OIS::KC_EQUALS)) pushKeyboardMessage(OIS::KC_EQUALS);
    if (this->oisKeyboard->isKeyDown(OIS::KC_BACK)) pushKeyboardMessage(OIS::KC_BACK);
    if (this->oisKeyboard->isKeyDown(OIS::KC_TAB)) pushKeyboardMessage(OIS::KC_TAB);
    if (this->oisKeyboard->isKeyDown(OIS::KC_Q)) pushKeyboardMessage(OIS::KC_Q);
    if (this->oisKeyboard->isKeyDown(OIS::KC_W)) pushKeyboardMessage(OIS::KC_W);
    if (this->oisKeyboard->isKeyDown(OIS::KC_E)) pushKeyboardMessage(OIS::KC_E);
    if (this->oisKeyboard->isKeyDown(OIS::KC_R)) pushKeyboardMessage(OIS::KC_R);
    if (this->oisKeyboard->isKeyDown(OIS::KC_T)) pushKeyboardMessage(OIS::KC_T);
    if (this->oisKeyboard->isKeyDown(OIS::KC_Y)) pushKeyboardMessage(OIS::KC_Y);
    if (this->oisKeyboard->isKeyDown(OIS::KC_U)) pushKeyboardMessage(OIS::KC_U);
    if (this->oisKeyboard->isKeyDown(OIS::KC_I)) pushKeyboardMessage(OIS::KC_I);
    if (this->oisKeyboard->isKeyDown(OIS::KC_O)) pushKeyboardMessage(OIS::KC_O);
    if (this->oisKeyboard->isKeyDown(OIS::KC_P)) pushKeyboardMessage(OIS::KC_P);
    if (this->oisKeyboard->isKeyDown(OIS::KC_LBRACKET)) pushKeyboardMessage(OIS::KC_LBRACKET);
    if (this->oisKeyboard->isKeyDown(OIS::KC_RBRACKET)) pushKeyboardMessage(OIS::KC_RBRACKET);
    if (this->oisKeyboard->isKeyDown(OIS::KC_RETURN)) pushKeyboardMessage(OIS::KC_RETURN);
    if (this->oisKeyboard->isKeyDown(OIS::KC_LCONTROL)) pushKeyboardMessage(OIS::KC_LCONTROL);
    if (this->oisKeyboard->isKeyDown(OIS::KC_A)) pushKeyboardMessage(OIS::KC_A);
    if (this->oisKeyboard->isKeyDown(OIS::KC_S)) pushKeyboardMessage(OIS::KC_S);
    if (this->oisKeyboard->isKeyDown(OIS::KC_D)) pushKeyboardMessage(OIS::KC_D);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F)) pushKeyboardMessage(OIS::KC_F);
    if (this->oisKeyboard->isKeyDown(OIS::KC_G)) pushKeyboardMessage(OIS::KC_G);
    if (this->oisKeyboard->isKeyDown(OIS::KC_H)) pushKeyboardMessage(OIS::KC_H);
    if (this->oisKeyboard->isKeyDown(OIS::KC_J)) pushKeyboardMessage(OIS::KC_J);
    if (this->oisKeyboard->isKeyDown(OIS::KC_K)) pushKeyboardMessage(OIS::KC_K);
    if (this->oisKeyboard->isKeyDown(OIS::KC_L)) pushKeyboardMessage(OIS::KC_L);
    if (this->oisKeyboard->isKeyDown(OIS::KC_SEMICOLON)) pushKeyboardMessage(OIS::KC_SEMICOLON);
    if (this->oisKeyboard->isKeyDown(OIS::KC_APOSTROPHE)) pushKeyboardMessage(OIS::KC_APOSTROPHE);
    if (this->oisKeyboard->isKeyDown(OIS::KC_GRAVE)) pushKeyboardMessage(OIS::KC_GRAVE);
    if (this->oisKeyboard->isKeyDown(OIS::KC_LSHIFT)) pushKeyboardMessage(OIS::KC_LSHIFT);
    if (this->oisKeyboard->isKeyDown(OIS::KC_BACKSLASH)) pushKeyboardMessage(OIS::KC_BACKSLASH);
    if (this->oisKeyboard->isKeyDown(OIS::KC_Z)) pushKeyboardMessage(OIS::KC_Z);
    if (this->oisKeyboard->isKeyDown(OIS::KC_X)) pushKeyboardMessage(OIS::KC_X);
    if (this->oisKeyboard->isKeyDown(OIS::KC_C)) pushKeyboardMessage(OIS::KC_C);
    if (this->oisKeyboard->isKeyDown(OIS::KC_V)) pushKeyboardMessage(OIS::KC_V);
    if (this->oisKeyboard->isKeyDown(OIS::KC_B)) pushKeyboardMessage(OIS::KC_B);
    if (this->oisKeyboard->isKeyDown(OIS::KC_N)) pushKeyboardMessage(OIS::KC_N);
    if (this->oisKeyboard->isKeyDown(OIS::KC_M)) pushKeyboardMessage(OIS::KC_M);
    if (this->oisKeyboard->isKeyDown(OIS::KC_COMMA)) pushKeyboardMessage(OIS::KC_COMMA);
    if (this->oisKeyboard->isKeyDown(OIS::KC_PERIOD)) pushKeyboardMessage(OIS::KC_PERIOD);
    if (this->oisKeyboard->isKeyDown(OIS::KC_SLASH)) pushKeyboardMessage(OIS::KC_SLASH);
    if (this->oisKeyboard->isKeyDown(OIS::KC_RSHIFT)) pushKeyboardMessage(OIS::KC_RSHIFT);
    if (this->oisKeyboard->isKeyDown(OIS::KC_MULTIPLY)) pushKeyboardMessage(OIS::KC_MULTIPLY);
    if (this->oisKeyboard->isKeyDown(OIS::KC_LMENU)) pushKeyboardMessage(OIS::KC_LMENU);
    if (this->oisKeyboard->isKeyDown(OIS::KC_SPACE)) pushKeyboardMessage(OIS::KC_SPACE);
    if (this->oisKeyboard->isKeyDown(OIS::KC_CAPITAL)) pushKeyboardMessage(OIS::KC_CAPITAL);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F1)) pushKeyboardMessage(OIS::KC_F1);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F2)) pushKeyboardMessage(OIS::KC_F2);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F3)) pushKeyboardMessage(OIS::KC_F3);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F4)) pushKeyboardMessage(OIS::KC_F4);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F5)) pushKeyboardMessage(OIS::KC_F5);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F6)) pushKeyboardMessage(OIS::KC_F6);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F7)) pushKeyboardMessage(OIS::KC_F7);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F8)) pushKeyboardMessage(OIS::KC_F8);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F9)) pushKeyboardMessage(OIS::KC_F9);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F10)) pushKeyboardMessage(OIS::KC_F10);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NUMLOCK)) pushKeyboardMessage(OIS::KC_NUMLOCK);
    if (this->oisKeyboard->isKeyDown(OIS::KC_SCROLL)) pushKeyboardMessage(OIS::KC_SCROLL);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NUMPAD7)) pushKeyboardMessage(OIS::KC_NUMPAD7);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NUMPAD8)) pushKeyboardMessage(OIS::KC_NUMPAD8);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NUMPAD9)) pushKeyboardMessage(OIS::KC_NUMPAD9);
    if (this->oisKeyboard->isKeyDown(OIS::KC_SUBTRACT)) pushKeyboardMessage(OIS::KC_SUBTRACT);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NUMPAD4)) pushKeyboardMessage(OIS::KC_NUMPAD4);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NUMPAD5)) pushKeyboardMessage(OIS::KC_NUMPAD5);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NUMPAD6)) pushKeyboardMessage(OIS::KC_NUMPAD6);
    if (this->oisKeyboard->isKeyDown(OIS::KC_ADD)) pushKeyboardMessage(OIS::KC_ADD);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NUMPAD1)) pushKeyboardMessage(OIS::KC_NUMPAD1);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NUMPAD2)) pushKeyboardMessage(OIS::KC_NUMPAD2);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NUMPAD3)) pushKeyboardMessage(OIS::KC_NUMPAD3);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NUMPAD0)) pushKeyboardMessage(OIS::KC_NUMPAD0);
    if (this->oisKeyboard->isKeyDown(OIS::KC_DECIMAL)) pushKeyboardMessage(OIS::KC_DECIMAL);
    if (this->oisKeyboard->isKeyDown(OIS::KC_OEM_102)) pushKeyboardMessage(OIS::KC_OEM_102);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F11)) pushKeyboardMessage(OIS::KC_F11);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F12)) pushKeyboardMessage(OIS::KC_F12);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F13)) pushKeyboardMessage(OIS::KC_F13);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F14)) pushKeyboardMessage(OIS::KC_F14);
    if (this->oisKeyboard->isKeyDown(OIS::KC_F15)) pushKeyboardMessage(OIS::KC_F15);
    if (this->oisKeyboard->isKeyDown(OIS::KC_KANA)) pushKeyboardMessage(OIS::KC_KANA);
    if (this->oisKeyboard->isKeyDown(OIS::KC_ABNT_C1)) pushKeyboardMessage(OIS::KC_ABNT_C1);
    if (this->oisKeyboard->isKeyDown(OIS::KC_CONVERT)) pushKeyboardMessage(OIS::KC_CONVERT);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NOCONVERT)) pushKeyboardMessage(OIS::KC_NOCONVERT);
    if (this->oisKeyboard->isKeyDown(OIS::KC_YEN)) pushKeyboardMessage(OIS::KC_YEN);
    if (this->oisKeyboard->isKeyDown(OIS::KC_ABNT_C2)) pushKeyboardMessage(OIS::KC_ABNT_C2);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NUMPADEQUALS)) pushKeyboardMessage(OIS::KC_NUMPADEQUALS);
    if (this->oisKeyboard->isKeyDown(OIS::KC_PREVTRACK)) pushKeyboardMessage(OIS::KC_PREVTRACK);
    if (this->oisKeyboard->isKeyDown(OIS::KC_AT)) pushKeyboardMessage(OIS::KC_AT);
    if (this->oisKeyboard->isKeyDown(OIS::KC_COLON)) pushKeyboardMessage(OIS::KC_COLON);
    if (this->oisKeyboard->isKeyDown(OIS::KC_UNDERLINE)) pushKeyboardMessage(OIS::KC_UNDERLINE);
    if (this->oisKeyboard->isKeyDown(OIS::KC_KANJI)) pushKeyboardMessage(OIS::KC_KANJI);
    if (this->oisKeyboard->isKeyDown(OIS::KC_STOP)) pushKeyboardMessage(OIS::KC_STOP);
    if (this->oisKeyboard->isKeyDown(OIS::KC_AX)) pushKeyboardMessage(OIS::KC_AX);
    if (this->oisKeyboard->isKeyDown(OIS::KC_UNLABELED)) pushKeyboardMessage(OIS::KC_UNLABELED);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NEXTTRACK)) pushKeyboardMessage(OIS::KC_NEXTTRACK);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NUMPADENTER)) pushKeyboardMessage(OIS::KC_NUMPADENTER);
    if (this->oisKeyboard->isKeyDown(OIS::KC_RCONTROL)) pushKeyboardMessage(OIS::KC_RCONTROL);
    if (this->oisKeyboard->isKeyDown(OIS::KC_MUTE)) pushKeyboardMessage(OIS::KC_MUTE);
    if (this->oisKeyboard->isKeyDown(OIS::KC_CALCULATOR)) pushKeyboardMessage(OIS::KC_CALCULATOR);
    if (this->oisKeyboard->isKeyDown(OIS::KC_PLAYPAUSE)) pushKeyboardMessage(OIS::KC_PLAYPAUSE);
    if (this->oisKeyboard->isKeyDown(OIS::KC_MEDIASTOP)) pushKeyboardMessage(OIS::KC_MEDIASTOP);
    if (this->oisKeyboard->isKeyDown(OIS::KC_VOLUMEDOWN)) pushKeyboardMessage(OIS::KC_VOLUMEDOWN);
    if (this->oisKeyboard->isKeyDown(OIS::KC_VOLUMEUP)) pushKeyboardMessage(OIS::KC_VOLUMEUP);
    if (this->oisKeyboard->isKeyDown(OIS::KC_WEBHOME)) pushKeyboardMessage(OIS::KC_WEBHOME);
    if (this->oisKeyboard->isKeyDown(OIS::KC_NUMPADCOMMA)) pushKeyboardMessage(OIS::KC_NUMPADCOMMA);
    if (this->oisKeyboard->isKeyDown(OIS::KC_DIVIDE)) pushKeyboardMessage(OIS::KC_DIVIDE);
    if (this->oisKeyboard->isKeyDown(OIS::KC_SYSRQ)) pushKeyboardMessage(OIS::KC_SYSRQ);
    if (this->oisKeyboard->isKeyDown(OIS::KC_RMENU)) pushKeyboardMessage(OIS::KC_RMENU);
    if (this->oisKeyboard->isKeyDown(OIS::KC_PAUSE)) pushKeyboardMessage(OIS::KC_PAUSE);
    if (this->oisKeyboard->isKeyDown(OIS::KC_HOME)) pushKeyboardMessage(OIS::KC_HOME);
    if (this->oisKeyboard->isKeyDown(OIS::KC_UP)) pushKeyboardMessage(OIS::KC_UP);
    if (this->oisKeyboard->isKeyDown(OIS::KC_PGUP)) pushKeyboardMessage(OIS::KC_PGUP);
    if (this->oisKeyboard->isKeyDown(OIS::KC_LEFT)) pushKeyboardMessage(OIS::KC_LEFT);
    if (this->oisKeyboard->isKeyDown(OIS::KC_RIGHT)) pushKeyboardMessage(OIS::KC_RIGHT);
    if (this->oisKeyboard->isKeyDown(OIS::KC_END)) pushKeyboardMessage(OIS::KC_END);
    if (this->oisKeyboard->isKeyDown(OIS::KC_DOWN)) pushKeyboardMessage(OIS::KC_DOWN);
    if (this->oisKeyboard->isKeyDown(OIS::KC_PGDOWN)) pushKeyboardMessage(OIS::KC_PGDOWN);
    if (this->oisKeyboard->isKeyDown(OIS::KC_INSERT)) pushKeyboardMessage(OIS::KC_INSERT);
    if (this->oisKeyboard->isKeyDown(OIS::KC_DELETE)) pushKeyboardMessage(OIS::KC_DELETE);
    if (this->oisKeyboard->isKeyDown(OIS::KC_LWIN)) pushKeyboardMessage(OIS::KC_LWIN);
    if (this->oisKeyboard->isKeyDown(OIS::KC_RWIN)) pushKeyboardMessage(OIS::KC_RWIN);
    if (this->oisKeyboard->isKeyDown(OIS::KC_APPS)) pushKeyboardMessage(OIS::KC_APPS);
    if (this->oisKeyboard->isKeyDown(OIS::KC_POWER)) pushKeyboardMessage(OIS::KC_POWER);
    if (this->oisKeyboard->isKeyDown(OIS::KC_SLEEP)) pushKeyboardMessage(OIS::KC_SLEEP);
    if (this->oisKeyboard->isKeyDown(OIS::KC_WAKE)) pushKeyboardMessage(OIS::KC_WAKE);
    if (this->oisKeyboard->isKeyDown(OIS::KC_WEBFAVORITES)) pushKeyboardMessage(OIS::KC_WEBFAVORITES);
    if (this->oisKeyboard->isKeyDown(OIS::KC_WEBREFRESH)) pushKeyboardMessage(OIS::KC_WEBREFRESH);
    if (this->oisKeyboard->isKeyDown(OIS::KC_WEBSTOP)) pushKeyboardMessage(OIS::KC_WEBSTOP);
    if (this->oisKeyboard->isKeyDown(OIS::KC_WEBFORWARD)) pushKeyboardMessage(OIS::KC_WEBFORWARD);
    if (this->oisKeyboard->isKeyDown(OIS::KC_WEBBACK)) pushKeyboardMessage(OIS::KC_WEBBACK);
    if (this->oisKeyboard->isKeyDown(OIS::KC_MYCOMPUTER)) pushKeyboardMessage(OIS::KC_MYCOMPUTER);
    if (this->oisKeyboard->isKeyDown(OIS::KC_MAIL)) pushKeyboardMessage(OIS::KC_MAIL);
    if (this->oisKeyboard->isKeyDown(OIS::KC_MEDIASELECT)) pushKeyboardMessage(OIS::KC_MEDIASELECT);
    }

// -----------------------------------------------------------------------------

void InputManager::pushKeyboardMessage(unsigned int keyCode)
    {
    sprintf(this->cmdBuffer, "eos.message.init(\"input\") eos.message.pushData(\"keyCode\", %d) eos.message.send(\"\")", keyCode);
    EOSLuaRuntime::getInstance()->executeLuaChunk(this->cmdBuffer);
    }

// -----------------------------------------------------------------------------

bool InputManager::isKeyPressed(OIS::KeyCode keyCode)
    {
    if (keyCode > 0 && keyCode <= 0xED)
        return this->oisKeyboard->isKeyDown(keyCode);
    return false;
    }

// -----------------------------------------------------------------------------

bool InputManager::isMouseClicked(OIS::KeyCode mouseBtnCode)
    {
    /*const OIS::MouseState &ms = this->oisMouse->getMouseState();
    if(ms.buttonDown( mouseBtnCode ))
       return true;*/
    return false;
    }

// -----------------------------------------------------------------------------

void InputManager::readMouseStatus()
    {
    const OIS::MouseState &ms = this->oisMouse->getMouseState();
    pushMouseMessage(ms.X.abs, ms.Y.abs, ms.Z.abs, ms.X.rel, ms.Y.rel, ms.Z.rel, ms.buttons);
    //
    }

// -----------------------------------------------------------------------------

void InputManager::pushMouseMessage(int Xabs, int Yabs, int Zabs, int Xrel, int Yrel, int Zrel, int buttons)
    {
    sprintf(this->cmdBuffer, "eos.message.init(\"input\") eos.message.pushData(\"x_abs\", %d) eos.message.pushData(\"y_abs\", %d) eos.message.pushData(\"z_abs\", %d) eos.message.pushData(\"x_rel\", %d) eos.message.pushData(\"y_rel\", %d) eos.message.pushData(\"z_rel\", %d) eos.message.pushData(\"buttons\", %d) eos.message.send(\"\")", Xabs, Yabs, Zabs, Xrel, Yrel, Zrel, buttons);
    EOSLuaRuntime::getInstance()->executeLuaChunk(this->cmdBuffer);
    }

// -----------------------------------------------------------------------------

void InputManager::getMouseCoords(int* x, int* y)
    {
    //SDL_GetMouseState(x, y);
    }

// -----------------------------------------------------------------------------

void InputManager::getMouseRelativeCoords(int* x, int* y)
    {
    //SDL_GetRelativeMouseState(x, y);
    }

// -----------------------------------------------------------------------------
