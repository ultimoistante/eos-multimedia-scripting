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

#ifndef EOSSOUND_H
#define EOSSOUND_H

#include <EOSLuaRuntime.h>
#include "EOSResourceManager.h"
#include "EOSAudioDefines.h"

#include <AL/al.h>
#include <AL/alc.h>

class EOSSound
{
public:
    EOSSound(char *filename);
    virtual ~EOSSound();

	virtual void play(float gain, bool isRelative, bool loop = false);
	virtual void replay();
	virtual void crossLap(EOSSound *sound);
	virtual void pause();
	virtual void unpause();
	virtual void mute();
	virtual void unmute();
	virtual void stop();
	virtual void setPosition(float px, float py, float pz);
	virtual void translate(float px, float py, float pz, float vx, float vy, float vz);
	virtual void setPitch(float pitch);
	virtual void setGain(float gain);
	virtual void flush();

	virtual bool isPlaying();
	virtual bool isLooping();
	virtual bool isMuted();
	virtual bool isLoaded();

    ///

    virtual void *getStream();
    virtual unsigned int getSource();
    virtual float *getPos();
    
    ///
    
    virtual       void  setID(int id) { this->id = id; }
    virtual       int   getID()       { return this->id; }
    virtual const char* getFileName() { return this->fileName.c_str(); }

protected:
    float   _pos[3],
            _vel[3],
            _pitch;

    bool    _loop,
            _loaded;

    ALuint  _src;

    // errors
	bool checkErrors(char *funcname);

private:
    ALfloat _preMuteGain;
    // ------- added by jadlazarus
    int         id;
    std::string fileName;
    // ---------------------------
    
};

#endif
