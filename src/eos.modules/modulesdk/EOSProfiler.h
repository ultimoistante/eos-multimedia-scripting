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


#ifndef EOS_KERNEL_PROFILER_H
#define	EOS_KERNEL_PROFILER_H

#include <map>
#include "EOSPrecisionTimer.h"

// -----------------------------------------------------------------------------

typedef struct kernel_profiling_data
    {
    float         startTime,
                  endTime,
                  lastTime,
                  totalTime;

    unsigned long iterations;

    kernel_profiling_data() : startTime(0), endTime(0), lastTime(0), totalTime(0), iterations(0) {}
    } KernelProfilingData;

// -----------------------------------------------------------------------------

class EOSProfiler
    {
    public:
        EOSProfiler(const char* moduleName);
		~EOSProfiler();        

        void    beginProfiling(const char *funcName);
        void    endProfiling(const char *funcName);

    private:
        const char          *moduleName;
        EOSPrecisionTimer   *timer;
        std::map<const char*, KernelProfilingData*> timings;

        void    printReport();
    };

#endif

