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

#ifndef EOSAUDIODEVICE_H
#define EOSAUDIODEVICE_H

#include "EOSAudioDefines.h"
#include "EOSSound.h"
#include "EOSSoundFactory.h"

#include <EOSLuaRuntime.h>

#include <dumb.h>
#include <stdio.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <vector>
#include <string>


class BindIndex
{
public:
    BindIndex(unsigned int num, unsigned char *buffer) : _bindNum(num), _buffer(buffer), _numOwners(1) {}
    ~BindIndex();

    unsigned int        _bindNum,
                        _numOwners;
    unsigned char      *_buffer;
};


class EOSAudioDevice
{
public:
	EOSAudioDevice();
	~EOSAudioDevice();

    static EOSAudioDevice *instance();

	bool initialize();
	bool shutdown();
	void update();

	// LISTENER
	void listenerSetPosition(float px, float py, float pz);
	void listenerSetVelocity(float vx, float vy, float vz);
	void listenerSetOrientation(float atx, float aty, float atz, float upx, float upy, float upz);
	void listenerSetOrientationAt(float atx, float aty, float atz);
	void listenerSetOrientationUp(float upx, float upy, float upz);
	void listenerSetGain(float gain);

	// FACTORS
	void setDistanceFactor(float factor);
	void setDopplerFactor(float factor);
	void setRolloffFactor(float rolloff);
	void setMaxDistance(float distance);
	void setReferenceDistance(float distance);

	// SOUNDS
	int  soundLoad(char *filename);
	void soundUnload(int id);
	void soundPlay(int id, float gain, bool isRelative, bool loop = false);
	void soundReplay(int id);
	bool soundCrossLap(int id, int new_id);
	void soundPause(int id);
	void soundUnpause(int id);
    void soundMute(int id);
    void soundUnmute(int id);
	void soundStop(int id);
	void soundSetPosition(int id, float px, float py, float pz);
	void soundTranslate(int id, float px, float py, float pz, float vx, float vy, float vz);
	void soundSetPitch(int id, float pitch);
	void soundSetGain(int id, float gain);
	void soundFlush(int id);
	bool soundIsPlaying(int id);
	bool soundIsLooping(int id);
	bool soundIsMuted(int id);

	void flushSounds();

    float		_distanceFactor,			// distance factor 1 unit = 1 meter
				_dopplerFactor,
				_speedOfSound,
				_rolloffFactor,
				_maxDistance,
				_referenceDistance,
				_masterVolume;

    int	    	_bufferSize;

    // BUFFERS
    unsigned int bindBuffer(const int format, const int size, const int frequency, unsigned char *buffer);
    void unbindBuffer(unsigned int num);
    void unbindAllBuffers();

    // SOURCES
    int getAvailableSource();

private:	
	int	    	_sourcesLimit;
	long        _skippedSounds;
	std::string _device;

	// extensions
	bool        _extLinearDistanceClamped;

	// listener position & orientation
	float       _listenerPos[3],
                _listenerOrientation[6];

	// sources data
	std::vector<unsigned int> sourcesHandle;
	bool allocateSources();
	void resetSource(unsigned int src);

	// buffers
	std::vector<BindIndex *> bindList;

    // sounds
	std::map<int, EOSSound *> sounds;
	int _soundID;

	// errors
	bool checkErrors(const char *funcname);
};

#endif

