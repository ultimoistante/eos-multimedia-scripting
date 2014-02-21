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


#include "EOSAudioDevice.h"

static EOSAudioDevice device;

EOSAudioDevice::EOSAudioDevice()
{
    _soundID = 0;
    _skippedSounds = 0;
}

EOSAudioDevice::~EOSAudioDevice()
{
}

EOSAudioDevice *EOSAudioDevice::instance()
{
	return &device;
}

bool EOSAudioDevice::initialize()
{
	ALCcontext *pContext = 0;
	ALCdevice *pDevice = 0;

	// load config
	_device = "auto";
	_bufferSize = BUFFER_SIZE;
	_sourcesLimit = SOURCES_LIMIT;
	_distanceFactor = 1;
	_dopplerFactor = 1;
	_speedOfSound = 343.3f;
	_rolloffFactor = 0.001f;
	_maxDistance = 4000;
	_referenceDistance = 3000;
	_masterVolume = 1;

	// clean error status
	alGetError();

	// initialize OpenAL
    pDevice = alcOpenDevice(NULL);      // select the "default device"
    if(pDevice)
    {
        pContext = alcCreateContext(pDevice, NULL);
        alcMakeContextCurrent(pContext);
    }

    const ALchar *strVendor = alGetString(AL_VENDOR),
                 *strVersion = alGetString(AL_VERSION),
                 *strRenderer = alGetString(AL_RENDERER),
                 *strAlExtensions = alGetString(AL_EXTENSIONS);

    LOGINFO("OpenAL v%s by %s", strVersion, strVendor);
    LOGINFO("  Renderer: %s", strRenderer);
    LOGINFO("  Extensions: %s", strAlExtensions);

    const ALCchar *strDefSpecifier = alcGetString(pDevice, ALC_DEFAULT_DEVICE_SPECIFIER),
                  *strCaptDefSpecifier = alcGetString(pDevice, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER),
                  *strAlcExtensions = alcGetString(pDevice, ALC_EXTENSIONS);

    LOGINFO("  Default device: %s", strDefSpecifier);
    LOGINFO("  Default capture device: %s", strCaptDefSpecifier);
    LOGINFO("  Device extensions: %s", strAlcExtensions);

    // query extensions
    _extLinearDistanceClamped = alIsExtensionPresent("AL_LINEAR_DISTANCE_CLAMPED");
    LOGINFO("  Extension [AL_LINEAR_DISTANCE_CLAMPED]: is %s", (_extLinearDistanceClamped)?"available.":"not available.");

	// allocate sources
	allocateSources();
	LOGINFO("  OpenAL: %i audio sources available", sourcesHandle.size());
	checkErrors("initialize[sources allocation]");

	// distance factor
	if(_extLinearDistanceClamped)
        alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
    else
        alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

	setDistanceFactor(_distanceFactor);
	checkErrors("initialize[distance factor]");

	// doppler factor
    alDopplerFactor(_dopplerFactor);		// don't exaggerate doppler shift
    alSpeedOfSound(_speedOfSound);	        // using meters/second

	// init listner
	listenerSetPosition(0, 0, 0);
	listenerSetOrientation(0, 0, 1, 0, 1, 0);
	listenerSetGain(_masterVolume);
	checkErrors("initialize[listener]");

    // init libdumb
    dumb_register_stdfiles();

	// check for errors in this function
	return checkErrors("initialize");
}

bool EOSAudioDevice::shutdown()
{
	ALCcontext *pContext;
    ALCdevice *pDevice;

    // shutdown libdumb
    dumb_exit();

    // log skipped sounds
    if(_skippedSounds > 0)
        LOGWARNING("[eos.audio] %i sounds skipped (not available sources)", _skippedSounds);

    // delete sounds
    LOGINFO("[eos.audio] Shutting down %i sound(s)", sounds.size());
    flushSounds();
    std::map<int, EOSSound *>::iterator it;
    for(it=sounds.begin(); it!=sounds.end(); it++)
        delete it->second;
    sounds.clear();

	// sources
	LOGINFO("[eos.audio] Shutting down %i audio source(s)", sourcesHandle.size());
	for(unsigned int i=0; i<sourcesHandle.size(); i++)
		alDeleteSources(1, &sourcesHandle[i]);
	sourcesHandle.clear();

    // buffers unbind
    unbindAllBuffers();

    pContext = alcGetCurrentContext();			        // get active context
    pDevice = alcGetContextsDevice(pContext);	        // get device for active context

    // context shutdown
    LOGINFO("[eos.audio] Context shutdown...");
    alcMakeContextCurrent(NULL);				        // disable context
    alcDestroyContext(pContext);				        // release context(s)

    // device shutdown
    LOGINFO("[eos.audio] Device shutdown...");
    alcCloseDevice(pDevice);					        // close device

    LOGINFO("[eos.audio] Shutdown complete.");

	// check for errors in this function
	return true; //checkErrors("shutdown()");
}

