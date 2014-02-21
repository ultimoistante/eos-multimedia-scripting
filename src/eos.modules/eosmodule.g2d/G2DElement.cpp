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


#include "G2DElement.h"

// -----------------------------------------------------------------------------

G2DElement::G2DElement()
    {
    zOrder = 0;
    }

// -----------------------------------------------------------------------------

G2DElement::~G2DElement()
    {
    std::map<std::string, std::string>::iterator it;
    for(it=properties.begin(); it!=properties.end(); it++)
        {
    //    delete it->first;
    //    delete it->second;
        }
    properties.clear();
    }

// -----------------------------------------------------------------------------

void G2DElement::paint()
    {
    }

// -----------------------------------------------------------------------------

void G2DElement::setProperty(const char *name, const char *value)
    {
    if(name != NULL && value != NULL)
        {
        std::map<std::string, std::string>::iterator it;
		it = properties.find(std::string(name));
        if(it != properties.end()) // name found
            (*it).second = value;
        else
            properties.insert(std::pair<std::string, std::string>(name, value));
        }
    }

// -----------------------------------------------------------------------------

char* G2DElement::getProperty(const char *name)
    {
    char *value = NULL;
	if(name != NULL)
        {
		std::map<std::string, std::string>::iterator it;
		it = properties.find(std::string(name));
		if(it != properties.end()) // name found
			value = (char *)(*it).second.c_str();
        }
	return value;
    }

// -----------------------------------------------------------------------------
