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

#ifndef EOSSOUNDOGG_H
#define EOSSOUNDOGG_H

#include "EOSSound.h"
#include "tinythread.h"

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

#include <iostream>
#include <time.h>
#include <algorithm>


class EOSSoundOGG : public EOSSound
{
public:
    EOSSoundOGG(char *filename);
    ~EOSSoundOGG();

	void play(float gain, bool isRelative, bool loop = false);
	void replay();
	void crossLap(EOSSound *sound);
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

	void *getStream();

protected:
    bool oggOpen(char *filename);
    void oggRelease();
    void oggDisplay();
    bool oggPlayback();
    bool oggPlaying();
    bool oggUpdate();

    bool oggStream(unsigned int buffer);
    void oggSeek(double seconds);
    void oggEmpty();
    const char *oggErrorString(int code);

    bool _oggStop,
         _oggPause,
         _oggLoop,
         _oggLoaded;

private:
    bool load(char *filename);
    bool checkErrors(const char *funcname);
    static void threadRun(void *oggSound);

    FILE               *_oggFile;
    OggVorbis_File      _oggStream;
    OggVorbis_File     *_oggStreamToCross;
    vorbis_info        *_vorbisInfo;
    vorbis_comment     *_vorbisComment;

    ALuint              _buffers[BUFFER_NUM];
    ALenum              _format;

    tthread::thread    *_thread;
};

#endif