void EOSAudioDevice::update()
{
    /*
     * UNDER DEVELOPMENT - NOT FOR PRODUCTION
     * 
    EOSSound *snd;
    int state, relative;
    float x, y, z, *pos, sqDist, refDist = (_referenceDistance*2)*(_referenceDistance*2);

    // mute attenuated sounds
    std::map<int, EOSSound *>::iterator it;
    for(it=sounds.begin(); it!=sounds.end(); it++)
    {
        snd = it->second;
        if(snd->isPlaying() && snd->isLooping())
        {
            pos = snd->getPos();

            x = pos[0] - _listenerPos[0];
            y = pos[1] - _listenerPos[1];
            z = pos[2] - _listenerPos[2];

            sqDist = x*x + y*y + z*z;
            //printf("sqdist: %f, refdist: %f\n", sqDist, refDist);

            if(sqDist > refDist && !snd->isMuted())
            {
                snd->mute();
                printf("mute\n");
            }
            else if(sqDist <= refDist && snd->isMuted())
            {
                snd->unmute();
                printf("unmute\n");
            }
        }
    }
    */
}

// LISTENER

void EOSAudioDevice::listenerSetPosition(float px, float py, float pz)
{
    checkErrors("pre-listenerSetPosition()");
	_listenerPos[0] = px * _distanceFactor;
	_listenerPos[1] = py * _distanceFactor;
	_listenerPos[2] = pz * _distanceFactor;
	alListener3f(AL_POSITION, _listenerPos[0], _listenerPos[1], _listenerPos[2]);
	checkErrors("listenerSetPosition()");
}

void EOSAudioDevice::listenerSetVelocity(float vx, float vy, float vz)
{
	alListener3f(AL_VELOCITY, vx, vy, vz);
	checkErrors("listenerSetVelocity()");
}

void EOSAudioDevice::listenerSetOrientation(float atx, float aty, float atz, float upx, float upy, float upz)
{
	_listenerOrientation[0] = atx;
	_listenerOrientation[1] = aty;
	_listenerOrientation[2] = atz;
	_listenerOrientation[3] = upx;
	_listenerOrientation[4] = upy;
	_listenerOrientation[5] = upz;
	alListenerfv(AL_ORIENTATION, _listenerOrientation);
	checkErrors("listenerSetOrientation()");
}

void EOSAudioDevice::listenerSetOrientationAt(float atx, float aty, float atz)
{
	_listenerOrientation[0] = atx;
	_listenerOrientation[1] = aty;
	_listenerOrientation[2] = atz;
	alListenerfv(AL_ORIENTATION, _listenerOrientation);
	checkErrors("listenerSetOrientationAt()");
}

void EOSAudioDevice::listenerSetOrientationUp(float upx, float upy, float upz)
{
	_listenerOrientation[3] = upx;
	_listenerOrientation[4] = upy;
	_listenerOrientation[5] = upz;
	alListenerfv(AL_ORIENTATION, _listenerOrientation);
	checkErrors("listenerSetOrientationUp()");
}

void EOSAudioDevice::listenerSetGain(float gain)
{
	_masterVolume = gain;
	alListenerf(AL_GAIN, _masterVolume);
	checkErrors("listenerSetGain()");
}

void EOSAudioDevice::setDistanceFactor(float factor)
{
	_distanceFactor = factor;
}

void EOSAudioDevice::setDopplerFactor(float factor)
{
    _dopplerFactor = factor;
    alDopplerFactor(_dopplerFactor);
    checkErrors("setDopplerFactor()");
}

void EOSAudioDevice::setRolloffFactor(float rolloff)
{
    _rolloffFactor = rolloff;
}

