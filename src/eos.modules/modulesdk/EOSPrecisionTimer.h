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

#ifndef EOS_EOSPRECISIONTIMER_H
#define EOS_EOSPRECISIONTIMER_H

#include <stdio.h>
#include <time.h>

// removed all obscene windows code (bluarghh...)
// sincerely, i have no time to port EOS in windows...
// anyway, who cares? wincowz sucks...

/*#if defined WIN32
	#ifndef WIN32_LEAN_AND_MEAN
	#  define WIN32_LEAN_AND_MEAN
	#endif
	#include "windows.h"
	//Get around Windows hackery
	#ifdef max
	#  undef max
	#endif
	#ifdef min
	#  undef min
	#endif
	#define FREQUENCY_RESAMPLE_RATE 200
	//#include <time.h>
#elif defined LINUX*/
	#include <sys/time.h>
//#endif


class EOSPrecisionTimer
	{
	protected:
		/*#if defined WIN32
		clock_t  zeroClock;
		DWORD    mStartTick;
		LONGLONG mLastTime;
		LARGE_INTEGER mStartTime;
		LARGE_INTEGER mFrequency;
		DWORD    mProcMask;
		DWORD    mSysMask;
		HANDLE   mThread;
		DWORD    mQueryCount;
		#elif defined LINUX*/
		struct timeval start;
		clock_t zeroClock;
		//#endif

	public:
		EOSPrecisionTimer() ;
		~EOSPrecisionTimer() { }

		// Resets timer
		void reset();

		// Returns seconds since last reset
		float getSeconds();

		// Returns milliseconds since last reset
		unsigned long getMilliseconds();

		// Returns microseconds since last reset
		unsigned long getMicroseconds();

	};

#endif
