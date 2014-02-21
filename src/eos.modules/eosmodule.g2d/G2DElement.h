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

#ifndef PICASSOELEMENT_H
#define	PICASSOELEMENT_H

#include <string>
#include <map>
#include <vector>
#include <algorithm>

class G2DElement
    {
    public:
        G2DElement();
        virtual ~G2DElement();

        virtual void paint();

        void setProperty(const char *name, const char *value);
        char* getProperty(const char *name);

        void setZOrder(int zOrder) { this->zOrder = zOrder; }
        int getZOrder() const      { return this->zOrder; }

    private:
        std::map<std::string, std::string> properties;
        int     zOrder;
    };

class G2DElementCompareAscending
    {
    public:
        bool operator()(const G2DElement *e1, const G2DElement *e2)
            {
            return (e1->getZOrder() < e2->getZOrder());
            }
    };

class G2DElementCompareDescending
    {
    public:
        bool operator()(const G2DElement *e1, const G2DElement *e2)
            {
            return (e1->getZOrder() > e2->getZOrder());
            }
    };


#endif