void EOSAudioDevice::setMaxDistance(float distance)
{
    _maxDistance = distance;
}

void EOSAudioDevice::setReferenceDistance(float distance)
{
    _referenceDistance = distance;
}

int EOSAudioDevice::soundLoad(char *filename)
	{
    int id = -1;
    
    // -------------- FIXED by jadlazarus (01/02/2008)
    // checks if the sound was already loaded
    // (fixes memory leaks bug)
    //printf("*** pre-load check >>>\n");
	std::map<int, EOSSound*>::iterator it;
	for(it = sounds.begin(); it != sounds.end(); it++)
		{
		EOSSound* sound = it->second;
		if(sound != NULL)		
			{
			if(strcmp(sound->getFileName(), filename) == 0)
				{
				//printf(" - already loaded: '%s', id: %d\n", filename, sound->getID());
				return sound->getID();
				}
			}
		}
	//printf("*** pre-load check <<< (sound not already loaded)\n");
    // -----------------------------------------------
    
    EOSSound *sound = EOSSoundFactory::instance()->createSound(filename);
    if(sound)
    	{
        if(sound->isLoaded())
        	{
            id = ++_soundID;
            sound->setID(id); // added by jadlazarus (01/02/2008)
            sounds.insert(std::pair<int, EOSSound *>(id, sound));
        	}
        else
            delete sound;
    	}
    return id;
	}

void EOSAudioDevice::soundUnload(int id)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        delete it->second;
        sounds.erase(it);
    }
}

void EOSAudioDevice::soundPlay(int id, float gain, bool isRelative, bool loop)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        EOSSound *snd = (*it).second;
        snd->play(gain, isRelative, loop);
    }
}

void EOSAudioDevice::soundReplay(int id)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        EOSSound *snd = (*it).second;
        snd->replay();
    }
}

bool EOSAudioDevice::soundCrossLap(int id, int new_id)
{
    std::map<int, EOSSound*>::iterator it1;
    std::map<int, EOSSound*>::iterator it2;
    it1 = sounds.find(id);
    it2 = sounds.find(new_id);

    if(it1 != sounds.end() && it2 != sounds.end())
    {
        EOSSound *snd1 = (*it1).second;
        EOSSound *snd2 = (*it2).second;
        snd1->crossLap(snd2);
        return true;
    }
    
    return false;
}

void EOSAudioDevice::soundPause(int id)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        EOSSound *snd = (*it).second;
        snd->pause();
    }
}

void EOSAudioDevice::soundUnpause(int id)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        EOSSound *snd = (*it).second;
        snd->unpause();
    }
}

void EOSAudioDevice::soundMute(int id)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        EOSSound *snd = (*it).second;
        snd->mute();
    }
}

void EOSAudioDevice::soundUnmute(int id)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        EOSSound *snd = (*it).second;
        snd->unmute();
    }
}

void EOSAudioDevice::soundStop(int id)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        EOSSound *snd = (*it).second;
        snd->stop();
    }
}

void EOSAudioDevice::soundSetPosition(int id, float px, float py, float pz)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        EOSSound *snd = (*it).second;
        snd->setPosition(px, py, pz);
    }
}

void EOSAudioDevice::soundTranslate(int id, float px, float py, float pz, float vx, float vy, float vz)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        EOSSound *snd = (*it).second;
        snd->translate(px, py, pz, vx, vy, vz);
    }
}

void EOSAudioDevice::soundSetPitch(int id, float pitch)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        EOSSound *snd = (*it).second;
        snd->setPitch(pitch);
    }
}

void EOSAudioDevice::soundSetGain(int id, float gain)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        EOSSound *snd = (*it).second;
        snd->setGain(gain);
    }
}

void EOSAudioDevice::soundFlush(int id)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        EOSSound *snd = (*it).second;
        snd->flush();
    }
}

bool EOSAudioDevice::soundIsPlaying(int id)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        EOSSound *snd = (*it).second;
        return snd->isPlaying();
    }
    return false;
}

bool EOSAudioDevice::soundIsLooping(int id)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        EOSSound *snd = (*it).second;
        return snd->isLooping();
    }
    return false;
}

bool EOSAudioDevice::soundIsMuted(int id)
{
    std::map<int, EOSSound*>::iterator it;
    it = sounds.find(id);
    if(it != sounds.end())
    {
        EOSSound *snd = (*it).second;
        return snd->isMuted();
    }
    return false;
}

