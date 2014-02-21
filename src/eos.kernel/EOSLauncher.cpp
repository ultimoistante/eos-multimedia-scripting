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
#include "EOSKernel.h"

#include <iostream>
using namespace std;

/** executable entry point
 */

int main(int argc, char** argv)
    {
    EOSKernel::getInstance()->setExecParamsCount(&argc);
    EOSKernel::getInstance()->setExecParamsValues(argv);
    //
    for (int i = 1; i < argc; i++)
        {
        if (argv[i][0] == '-') // commands
            {
            if (strcmp(argv[i], "--enable-ctrl-streams") == 0)
                EOSKernel::getInstance()->enableControlStreams();
            //
            if (strcmp(argv[i], "--enable-debug-socket") == 0)
                EOSKernel::getInstance()->enableDebugSocket();
            //
            if (strcmp(argv[i], "--norun") == 0)
                EOSKernel::getInstance()->enableJustCompileScriptsMode();
            }
        else
            EOSKernel::getInstance()->setApplicationPath(argv[i]);
        }
    //
    EOSKernel::getInstance()->start();
    //
    return 0;
    }

