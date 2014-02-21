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


#include "EOSMessage.h"
#include "EOSKernel.h"

// -----------------------------------------------------------------------------

EOSMessage::EOSMessage(const char* messageClass)
    {
    if (messageClass != NULL)
        addValue("class", messageClass);
    this->senderName = "";
    this->receiverName = "";
    this->tmpCmdBuffer = "";
    }

// -----------------------------------------------------------------------------

EOSMessage::~EOSMessage()
    {
    int size = values.size();
    for (int i = 0; i < size; i++)
        delete values[i];
    values.clear();
    }

// -----------------------------------------------------------------------------

void EOSMessage::addValue(const char* valueKey, bool valueData)
    {
    values.push_back(new MessageField(valueKey, new VariantData(valueData)));
    }

// -----------------------------------------------------------------------------

void EOSMessage::addValue(const char* valueKey, double valueData)
    {
    values.push_back(new MessageField(valueKey, new VariantData(valueData)));
    }

// -----------------------------------------------------------------------------

void EOSMessage::addValue(const char* valueKey, const char* valueData)
    {
    values.push_back(new MessageField(valueKey, new VariantData(valueData)));
    }

// -----------------------------------------------------------------------------

void EOSMessage::toLuaMessageBoardTable(lua_State* luaRuntime)
    {
    // ultimoistante: changed on Nov 13, 2012
    EOSLuaEnvironment* luaEnv = EOSKernel::getInstance()->getLuaEnvironment();
    char cmdString[255];
    //
    // creates the new table
    this->tmpCmdBuffer = "eos.message.current = {} ";
    //
    // then browses all message's fields
    int size = values.size();
    for (int i = 0; i < size; i++)
        {
        // Pushes the cell value
        if (values[i]->value->isBoolean())
            snprintf(cmdString, 255, "eos.message.current.%s = %s ", values[i]->name.c_str(), values[i]->value->getBooleanValue() == true ?  "true" : "false");
        else if(values[i]->value->isNumeric())
            snprintf(cmdString, 255, "eos.message.current.%s = %f ", values[i]->name.c_str(), values[i]->value->getNumericValue());
        else if(values[i]->value->isString())
            snprintf(cmdString, 255, "eos.message.current.%s = \"%s\" ", values[i]->name.c_str(), values[i]->value->getStringValue());
        //
        this->tmpCmdBuffer += cmdString;
        //printf("# pushing: %s\n", cmdString);
        }
    luaEnv->runBuffer((const unsigned char*) this->tmpCmdBuffer.c_str(), this->tmpCmdBuffer.length());
    
    /*
    lua_getglobal(luaRuntime, "eos.message.current"); // ultimoistante: changed Nov 13, 2012 -  was "EOSMessage"
    if (lua_istable(luaRuntime, -1)) // table exists
        {
        lua_pop(luaRuntime, 1); // pops lua_getglobal result from the stack
        // deletes the table (simply doing table = nil)
        lua_pushnil(luaRuntime);
        lua_setglobal(luaRuntime, "eos.message.current"); // ultimoistante: changed Nov 13, 2012 -  was "EOSMessage"
        }

    // creates the new table
    lua_newtable(luaRuntime);
    // then browses all message's fields
    int size = values.size();
    for (int i = 0; i < size; i++)
        {
        lua_pushstring(luaRuntime, values[i]->name.c_str()); // Pushes the table index (a string)
        printf("pushing: %s\n", values[i]->name.c_str());
        //
        // Pushes the cell value
        if (values[i]->value->isBoolean())
            lua_pushboolean(luaRuntime, values[i]->value->getBooleanValue());
        else if (values[i]->value->isNumeric())
            lua_pushnumber(luaRuntime, values[i]->value->getNumericValue());
        else if (values[i]->value->isString())
            lua_pushstring(luaRuntime, values[i]->value->getStringValue());
        //
        // stores the pair (index, value) in the table
        lua_rawset(luaRuntime, -3);
        // actual stack: <- [stack bottom] table, index, value [top]
        }
    // the table must be terminated by a cell which is indexed by the literal "n"
    // containing the total number of elements in the table.
    lua_pushliteral(luaRuntime, "n"); // Pushes the literal "n"
    lua_pushnumber(luaRuntime, values.size()); // Pushes the total number of cells inserted
    lua_rawset(luaRuntime, -3); // Stores the pair ("n", <totalvalues>) in the table

    // By which name is the script going to reference our table?
    lua_setglobal(luaRuntime, "eos.message.current"); // ultimoistante: changed Nov 13, 2012 -  was "EOSMessage"
    */
    }

// -----------------------------------------------------------------------------