void EOSAudioDevice::flushSounds()
{
	LOGINFO("[eos.audio] Flush sounds...");
	std::map<int, EOSSound*>::iterator it;
    for(it=sounds.begin(); it!=sounds.end(); it++)
        it->second->flush();
}

unsigned int EOSAudioDevice::bindBuffer(const int format, const int size, const int freq, unsigned char *buffer)
{
    unsigned int num;

    for(unsigned int i=0; i<bindList.size(); i++)
    {
        if(bindList[i]->_buffer == buffer)
        {
            bindList[i]->_numOwners++;
            return bindList[i]->_bindNum;
        }
    }

	alGenBuffers(1, &num);
	alBufferData(num, format, buffer, size, freq);

	BindIndex *b = new BindIndex(num, buffer);
	bindList.push_back(b);

	// check for errors in this function
	checkErrors("bindBuffer()");

	return num;
}

void EOSAudioDevice::unbindBuffer(unsigned int num)
{
    for(unsigned int i=0; i<bindList.size(); i++)
    {
        if(bindList[i]->_bindNum == num)
        {
            bindList[i]->_numOwners--;
            if(bindList[i]->_numOwners == 0 && alIsBuffer(num))
            {
                alDeleteBuffers(1, &num);
				//printf("unbind: buffer number %d\n", num);
                //
                int alError = alGetError();
                switch(alError)
                {
                case AL_NO_ERROR:
                    bindList.erase(bindList.begin()+i, bindList.begin()+i);
			        //printf("unbind: AL_NO_ERROR (ok)\n");
                    break;
                case AL_INVALID_OPERATION:
                    // The buffer is still in use and can not be deleted
		          	//printf("unbind: AL_INVALID_OPERATION!\n");
                    break;
                default:
                    LOGERROR("[eos.audio] ERROR: %s in EOSAudioDevice::unbindBuffer()", (char *)alGetString(alError));
                }
            }           
            return;
        }
    }
}

void EOSAudioDevice::unbindAllBuffers()
{
    for(unsigned int i=0; i<bindList.size(); i++)
    {
        if(alIsBuffer(bindList[i]->_bindNum))
        {
            alDeleteBuffers(1, &bindList[i]->_bindNum);
            alGetError();
        }
    }
    bindList.clear();
}

int EOSAudioDevice::getAvailableSource()
{
	int state = -1;
	for(unsigned int handle=0; handle<sourcesHandle.size(); handle++)
	{
		alGetSourcei(sourcesHandle[handle], AL_SOURCE_STATE, &state);
        checkErrors("getAvailableSource[source status query]");

		if(state == AL_STOPPED || state == AL_INITIAL)
		{
		    resetSource(sourcesHandle[handle]);
		    return sourcesHandle[handle];
		}
	}

    // reset error status
    int alError = alGetError();
    _skippedSounds++;

	//checkErrors("GetAvailableSource()");
	//LOGERROR("[eos.audio] getAvailableSource() -> No available sources.");

	return SOURCE_NONE;
}

bool EOSAudioDevice::allocateSources()
{
	// reset error state
	alGetError();

	// create as many sources as possible
	unsigned int src;
	for(int i=0; i<_sourcesLimit; i++)
	{
		alGenSources(1, &src);
		if(alGetError() != AL_NO_ERROR)
			break;
		sourcesHandle.push_back(src);
	}

	// check for errors in this function
	return checkErrors("allocateSources()");
}

void EOSAudioDevice::resetSource(unsigned int src)
{
    // reset source parameters
	alSourcei(src, AL_BUFFER, AL_NONE);
	alSourcef(src, AL_PITCH, 1.0f);
	alSourcef(src, AL_GAIN, 1.0f);
	alSourcei(src, AL_LOOPING,	AL_FALSE);
	alSourcei(src, AL_SOURCE_RELATIVE, AL_FALSE);
	checkErrors("resetSource()");
}

bool EOSAudioDevice::checkErrors(const char *funcname)
{
	int alError = alGetError();
	if(alError != AL_NO_ERROR)
	{
		LOGERROR("[eos.audio] ERROR: %s in EOSAudioDevice::%s", (char *)alGetString(alError), funcname);
		return false;
	}
	return true;
}
