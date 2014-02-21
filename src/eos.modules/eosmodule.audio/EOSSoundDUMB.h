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

#ifndef EOSSOUNDDUMB_H
#define EOSSOUNDDUMB_H

#include "EOSSound.h"
#include "tinythread.h"
#include <dumb.h>
#include <algorithm>


class EOSSoundDUMB : public EOSSound
{
public:
    EOSSoundDUMB(char *filename);
    ~EOSSoundDUMB();

	void play(float gain, bool isRelative, bool loop = false);
	void replay();
	void pause();
	void unpause();
	void stop();
	void setPosition(float px, float py, float pz);
	void translate(float px, float py, float pz, float vx, float vy, float vz);
	void setPitch(float pitch);
	void setGain(float gain);
	void flush();

	bool isPlaying();
	bool isLooping();

protected:
    bool dumbOpen(char *filename);
    void dumbRelease();
    bool dumbPlayback();
    bool dumbPlaying();
    bool dumbUpdate();

    bool dumbStream(unsigned int buffer);
    void dumbSeek(double seconds);
    void dumbEmpty();

    bool _dumbStop,
         _dumbPause,
         _dumbLoop,
         _dumbLoaded;

private:
    bool checkErrors(const char *funcname);
    static void threadRun(void *dumbSound);

    DUH             *_duh;
    DUH_SIGRENDERER *_renderer;

    float            _samplingRate,
                     _delta;
    long             _seek;

    ALuint           _buffers[BUFFER_NUM];
    ALenum           _format;

    tthread::thread *_thread;
};

#endif
