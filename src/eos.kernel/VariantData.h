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

#ifndef EOS_KERNEL_VARIANTDATA_H
#define EOS_KERNEL_VARIANTDATA_H

#include <string>

#define VARIANT_TYPE_BOOLEAN    1
#define VARIANT_TYPE_NUMERIC    2
#define VARIANT_TYPE_STRING     3

/** Class used to handle Lua Variant (boolean/numeric/string) data in EOS Kernel
 */

class VariantData
    {
    public:
        VariantData(bool value)        { setValue(value); }
        VariantData(double value)      { setValue(value); }
        VariantData(const char* value) { setValue(value); }
        //
        void setValue(bool value)        { this->booleanValue = value; this->type = VARIANT_TYPE_BOOLEAN; }
        void setValue(double value)      { this->numericValue = value; this->type = VARIANT_TYPE_NUMERIC; }
        void setValue(const char* value) { this->stringValue  = value; this->type = VARIANT_TYPE_STRING; }
        //
        bool isBoolean() { return this->type == VARIANT_TYPE_BOOLEAN; }
        bool isNumeric() { return this->type == VARIANT_TYPE_NUMERIC; }
        bool isString()  { return this->type == VARIANT_TYPE_STRING; }
        //
        bool        getBooleanValue() { return this->booleanValue; }
        double      getNumericValue() { return this->numericValue; }
        const char* getStringValue()  { return this->stringValue.c_str(); }
        //
    private:
        bool        booleanValue;  // type VARIANT_TYPE_BOOLEAN
        double      numericValue;  // type VARIANT_TYPE_NUMERIC
        std::string stringValue;   // type VARIANT_TYPE_STRING
        //
        char type;
    };

#endif

