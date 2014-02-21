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

#ifndef EOS_KERNEL_EOSMESSAGE_H
#define EOS_KERNEL_EOSMESSAGE_H

#include "VariantData.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
}

#include <vector>

/** This class represents a single field of an EOS Message Structure
 */
class MessageField
    {
    public:
        MessageField(const char* fieldname, VariantData* vdata)
            {
            this->name = (fieldname != NULL) ? fieldname : "";
            this->value = vdata;
            }
        ~MessageField()
            {
            if(value != NULL)
                delete value;
            }
    std::string name;   ///< name of the field
    VariantData* value; ///< value of the field
    };


/** This class models the EOS Message Structure
 */
class EOSMessage
    {
    public:
        /// constructor
        EOSMessage(const char* messageClass);
        /// destructor
        ~EOSMessage();
        //
        /// pushes a boolean value field in the message structure
        void addValue(const char* valueKey, bool valueData);
        /// pushes a numeric value field in the message structure
        void addValue(const char* valueKey, double valueData);
        /// pushes a string value field in the message structure
        void addValue(const char* valueKey, const char* valueData);
        //
        /// loads all message fields in the Lua runtime EOSMessage global table
        void toLuaMessageBoardTable(lua_State* luaRuntime);
        //
        /// sets sender process name
        void setSender  (const char* sender)   { this->senderName = (sender != NULL) ? sender : ""; }
        /// sets receiver process name
        void setReceiver(const char* receiver) { this->receiverName = (receiver != NULL) ? receiver : ""; }
        /// returns sender process name
        std::string& getSender()   { return this->senderName; }
        /// returns receiver process name
        std::string& getReceiver() { return this->receiverName; }
        //
    private:
        std::vector<MessageField*> values; ///< message fields vector
        std::string senderName;            ///< this string holds sender name
        std::string receiverName;          ///< this string holds receiver name
	  std::string tmpCmdBuffer;
    };



#endif
