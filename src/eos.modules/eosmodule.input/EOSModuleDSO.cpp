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
#include <stdlib.h>

//#include <string>
#include <vector>

#include "InputManager.h"

extern "C"
    {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "EOSLuaRuntime.h"



    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    //
    // Module manifest structure. Developer must edit just the #define values below.
    //
#define _MODULE_NAME            "input"
#define _MODULE_VERSION         "0.3"
#define _MODULE_DATE            "October 31, 2012"
#define _MODULE_DESC_SHORT      "Input Module for EOS Scripting Environment"
#define _MODULE_DESC_LONG       ""
#define _MODULE_AUTHOR_NAME     "EOS Scripting Team"
#define _MODULE_AUTHOR_WEBSITE  "http://sourceforge.net/projects/eos-scripting/"
    //
    // -----------------------------------------------------------------------------
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
        //
        EOSLuaRuntime::getInstance()->logInfoMessage("[eos."_MODULE_NAME"] Shutting down...");
        return 0;
        }

    // ====================== CUSTOM MODULE FUNCTIONS ==============================
    // -----------------------------------------------------------------------------

    // --- First, we define our library functions:

    static int attachToWindow(lua_State* luaRuntime)
        {
        bool done = false;
        if (EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "attachToWindow", 5))
            done = InputManager::getInstance()->attachToWindow((unsigned long) lua_tonumber(luaRuntime, 1), (unsigned int) lua_tonumber(luaRuntime, 2), (unsigned int) lua_tonumber(luaRuntime, 3), lua_toboolean(luaRuntime, 4), lua_toboolean(luaRuntime, 5));
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    static int detach(lua_State* luaRuntime)
        {
        InputManager::getInstance()->detach();
        //
        return 0;
        }

    // -----------------------------------------------------------------------------

    static int update(lua_State* luaRuntime)
        {
        bool done = false;
        if (InputManager::getInstance()->isAttached())
            {
            InputManager::getInstance()->update();
            done = true;
            }
        //
        lua_pushboolean(luaRuntime, done);
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="mouseGetCoords">
                    <language code="en">
                            <brief text="Gets current mouse pointer position" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="mouse_x" type="Numeric" desc="current mouse pointer X position" />
                                    <return name="mouse_y" type="Numeric" desc="current mouse pointer Y position" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna la posizione corrente del puntatore del mouse" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="mouse_x" type="Numeric" desc="posizione corrente X del puntatore del mouse" />
                                    <return name="mouse_y" type="Numeric" desc="posizione corrente Y del puntatore del mouse" />
                            </returns>
                    </language>
            </function>
     */
    static int mouseGetCoords(lua_State* luaRuntime)
        {
        int x = 0, y = 0;
        if (InputManager::getInstance()->isAttached())
            {
            InputManager::getInstance()->getMouseCoords(&x, &y);
            }
        //
        lua_pushnumber(luaRuntime, x);
        lua_pushnumber(luaRuntime, y);
        return 2;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="mouseGetRelativeCoords">
                    <language code="en">
                            <brief text="Gets relative mouse pointer movement (in pixels) from previous frame" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="delta_x" type="Numeric" desc="mouse X movement amount (pixels)" />
                                    <return name="delta_y" type="Numeric" desc="mouse Y movement amount (pixels)" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Ritorna lo spostamento del mouse (in pixels) relativo al frame precedente" />
                            <description text="" />
                            <params>
                            </params>
                            <returns>
                                    <return name="delta_x" type="Numeric" desc="spostamento X del mouse (in pixels)" />
                                    <return name="delta_y" type="Numeric" desc="spostamento Y del mouse (in pixels)" />
                            </returns>
                    </language>
            </function>
     */
    static int mouseGetRelativeCoords(lua_State* luaRuntime)
        {
        int x = 0, y = 0;
        if (InputManager::getInstance()->isAttached())
            InputManager::getInstance()->getMouseRelativeCoords(&x, &y);
        //
        lua_pushnumber(luaRuntime, x);
        lua_pushnumber(luaRuntime, y);
        return 2;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="keyState">
                    <language code="en">
                            <brief text="Tests if a key is pressed" />
                            <description text="" />
                            <params>
                                    <param name="keyId" type="Constant" desc="the id of the key to test" />
                            </params>
                            <returns>
                                    <return name="pressed" type="Boolean" desc="true if pressed, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Testa lo stato di pressione di un tasto" />
                            <description text="" />
                            <params>
                                    <param name="keyId" type="Constant" desc="l'id del tasto da controllare" />
                            </params>
                            <returns>
                                    <return name="pressed" type="Boolean" desc="true se premuto, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    static int keyState(lua_State* luaRuntime)
        {
        if (EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME, "keyState", 1))
            lua_pushboolean(luaRuntime, InputManager::getInstance()->isKeyPressed((OIS::KeyCode)lua_tonumber(luaRuntime, 1)));
        return 1;
        }

    // -----------------------------------------------------------------------------

    /**
            <function name="mouseKeyState">
                    <language code="en">
                            <brief text="Tests if a mouse key is pressed" />
                            <description text="" />
                            <params>
                                    <param name="mouseKeyId" type="Constant" desc="the id of the key to test" />
                            </params>
                            <returns>
                                    <return name="pressed" type="Boolean" desc="true if pressed, false otherwise" />
                            </returns>
                    </language>
                    <language code="it">
                            <brief text="Testa lo stato di pressione di un tasto del mouse" />
                            <description text="" />
                            <params>
                                    <param name="mouseKeyId" type="Constant" desc="l'id del tasto da controllare" />
                            </params>
                            <returns>
                                    <return name="pressed" type="Boolean" desc="true se premuto, false altrimenti" />
                            </returns>
                    </language>
            </function>
     */
    static int mouseKeyState(lua_State* luaRuntime)
        {
        /*if(EOSLuaRuntime::checkLuaParameters("eos."_MODULE_NAME", "mouseKeyState", 1))
                lua_pushboolean(luaRuntime, OISInputManager::getInstance()->isMouseClicked((int)lua_tonumber(luaRuntime, 1)));
        return 1;*/
        return 0;
        }

    // -----------------------------------------------------------------------------
    // ========================== END LIBRARY FUNCTIONS ============================

    class KeyValue
        {
    public:

        KeyValue(const char* key, int value)
            {
            this->key = strdup(key);
            this->value = value;
            }

        ~KeyValue()
            {
            free(this->key);
            }
        char* key;
        int value;
            };

    std::vector<KeyValue*> keysymbols;

    void loadKeysymSymbols()
        {
        // keyboard symbols:
        keysymbols.push_back(new KeyValue("KEY_ESC", OIS::KC_ESCAPE)); /** <symbol name="KEY_ESC"         ><language code="en"><description text="ESC key"                             /></language><language code="it"><description text="Tasto ESC"                                /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_1", OIS::KC_1)); /** <symbol name="KEY_1"           ><language code="en"><description text="ONE   ('1') character key"           /></language><language code="it"><description text="Tasto carattere UNO    ('1')"             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_2", OIS::KC_2)); /** <symbol name="KEY_2"           ><language code="en"><description text="TWO   ('2') character key"           /></language><language code="it"><description text="Tasto carattere DUE    ('2')"             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_3", OIS::KC_3)); /** <symbol name="KEY_3"           ><language code="en"><description text="THREE ('3') character key"           /></language><language code="it"><description text="Tasto carattere TRE    ('3')"             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_4", OIS::KC_4)); /** <symbol name="KEY_4"           ><language code="en"><description text="FOUR  ('4') character key"           /></language><language code="it"><description text="Tasto carattere QUATTRO('4')"             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_5", OIS::KC_5)); /** <symbol name="KEY_5"           ><language code="en"><description text="FIVE  ('5') character key"           /></language><language code="it"><description text="Tasto carattere CINQUE ('5')"             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_6", OIS::KC_6)); /** <symbol name="KEY_6"           ><language code="en"><description text="SIX   ('6') character key"           /></language><language code="it"><description text="Tasto carattere SEI    ('6')"             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_7", OIS::KC_7)); /** <symbol name="KEY_7"           ><language code="en"><description text="SEVEN ('7') character key"           /></language><language code="it"><description text="Tasto carattere SETTE  ('7')"             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_8", OIS::KC_8)); /** <symbol name="KEY_8"           ><language code="en"><description text="EIGHT ('8') character key"           /></language><language code="it"><description text="Tasto carattere OTTO   ('8')"             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_9", OIS::KC_9)); /** <symbol name="KEY_9"           ><language code="en"><description text="NINE  ('9') character key"           /></language><language code="it"><description text="Tasto carattere NOVE   ('9')"             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_0", OIS::KC_0)); /** <symbol name="KEY_0"           ><language code="en"><description text="ZERO  ('0') character key"           /></language><language code="it"><description text="Tasto carattere ZERO   ('0')"             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_MINUS", OIS::KC_MINUS)); /** <symbol name="KEY_MINUS"       ><language code="en"><description text="MINUS ('-') character key"           /></language><language code="it"><description text="Tasto carattere MENO   ('-')"             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_EQUALS", OIS::KC_EQUALS)); /** <symbol name="KEY_EQUALS"      ><language code="en"><description text="EQUALS ('=') character key"          /></language><language code="it"><description text="Tasto carattere UGUALE ('=')"             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_BACKSPACE", OIS::KC_BACK)); /** <symbol name="KEY_BACKSPACE"   ><language code="en"><description text="BACKSPACE key"                       /></language><language code="it"><description text="Tasto BACKSPACE"                          /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_TAB", OIS::KC_TAB)); /** <symbol name="KEY_TAB"         ><language code="en"><description text="TAB key"                             /></language><language code="it"><description text="Tasto TAB"                                /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_Q", OIS::KC_Q)); /** <symbol name="KEY_Q"           ><language code="en"><description text="Q ('q') character key"               /></language><language code="it"><description text="Tasto carattere Q ('q')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_W", OIS::KC_W)); /** <symbol name="KEY_W"           ><language code="en"><description text="W ('w') character key"               /></language><language code="it"><description text="Tasto carattere W ('w')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_E", OIS::KC_E)); /** <symbol name="KEY_E"           ><language code="en"><description text="E ('e') character key"               /></language><language code="it"><description text="Tasto carattere E ('e')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_R", OIS::KC_R)); /** <symbol name="KEY_R"           ><language code="en"><description text="R ('r') character key"               /></language><language code="it"><description text="Tasto carattere R ('r')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_T", OIS::KC_T)); /** <symbol name="KEY_T"           ><language code="en"><description text="T ('t') character key"               /></language><language code="it"><description text="Tasto carattere T ('t')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_Y", OIS::KC_Y)); /** <symbol name="KEY_Y"           ><language code="en"><description text="Y ('y') character key"               /></language><language code="it"><description text="Tasto carattere Y ('y')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_U", OIS::KC_U)); /** <symbol name="KEY_U"           ><language code="en"><description text="U ('u') character key"               /></language><language code="it"><description text="Tasto carattere U ('u')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_I", OIS::KC_I)); /** <symbol name="KEY_I"           ><language code="en"><description text="I ('i') character key"               /></language><language code="it"><description text="Tasto carattere I ('i')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_O", OIS::KC_O)); /** <symbol name="KEY_O"           ><language code="en"><description text="O ('o') character key"               /></language><language code="it"><description text="Tasto carattere O ('o')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_P", OIS::KC_P)); /** <symbol name="KEY_P"           ><language code="en"><description text="P ('p') character key"               /></language><language code="it"><description text="Tasto carattere P ('p')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_LEFTBRACKET", OIS::KC_LBRACKET)); /** <symbol name="KEY_LEFTBRACKET" ><language code="en"><description text="LEFT BRACKET ('[') character key"    /></language><language code="it"><description text="Tasto carattere PAR. QUADRA APERTA ('[')" /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_RIGHTBRACKET", OIS::KC_RBRACKET)); /** <symbol name="KEY_RIGHTBRACKET"><language code="en"><description text="RIGHT BRACKET (']') character key"   /></language><language code="it"><description text="Tasto carattere PAR. QUADRA CHIUSA (']')" /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_RETURN", OIS::KC_RETURN)); /** <symbol name="KEY_RETURN"      ><language code="en"><description text="RETURN (ENTER) key"                  /></language><language code="it"><description text="Tasto RETURN (ENTER)"                     /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_LCTRL", OIS::KC_LCONTROL)); /** <symbol name="KEY_LCTRL"       ><language code="en"><description text="LEFT CTRL key"                       /></language><language code="it"><description text="Tasto CTRL Sinistro"                      /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_A", OIS::KC_A)); /** <symbol name="KEY_A"           ><language code="en"><description text="A ('a') character key"               /></language><language code="it"><description text="Tasto carattere A ('a')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_S", OIS::KC_S)); /** <symbol name="KEY_S"           ><language code="en"><description text="S ('s') character key"               /></language><language code="it"><description text="Tasto carattere S ('s')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_D", OIS::KC_D)); /** <symbol name="KEY_D"           ><language code="en"><description text="D ('d') character key"               /></language><language code="it"><description text="Tasto carattere D ('d')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F", OIS::KC_F)); /** <symbol name="KEY_F"           ><language code="en"><description text="F ('f') character key"               /></language><language code="it"><description text="Tasto carattere F ('f')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_G", OIS::KC_G)); /** <symbol name="KEY_G"           ><language code="en"><description text="G ('g') character key"               /></language><language code="it"><description text="Tasto carattere G ('g')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_H", OIS::KC_H)); /** <symbol name="KEY_H"           ><language code="en"><description text="H ('h') character key"               /></language><language code="it"><description text="Tasto carattere H ('h')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_J", OIS::KC_J)); /** <symbol name="KEY_J"           ><language code="en"><description text="J ('j') character key"               /></language><language code="it"><description text="Tasto carattere J ('j')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_K", OIS::KC_K)); /** <symbol name="KEY_K"           ><language code="en"><description text="K ('k') character key"               /></language><language code="it"><description text="Tasto carattere K ('k')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_L", OIS::KC_L)); /** <symbol name="KEY_L"           ><language code="en"><description text="L ('l') character key"               /></language><language code="it"><description text="Tasto carattere L ('l')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_SEMICOLON", OIS::KC_SEMICOLON)); /** <symbol name="KEY_SEMICOLON"   ><language code="en"><description text="SEMICOLON (';') character key"       /></language><language code="it"><description text="Tasto carattere PUNTO E VIRGOLA (';')"    /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_APOSTROPHE", OIS::KC_APOSTROPHE)); /** <symbol name="KEY_APOSTROPHE"  ><language code="en"><description text="APOSTROPHE (') character key"        /></language><language code="it"><description text="Tasto carattere APOSTROFO (')"            /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_GRAVE", OIS::KC_GRAVE)); /** <symbol name="KEY_GRAVE"       ><language code="en"><description text="ACCENT (') character key"            /></language><language code="it"><description text="Tasto carattere ACCENTO (')"              /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_LSHIFT", OIS::KC_LSHIFT)); /** <symbol name="KEY_LSHIFT"      ><language code="en"><description text="LEFT SHIFT key"                      /></language><language code="it"><description text="Tasto SHIFT Sinistro"                     /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_BACKSLASH", OIS::KC_BACKSLASH)); /** <symbol name="KEY_BACKSLASH"   ><language code="en"><description text="BACKSLASH ('\\') character key"      /></language><language code="it"><description text="Tasto carattere BACKSLASH ('\\')"         /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_Z", OIS::KC_Z)); /** <symbol name="KEY_Z"           ><language code="en"><description text="Z ('z') character key"               /></language><language code="it"><description text="Tasto carattere Z ('z')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_X", OIS::KC_X)); /** <symbol name="KEY_X"           ><language code="en"><description text="X ('x') character key"               /></language><language code="it"><description text="Tasto carattere X ('x')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_C", OIS::KC_C)); /** <symbol name="KEY_C"           ><language code="en"><description text="C ('c') character key"               /></language><language code="it"><description text="Tasto carattere C ('c')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_V", OIS::KC_V)); /** <symbol name="KEY_V"           ><language code="en"><description text="V ('v') character key"               /></language><language code="it"><description text="Tasto carattere V ('v')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_B", OIS::KC_B)); /** <symbol name="KEY_B"           ><language code="en"><description text="B ('b') character key"               /></language><language code="it"><description text="Tasto carattere B ('b')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_N", OIS::KC_N)); /** <symbol name="KEY_N"           ><language code="en"><description text="N ('n') character key"               /></language><language code="it"><description text="Tasto carattere N ('n')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_M", OIS::KC_M)); /** <symbol name="KEY_M"           ><language code="en"><description text="M ('m') character key"               /></language><language code="it"><description text="Tasto carattere M ('m')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_COMMA", OIS::KC_COMMA)); /** <symbol name="KEY_COMMA"       ><language code="en"><description text="COMMA (',') character key"           /></language><language code="it"><description text="Tasto carattere VIRGOLA (',')"            /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_PERIOD", OIS::KC_PERIOD)); /** <symbol name="KEY_PERIOD"      ><language code="en"><description text="PERIOD ('.') character key"          /></language><language code="it"><description text="Tasto carattere PUNTO ('.')"              /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_SLASH", OIS::KC_SLASH)); /** <symbol name="KEY_SLASH"       ><language code="en"><description text="SLASH ('/') character key"           /></language><language code="it"><description text="Tasto carattere SLASH ('/')"              /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_RSHIFT", OIS::KC_RSHIFT)); /** <symbol name="KEY_RSHIFT"      ><language code="en"><description text="RIGHT SHIFT key"                     /></language><language code="it"><description text="Tasto SHIFT Destro"                       /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP_MULTIPLY", OIS::KC_MULTIPLY)); /** <symbol name="KEY_KP_MULTIPLY" ><language code="en"><description text="KEYPAD MULTIPLY ('*') key"           /></language><language code="it"><description text="Tasto NUMPAD MULTIPLY ('*')"              /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_LALT", OIS::KC_LMENU)); /** <symbol name="KEY_LALT"        ><language code="en"><description text="LEFT ALT key"                        /></language><language code="it"><description text="Tasto ALT SINISTRO"                       /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_SPACE", OIS::KC_SPACE)); /** <symbol name="KEY_SPACE"       ><language code="en"><description text="SPACE bar"                           /></language><language code="it"><description text="Barra SPAZIO"                             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_CAPSLOCK", OIS::KC_CAPITAL)); /** <symbol name="KEY_CAPSLOCK"    ><language code="en"><description text="CAPS LOCK key"                       /></language><language code="it"><description text="Tasto BLOC MAIUSC (CAPS LOCK)"            /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F1", OIS::KC_F1)); /** <symbol name="KEY_F1"          ><language code="en"><description text="function key F1"                     /></language><language code="it"><description text="Tasto funzione F1"                        /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F2", OIS::KC_F2)); /** <symbol name="KEY_F2"          ><language code="en"><description text="function key F2"                     /></language><language code="it"><description text="Tasto funzione F2"                        /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F3", OIS::KC_F3)); /** <symbol name="KEY_F3"          ><language code="en"><description text="function key F3"                     /></language><language code="it"><description text="Tasto funzione F3"                        /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F4", OIS::KC_F4)); /** <symbol name="KEY_F4"          ><language code="en"><description text="function key F4"                     /></language><language code="it"><description text="Tasto funzione F4"                        /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F5", OIS::KC_F5)); /** <symbol name="KEY_F5"          ><language code="en"><description text="function key F5"                     /></language><language code="it"><description text="Tasto funzione F5"                        /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F6", OIS::KC_F6)); /** <symbol name="KEY_F6"          ><language code="en"><description text="function key F6"                     /></language><language code="it"><description text="Tasto funzione F6"                        /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F7", OIS::KC_F7)); /** <symbol name="KEY_F7"          ><language code="en"><description text="function key F7"                     /></language><language code="it"><description text="Tasto funzione F7"                        /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F8", OIS::KC_F8)); /** <symbol name="KEY_F8"          ><language code="en"><description text="function key F8"                     /></language><language code="it"><description text="Tasto funzione F8"                        /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F9", OIS::KC_F9)); /** <symbol name="KEY_F9"          ><language code="en"><description text="function key F9"                     /></language><language code="it"><description text="Tasto funzione F9"                        /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F10", OIS::KC_F10)); /** <symbol name="KEY_F10"         ><language code="en"><description text="function key F10"                    /></language><language code="it"><description text="Tasto funzione F10"                       /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_NUMLOCK", OIS::KC_NUMLOCK)); /** <symbol name="KEY_NUMLOCK"     ><language code="en"><description text="NUM LOCK key"                        /></language><language code="it"><description text="Tasto BLOC NUM (NUM LOCK)"                /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_SCROLLOCK", OIS::KC_SCROLL)); /** <symbol name="KEY_SCROLLOCK"   ><language code="en"><description text="SCROLL LOCK key"                     /></language><language code="it"><description text="Tasto BLOC SCORR (SCROLL LOCK)"           /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP7", OIS::KC_NUMPAD7)); /** <symbol name="KEY_KP7"         ><language code="en"><description text="KEYPAD SEVEN  ('7') key"             /></language><language code="it"><description text="Tasto NUMPAD SETTE   ('7')"               /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP8", OIS::KC_NUMPAD8)); /** <symbol name="KEY_KP8"         ><language code="en"><description text="KEYPAD EIGHT  ('8') key"             /></language><language code="it"><description text="Tasto NUMPAD OTTO    ('8')"               /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP9", OIS::KC_NUMPAD9)); /** <symbol name="KEY_KP9"         ><language code="en"><description text="KEYPAD NINE   ('9') key"             /></language><language code="it"><description text="Tasto NUMPAD NOVE    ('9')"               /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP_MINUS", OIS::KC_SUBTRACT)); /** <symbol name="KEY_KP_MINUS"    ><language code="en"><description text="KEYPAD MINUS ('-') key"              /></language><language code="it"><description text="Tasto NUMPAD MENO ('-')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP4", OIS::KC_NUMPAD4)); /** <symbol name="KEY_KP4"         ><language code="en"><description text="KEYPAD FOUR   ('4') key"             /></language><language code="it"><description text="Tasto NUMPAD QUATTRO ('4')"               /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP5", OIS::KC_NUMPAD5)); /** <symbol name="KEY_KP5"         ><language code="en"><description text="KEYPAD FIVE   ('5') key"             /></language><language code="it"><description text="Tasto NUMPAD CINQUE  ('5')"               /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP6", OIS::KC_NUMPAD6)); /** <symbol name="KEY_KP6"         ><language code="en"><description text="KEYPAD SIX    ('6') key"             /></language><language code="it"><description text="Tasto NUMPAD SEI     ('6')"               /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP_PLUS", OIS::KC_ADD)); /** <symbol name="KEY_KP_PLUS"     ><language code="en"><description text="KEYPAD PLUS ('+') key"               /></language><language code="it"><description text="Tasto NUMPAD PIU' ('+')"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP1", OIS::KC_NUMPAD1)); /** <symbol name="KEY_KP1"         ><language code="en"><description text="KEYPAD ONE    ('1') key"             /></language><language code="it"><description text="Tasto NUMPAD UNO     ('1')"               /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP2", OIS::KC_NUMPAD2)); /** <symbol name="KEY_KP2"         ><language code="en"><description text="KEYPAD TWO    ('2') key"             /></language><language code="it"><description text="Tasto NUMPAD DUE     ('2')"               /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP3", OIS::KC_NUMPAD3)); /** <symbol name="KEY_KP3"         ><language code="en"><description text="KEYPAD THREE  ('3') key"             /></language><language code="it"><description text="Tasto NUMPAD TRE     ('3')"               /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP0", OIS::KC_NUMPAD0)); /** <symbol name="KEY_KP0"         ><language code="en"><description text="KEYPAD ZERO   ('0') key"             /></language><language code="it"><description text="Tasto NUMPAD ZERO    ('0')"               /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP_PERIOD", OIS::KC_DECIMAL)); /** <symbol name="KEY_KP_PERIOD"   ><language code="en"><description text="KEYPAD PERIOD ('.') key"             /></language><language code="it"><description text="Tasto NUMPAD PUNTO   ('.')"               /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F11", OIS::KC_F11)); /** <symbol name="KEY_F11"         ><language code="en"><description text="function key F11"                    /></language><language code="it"><description text="Tasto funzione F11"                       /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F12", OIS::KC_F12)); /** <symbol name="KEY_F12"         ><language code="en"><description text="function key F12"                    /></language><language code="it"><description text="Tasto funzione F12"                       /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F13", OIS::KC_F13)); /** <symbol name="KEY_F13"         ><language code="en"><description text="function key F13"                    /></language><language code="it"><description text="Tasto funzione F13"                       /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F14", OIS::KC_F14)); /** <symbol name="KEY_F14"         ><language code="en"><description text="function key F14"                    /></language><language code="it"><description text="Tasto funzione F14"                       /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_F15", OIS::KC_F15)); /** <symbol name="KEY_F15"         ><language code="en"><description text="function key F15"                    /></language><language code="it"><description text="Tasto funzione F15"                       /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP_EQUALS", OIS::KC_NUMPADEQUALS)); /** <symbol name="KEY_KP_EQUALS"   ><language code="en"><description text="KEYPAD EQUALS key"                   /></language><language code="it"><description text="Tasto NUMPAD UGUALE ('=')"                /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_PREVTRACK", OIS::KC_PREVTRACK)); /** <symbol name="KEY_PREVTRACK"   ><language code="en"><description text="Media PREVTRACK key"                 /></language><language code="it"><description text="Tasto Media PREVTRACK"                    /></language></symbol> */
        //keysymbols.push_back( new KeyValue("KEY_AT",           OIS::KC_AT));          /** <symbol name="KEY_AT"          ><language code="en"><description text="AT key"                   /></language><language code="it"><description text="Tasto NUMPAD UGUALE ('=')"                /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_COLON", OIS::KC_COLON)); /** <symbol name="KEY_COLON"       ><language code="en"><description text="COLON (':') character key"           /></language><language code="it"><description text="Tasto carattere DUE PUNTI (':')"          /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_UNDERSCORE", OIS::KC_UNDERLINE)); /** <symbol name="KEY_UNDERSCORE"  ><language code="en"><description text="UNDERSCORE ('_') character key"      /></language><language code="it"><description text="Tasto carattere UNDERSCORE ('_')"         /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_STOP", OIS::KC_STOP)); /** <symbol name="KEY_STOP"        ><language code="en"><description text="STOP key"                            /></language><language code="it"><description text="Tasto STOP"                               /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_NEXTTRACK", OIS::KC_NEXTTRACK)); /** <symbol name="KEY_NEXTTRACK"   ><language code="en"><description text="Media NEXTTRACK key"                 /></language><language code="it"><description text="Tasto Media NEXTTRACK"                    /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP_ENTER", OIS::KC_NUMPADENTER)); /** <symbol name="KEY_KP_ENTER"    ><language code="en"><description text="KEYPAD ENTER key"                    /></language><language code="it"><description text="Tasto NUMPAD ENTER"                       /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_RCTRL", OIS::KC_RCONTROL)); /** <symbol name="KEY_RCTRL"       ><language code="en"><description text="RIGHT CTRL key"                      /></language><language code="it"><description text="Tasto CTRL Destro"                        /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_MUTE", OIS::KC_MUTE)); /** <symbol name="KEY_MUTE"        ><language code="en"><description text="Media MUTE key"                      /></language><language code="it"><description text="Tasto Media MUTE"                         /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_CALCULATOR", OIS::KC_CALCULATOR)); /** <symbol name="KEY_CALCULATOR"  ><language code="en"><description text="Quicklaunch CALCULATOR key"          /></language><language code="it"><description text="Tasto avvio veloce CALCULATOR"            /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_PLAYPAUSE", OIS::KC_PLAYPAUSE)); /** <symbol name="KEY_PLAYPAUSE"   ><language code="en"><description text="Media PLAY/PAUSE key"                /></language><language code="it"><description text="Tasto media PLAY/PAUSE"                   /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_MEDIASTOP", OIS::KC_MEDIASTOP)); /** <symbol name="KEY_MEDIASTOP"   ><language code="en"><description text="Media STOP key"                      /></language><language code="it"><description text="Tasto Media STOP"                         /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_VOLUMEDOWN", OIS::KC_VOLUMEDOWN)); /** <symbol name="KEY_VOLUMEDOWN"  ><language code="en"><description text="Media VOLUME DOWN key"               /></language><language code="it"><description text="Tasto Media VOLUME DOWN"                  /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_VOLUMEUP", OIS::KC_VOLUMEUP)); /** <symbol name="KEY_VOLUMEUP"    ><language code="en"><description text="Media VOLUME UP key"                 /></language><language code="it"><description text="Tasto Media VOLUME UP"                    /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_WEBHOME", OIS::KC_WEBHOME)); /** <symbol name="KEY_WEBHOME"     ><language code="en"><description text="Quicklaunch WEB HOME key"            /></language><language code="it"><description text="Tasto avvio veloce WEB HOME"              /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP_COMMA", OIS::KC_NUMPADCOMMA)); /** <symbol name="KEY_KP_COMMA"    ><language code="en"><description text="KEYPAD COMMA (',') key"              /></language><language code="it"><description text="Tasto NUMPAD VIRGOLA (',')"               /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_KP_DIVIDE", OIS::KC_DIVIDE)); /** <symbol name="KEY_KP_DIVIDE"   ><language code="en"><description text="KEYPAD DIVIDE ('/') key"             /></language><language code="it"><description text="Tasto NUMPAD DIVISIONE ('/')"             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_SYSREQ", OIS::KC_SYSRQ)); /** <symbol name="KEY_SYSREQ"      ><language code="en"><description text="SYSREQ key"                          /></language><language code="it"><description text="Tasto R SIST (SYSREQ)"                    /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_RALT", OIS::KC_RMENU)); /** <symbol name="KEY_RALT"        ><language code="en"><description text="RIGHT ALT key"                       /></language><language code="it"><description text="Tasto ALT Destro"                         /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_PAUSE", OIS::KC_PAUSE)); /** <symbol name="KEY_PAUSE"       ><language code="en"><description text="PAUSE key"                           /></language><language code="it"><description text="Tasto PAUSA"                              /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_HOME", OIS::KC_HOME)); /** <symbol name="KEY_HOME"        ><language code="en"><description text="HOME key"                            /></language><language code="it"><description text="Tasto HOME"                               /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_UP", OIS::KC_UP)); /** <symbol name="KEY_UP"          ><language code="en"><description text="cursor UP key"                       /></language><language code="it"><description text="Tasto cursore SU"                         /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_PAGEUP", OIS::KC_PGUP)); /** <symbol name="KEY_PAGEUP"      ><language code="en"><description text="PAGE UP key"                         /></language><language code="it"><description text="Tasto PAG SU"                             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_LEFT", OIS::KC_LEFT)); /** <symbol name="KEY_LEFT"        ><language code="en"><description text="cursor LEFT key"                     /></language><language code="it"><description text="Tasto cursore SINISTRA"                   /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_RIGHT", OIS::KC_RIGHT)); /** <symbol name="KEY_RIGHT"       ><language code="en"><description text="cursor RIGHT key"                    /></language><language code="it"><description text="Tasto cursore DESTRA"                     /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_END", OIS::KC_END)); /** <symbol name="KEY_END"         ><language code="en"><description text="END key"                             /></language><language code="it"><description text="Tasto FINE (END)"                         /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_DOWN", OIS::KC_DOWN)); /** <symbol name="KEY_DOWN"        ><language code="en"><description text="cursor DOWN key"                     /></language><language code="it"><description text="Tasto cursore GIU'"                       /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_PAGEDOWN", OIS::KC_PGDOWN)); /** <symbol name="KEY_PAGEDOWN"    ><language code="en"><description text="PAGE DOWN key"                       /></language><language code="it"><description text="Tasto PAG GIU'"                           /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_INSERT", OIS::KC_INSERT)); /** <symbol name="KEY_INSERT"      ><language code="en"><description text="INSERT key"                          /></language><language code="it"><description text="Tasto INSERT"                             /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_DELETE", OIS::KC_DELETE)); /** <symbol name="KEY_DELETE"      ><language code="en"><description text="DELETE key"                          /></language><language code="it"><description text="Tasto CANC (DELETE)"                      /></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_LMETA", OIS::KC_LWIN)); /** <symbol name="KEY_LMETA"       ><language code="en"><description text="LEFT META (formerly Windows) key"    /></language><language code="it"><description text="Tasto META (comunemente Windows) SINISTRO"/></language></symbol> */
        keysymbols.push_back(new KeyValue("KEY_RMETA", OIS::KC_RWIN)); /** <symbol name="KEY_RMETA"       ><language code="en"><description text="RIGHT META (formerly Windows) key"   /></language><language code="it"><description text="Tasto META (comunemente Windows) DESTRO"  /></language></symbol> */

        /* JAMMA ADAPTER symbols (OLD):
        keysymbols.push_back( new KeyValue("JAMMA_TEST",       OIS::KC_RETURN));
        keysymbols.push_back( new KeyValue("JAMMA_COIN",       OIS::KC_5));     
        keysymbols.push_back( new KeyValue("JAMMA_START_1",    OIS::KC_1));     
        keysymbols.push_back( new KeyValue("JAMMA_1P_UP",      OIS::KC_UP));   
        keysymbols.push_back( new KeyValue("JAMMA_1P_DOWN",    OIS::KC_DOWN));
        keysymbols.push_back( new KeyValue("JAMMA_1P_LEFT",    OIS::KC_LEFT)); 
        keysymbols.push_back( new KeyValue("JAMMA_1P_RIGHT",   OIS::KC_RIGHT));
        keysymbols.push_back( new KeyValue("JAMMA_1P_B1",      OIS::KC_LCONTROL));
        keysymbols.push_back( new KeyValue("JAMMA_1P_B2",      OIS::KC_LMENU));   
        keysymbols.push_back( new KeyValue("JAMMA_1P_B3",      OIS::KC_LSHIFT));  
        keysymbols.push_back( new KeyValue("JAMMA_1P_B4",      OIS::KC_SPACE));   
        keysymbols.push_back( new KeyValue("JAMMA_MENU",       OIS::KC_ESCAPE));  
        keysymbols.push_back( new KeyValue("JAMMA_START_2",    OIS::KC_2));       
        keysymbols.push_back( new KeyValue("JAMMA_2P_UP",      OIS::KC_R));       
        keysymbols.push_back( new KeyValue("JAMMA_2P_DOWN",    OIS::KC_F));       
        keysymbols.push_back( new KeyValue("JAMMA_2P_LEFT",    OIS::KC_D));       
        keysymbols.push_back( new KeyValue("JAMMA_2P_RIGHT",   OIS::KC_G));       
        keysymbols.push_back( new KeyValue("JAMMA_2P_B1",      OIS::KC_A));       
        keysymbols.push_back( new KeyValue("JAMMA_2P_B2",      OIS::KC_S));       
        keysymbols.push_back( new KeyValue("JAMMA_2P_B3",      OIS::KC_Q));       
        keysymbols.push_back( new KeyValue("JAMMA_2P_B4",      OIS::KC_W));       
         */

        // JAMMA ADAPTER symbols:
        keysymbols.push_back(new KeyValue("JAMMA_TEST", OIS::KC_T)); /** <symbol name="JAMMA_TEST"    ><language code="en"><description text="JAMMA TEST signal (equivalent to key T)"   /></language><language code="it"><description text="Segnale JAMMA TEST (equivalente al tasto T)"   /></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_SERVICE", OIS::KC_U)); /** <symbol name="JAMMA_SERVICE" ><language code="en"><description text="JAMMA SERVICE Signal (equivalent to key U)"/></language><language code="it"><description text="Segnale JAMMA SERVICE (equivalente al tasto U)"/></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_TILT", OIS::KC_Y)); /** <symbol name="JAMMA_TILT"    ><language code="en"><description text="JAMMA TILT Signal (equivalent to key Y)"   /></language><language code="it"><description text="Segnale JAMMA TILT (equivalente al tasto Y)"   /></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_COIN", OIS::KC_5)); /** <symbol name="JAMMA_COIN"    ><language code="en"><description text="JAMMA COIN signal (equivalent to key 5)"   /></language><language code="it"><description text="Segnale JAMMA COIN (equivalente al tasto 5)"   /></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_START_1", OIS::KC_1)); /** <symbol name="JAMMA_START_1" ><language code="en"><description text="JAMMA Player 1 START signal (equivalent to key 1)"  /></language><language code="it"><description text="Segnale JAMMA START Giocatore 1 (equivalente al tasto 1)"/></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_START_2", OIS::KC_2)); /** <symbol name="JAMMA_START_2" ><language code="en"><description text="JAMMA Player 2 START signal (equivalent to key 2)"  /></language><language code="it"><description text="Segnale JAMMA START Giocatore 2 (equivalente al tasto 2)"/></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_1P_UP", OIS::KC_UP)); /** <symbol name="JAMMA_1P_UP"   ><language code="en"><description text="JAMMA Player 1 Joystick UP (equivalent to key CURSOR UP)"   /></language><language code="it"><description text="Segnale JAMMA Joystick SU Giocatore 1 (equivalente al tasto CURSOR UP)"      /></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_1P_DOWN", OIS::KC_DOWN)); /** <symbol name="JAMMA_1P_DOWN" ><language code="en"><description text="JAMMA Player 1 Joystick DOWN (equivalent to key CURSOR DOWN)" /></language><language code="it"><description text="Segnale JAMMA Joystick GIU Giocatore 1 (equivalente al tasto CURSOR DOWN)"     /></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_1P_LEFT", OIS::KC_LEFT)); /** <symbol name="JAMMA_1P_LEFT" ><language code="en"><description text="JAMMA Player 1 Joystick LEFT (equivalent to key CURSOR LEFT)" /></language><language code="it"><description text="Segnale JAMMA Joystick SINISTRA Giocatore 1 (equivalente al tasto CURSOR LEFT)"/></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_1P_RIGHT", OIS::KC_RIGHT)); /** <symbol name="JAMMA_1P_RIGHT"><language code="en"><description text="JAMMA Player 1 Joystick RIGHT (equivalent to key CURSOR RIGHT)"/></language><language code="it"><description text="Segnale JAMMA Joystick DESTRO Giocatore 1 (equivalente al tasto CURSOR RIGHT)"  /></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_1P_B1", OIS::KC_Z)); /** <symbol name="JAMMA_1P_B1"   ><language code="en"><description text="JAMMA Player 1 BUTTON 1 (equivalent to key Z)" /></language><language code="it"><description text="Segnale JAMMA BOTTONE 1 Giocatore 1 (equivalente al tasto Z)"/></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_1P_B2", OIS::KC_X)); /** <symbol name="JAMMA_1P_B2"   ><language code="en"><description text="JAMMA Player 1 BUTTON 2 (equivalent to key X)" /></language><language code="it"><description text="Segnale JAMMA BOTTONE 2 Giocatore 1 (equivalente al tasto X)"/></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_1P_B3", OIS::KC_C)); /** <symbol name="JAMMA_1P_B3"   ><language code="en"><description text="JAMMA Player 1 BUTTON 3 (equivalent to key C)" /></language><language code="it"><description text="Segnale JAMMA BOTTONE 3 Giocatore 1 (equivalente al tasto C)"/></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_1P_B4", OIS::KC_V)); /** <symbol name="JAMMA_1P_B4"   ><language code="en"><description text="JAMMA Player 1 BUTTON 4 (equivalent to key V)" /></language><language code="it"><description text="Segnale JAMMA BOTTONE 4 Giocatore 1 (equivalente al tasto V)"/></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_1P_B5", OIS::KC_B)); /** <symbol name="JAMMA_1P_B5"   ><language code="en"><description text="JAMMA Player 1 BUTTON 5 (equivalent to key B)" /></language><language code="it"><description text="Segnale JAMMA BOTTONE 5 Giocatore 1 (equivalente al tasto B)"/></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_2P_UP", OIS::KC_R)); /** <symbol name="JAMMA_2P_UP"   ><language code="en"><description text="JAMMA Player 2 Joystick UP (equivalent to key R)"    /></language><language code="it"><description text="Segnale JAMMA Joystick SU Giocatore 2 (equivalente al tasto R)"      /></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_2P_DOWN", OIS::KC_F)); /** <symbol name="JAMMA_2P_DOWN" ><language code="en"><description text="JAMMA Player 2 Joystick DOWN (equivalent to key F)"  /></language><language code="it"><description text="Segnale JAMMA Joystick GIU Giocatore 2 (equivalente al tasto F)"     /></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_2P_LEFT", OIS::KC_D)); /** <symbol name="JAMMA_2P_LEFT" ><language code="en"><description text="JAMMA Player 2 Joystick LEFT (equivalent to key D)"  /></language><language code="it"><description text="Segnale JAMMA Joystick SINISTRA Giocatore 2 (equivalente al tasto D)"/></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_2P_RIGHT", OIS::KC_G)); /** <symbol name="JAMMA_2P_RIGHT"><language code="en"><description text="JAMMA Player 2 Joystick RIGHT (equivalent to key G)" /></language><language code="it"><description text="Segnale JAMMA Joystick DESTRO Giocatore 2 (equivalente al tasto G)"  /></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_2P_B1", OIS::KC_A)); /** <symbol name="JAMMA_2P_B1"   ><language code="en"><description text="JAMMA Player 2 BUTTON 1 (equivalent to key A)"       /></language><language code="it"><description text="Segnale JAMMA BOTTONE 1 Giocatore 2 (equivalente al tasto A)" /></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_2P_B2", OIS::KC_S)); /** <symbol name="JAMMA_2P_B2"   ><language code="en"><description text="JAMMA Player 2 BUTTON 2 (equivalent to key S)"       /></language><language code="it"><description text="Segnale JAMMA BOTTONE 2 Giocatore 2 (equivalente al tasto S)" /></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_2P_B3", OIS::KC_Q)); /** <symbol name="JAMMA_2P_B3"   ><language code="en"><description text="JAMMA Player 2 BUTTON 3 (equivalent to key Q)"       /></language><language code="it"><description text="Segnale JAMMA BOTTONE 3 Giocatore 2 (equivalente al tasto Q)" /></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_2P_B4", OIS::KC_W)); /** <symbol name="JAMMA_2P_B4"   ><language code="en"><description text="JAMMA Player 2 BUTTON 4 (equivalent to key W)"       /></language><language code="it"><description text="Segnale JAMMA BOTTONE 4 Giocatore 2 (equivalente al tasto W)" /></language></symbol> */
        keysymbols.push_back(new KeyValue("JAMMA_2P_B5", OIS::KC_E)); /** <symbol name="JAMMA_2P_B5"   ><language code="en"><description text="JAMMA Player 2 BUTTON 5 (equivalent to key E)"       /></language><language code="it"><description text="Segnale JAMMA BOTTONE 5 Giocatore 2 (equivalente al tasto E)" /></language></symbol> */

        //
        char tmpstr[120];
        int totalsymbols = keysymbols.size();
        for (int i = 0; i < totalsymbols; i++)
            {
            sprintf(tmpstr, "eos."_MODULE_NAME".%s = %d", keysymbols[i]->key, keysymbols[i]->value);
            EOSLuaRuntime::getInstance()->executeLuaChunk(tmpstr);
            delete keysymbols[i];
            }
        keysymbols.clear();
        }


    // ======================== LUA FUNCTIONS REGISTRATION =========================

    // ==== Here we declare an array with all functions and their respective names.
    // ==== This array has elements of type luaL_reg, which is a structure with
    // ==== two fields: a string and a function pointer.

    const struct luaL_reg fn_table_module [] = {
        {"cleanup", moduleCleanup},
        {NULL, NULL} // <-- end-of-table sentinel
        };

    const struct luaL_reg fn_table_main [] = {
        {"attachToWindow", attachToWindow},
        {"detach", detach},
        {"update", update},
        {"mouseGetCoords", mouseGetCoords},
        {"mouseGetRelativeCoords", mouseGetRelativeCoords},
        {"keyState", keyState},
        {"mouseKeyState", mouseKeyState},
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

        // registers "eos.oisinput" functions
        luaL_register(luaRuntime, "eos."_MODULE_NAME, fn_table_main);

        // key symbols loading:
        loadKeysymSymbols();
        //
        //
        return 1;
        }

    // -----------------------------------------------------------------------------

    } // extern "C" closure

/**
        </module>
 */

/*
v. 0.1.0
        Mar 16, 2007:
                .first development attempt

 */
