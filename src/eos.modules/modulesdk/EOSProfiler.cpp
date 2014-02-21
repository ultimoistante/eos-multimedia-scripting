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


#include "EOSProfiler.h"

// -----------------------------------------------------------------------------

EOSProfiler::EOSProfiler(const char* moduleName)
    {
    this->moduleName = moduleName;
    //
    timer = new EOSPrecisionTimer();
    timer->reset();
    }

// -----------------------------------------------------------------------------

EOSProfiler::~EOSProfiler()
    {
    printReport();

    // delete timings
    std::map<const char*, KernelProfilingData*>::iterator it;
    for(it = timings.begin(); it != timings.end(); ++it)
        delete it->second;
    timings.clear();

    // delete timer
    if(timer)
        delete timer;
    }

// -----------------------------------------------------------------------------

void EOSProfiler::beginProfiling(const char *funcName)
    {    
    KernelProfilingData *kpd = NULL;
    std::map<const char*, KernelProfilingData*>::iterator it;
    it = timings.find(funcName);
    if(it != timings.end())
        kpd = it->second;
    else
        {
        kpd = new KernelProfilingData;
        if(kpd != NULL)
            timings.insert(std::pair<const char*, KernelProfilingData*>(funcName, kpd));
        }
    kpd->startTime = timer->getSeconds();
    }

// -----------------------------------------------------------------------------

void EOSProfiler::endProfiling(const char *funcName)
    {
    std::map<const char*, KernelProfilingData*>::iterator it;
    it = timings.find(funcName);
    if(it != timings.end())
        {
        KernelProfilingData *kpd = it->second;
        kpd->endTime = timer->getSeconds();
        kpd->lastTime = kpd->endTime - kpd->startTime;
        kpd->totalTime += kpd->lastTime;
        kpd->iterations++;
        }
    }

// -----------------------------------------------------------------------------

void EOSProfiler::printReport()
    {
    printf("\nEOS Profiler - Report of module: %s - profiled: %i elements \n", moduleName, timings.size());
    //
    KernelProfilingData *kpd = NULL;
    std::map<const char*, KernelProfilingData*>::iterator it;

    for(it = timings.begin(); it != timings.end(); it++)
        {
        kpd = it->second;
        printf("[%s]  last: %.3fms  total %.3fms  average %.3fms\n", it->first, kpd->lastTime*1000.0f, kpd->totalTime*1000.0f, (kpd->totalTime/kpd->iterations)*1000.0f);
        }

    printf("\n");
    }
