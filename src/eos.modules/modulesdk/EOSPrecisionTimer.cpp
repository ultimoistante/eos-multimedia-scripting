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

#include "EOSPrecisionTimer.h"

// -----------------------------------------------------------------------------

EOSPrecisionTimer::EOSPrecisionTimer()
	{
	reset();
	}

// -----------------------------------------------------------------------------

void EOSPrecisionTimer::reset()
	{
	/*#if defined WIN32
	//
	zeroClock = clock();

	QueryPerformanceFrequency(&mFrequency);
	QueryPerformanceCounter(&mStartTime);
	mStartTick = GetTickCount();
	mLastTime = 0;
	mQueryCount = 0;

	// Save the current process
	HANDLE mProc = GetCurrentProcess();

	// Get the current Affinity
	#if _MSC_VER >= 1400 && defined (_M_X64)
	GetProcessAffinityMask(mProc, (PDWORD_PTR)&mProcMask, (PDWORD_PTR)&mSysMask);
	#else
	GetProcessAffinityMask(mProc, &mProcMask, &mSysMask);
	#endif

	mThread = GetCurrentThread();
	//
	#elif defined LINUX*/
	//
	zeroClock = clock();
	gettimeofday(&start, NULL);
	//
	//#endif
	}

// -----------------------------------------------------------------------------

float EOSPrecisionTimer::getSeconds()
	{
	/*#if defined WIN32
	//
	return (float)getMilliseconds() / 1000.0f;
	//
	#elif defined LINUX*/
	//
	struct timeval now;
	gettimeofday(&now, NULL);
	return (float)((now.tv_sec-start.tv_sec) + (now.tv_usec-start.tv_usec)/1000000.0f);
	//
	//#endif
	}

// -----------------------------------------------------------------------------

unsigned long EOSPrecisionTimer::getMilliseconds()
	{
	/*#if defined WIN32
	//
	LARGE_INTEGER curTime;
	// Set affinity to the first core
	SetThreadAffinityMask(mThread, 1);
	// Query the timer
	QueryPerformanceCounter(&curTime);
	// Reset affinity
	SetThreadAffinityMask(mThread, mProcMask);
	// Resample the frequency
	mQueryCount++;
	if(mQueryCount == FREQUENCY_RESAMPLE_RATE)
		{
		mQueryCount = 0;
		QueryPerformanceFrequency(&mFrequency);
		}
	LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;
	// scale by 1000 for milliseconds
	unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
	// detect and compensate for performance counter leaps
	// (surprisingly common, see Microsoft KB: Q274323)
	unsigned long check = GetTickCount() - mStartTick;
	signed long msecOff = (signed long)(newTicks - check);
	if (msecOff < -100 || msecOff > 100)
		{
		// We must keep the timer running forward :)
		LONGLONG adjust = (msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
		mStartTime.QuadPart += adjust;
		newTime -= adjust;
		// Re-calculate milliseconds
		newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
		}
	// Record last time for adjust
	mLastTime = newTime;
	//
	return newTicks;
	//
	#elif defined LINUX*/
	//
	struct timeval now;
	gettimeofday(&now, NULL);
	return (now.tv_sec-start.tv_sec)*1000+(now.tv_usec-start.tv_usec)/1000;
	//
	//#endif
	}

// -----------------------------------------------------------------------------

unsigned long EOSPrecisionTimer::getMicroseconds()
	{
	/*#if defined WIN32
	//
	LARGE_INTEGER curTime;
	QueryPerformanceCounter(&curTime);
	LONGLONG newTime = curTime.QuadPart - mStartTime.QuadPart;
	// get milliseconds to check against GetTickCount
	unsigned long newTicks = (unsigned long) (1000 * newTime / mFrequency.QuadPart);
	// detect and compensate for performance counter leaps
	// (surprisingly common, see Microsoft KB: Q274323)
	unsigned long check = GetTickCount() - mStartTick;
	signed long msecOff = (signed long)(newTicks - check);
	if (msecOff < -100 || msecOff > 100)
		{
		// We must keep the timer running forward :)
		LONGLONG adjust = (msecOff * mFrequency.QuadPart / 1000, newTime - mLastTime);
		mStartTime.QuadPart += adjust;
		newTime -= adjust;
		}
	// Record last time for adjust
	mLastTime = newTime;
	// scale by 1000000 for microseconds
	unsigned long newMicro = (unsigned long) (1000000 * newTime / mFrequency.QuadPart);
	//
	return newMicro;
	//
	#elif defined LINUX*/
	//
	struct timeval now;
	gettimeofday(&now, NULL);
	return (now.tv_sec-start.tv_sec)*1000000+(now.tv_usec-start.tv_usec);
	//
	//#endif
	}

// -----------------------------------------------------------------------------
