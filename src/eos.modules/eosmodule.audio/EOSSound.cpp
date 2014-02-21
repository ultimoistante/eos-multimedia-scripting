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


#include "EOSSound.h"

EOSSound::EOSSound(char *filename)
	{
    _pos[0] = 0;
    _pos[1] = 0;
    _pos[2] = 0;
    _vel[0] = 0;
    _vel[1] = 0;
    _vel[2] = 0;
    _pitch = 1;
    _loop = false;
    _loaded = false;
    _src = SOURCE_NONE;
    _preMuteGain = 0;
    // ------- added by jadlazarus
    id = -1; 
    fileName = filename;
    // ---------------------------
	}

EOSSound::~EOSSound()
{
}

void EOSSound::play(float gain, bool isRelative, bool loop)
{
}

void EOSSound::replay()
{
}

void EOSSound::crossLap(EOSSound *sound)
{
}

void EOSSound::pause()
{
}

void EOSSound::unpause()
{
}

void EOSSound::mute()
{
    alGetSourcef(_src, AL_GAIN, &_preMuteGain);
    alSourcef(_src, AL_GAIN, 0);
}

void EOSSound::unmute()
{
    alSourcef(_src, AL_GAIN, _preMuteGain);
    _preMuteGain = 0;
}

void EOSSound::stop()
{
}

void EOSSound::setPosition(float px, float py, float pz)
{
}

void EOSSound::translate(float px, float py, float pz, float vx, float vy, float vz)
{
}

void EOSSound::setPitch(float pitch)
{
}

void EOSSound::setGain(float gain)
{
}

void EOSSound::flush()
{
}

bool EOSSound::isPlaying()
{
}

bool EOSSound::isLooping()
{
}

bool EOSSound::isMuted()
{
    return !(_preMuteGain == 0);
}

bool EOSSound::isLoaded()
{
    return _loaded;
}

void *EOSSound::getStream()
{
    return NULL;
}

unsigned int EOSSound::getSource()
{
    return _src;
}

float *EOSSound::getPos()
{
    return _pos;
}

bool EOSSound::checkErrors(char *funcname)
{
	int alError = alGetError();
	if(alError != AL_NO_ERROR)
	{
		LOGERROR("AUDIO ERROR: %s in EOSSound::%s", (char *)alGetString(alError), funcname);
		return false;
	}
	return true;
}
