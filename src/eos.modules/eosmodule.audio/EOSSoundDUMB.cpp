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


#include "EOSSoundDUMB.h"
#include "EOSAudioDevice.h"

EOSSoundDUMB::EOSSoundDUMB(char *filename) : EOSSound(filename)
{
    _duh = NULL;
    _dumbLoaded = dumbOpen(filename);
    _loaded = _dumbLoaded;
    _thread = NULL;
}

EOSSoundDUMB::~EOSSoundDUMB()
{
    if(_duh)
        unload_duh(_duh);
}

void EOSSoundDUMB::play(float gain, bool isRelative, bool loop)
{
    if(!_dumbLoaded) return;

    _dumbStop = false;
    _dumbPause = false;

   	// set source parameters
	alSourcef (_src, AL_PITCH,			    _pitch);
	alSourcef (_src, AL_GAIN,			    gain);

	// set source position and velocity
	alSourcei (_src, AL_SOURCE_RELATIVE,    isRelative);
	alSource3f(_src, AL_POSITION,		    _pos[0], _pos[1], _pos[2]);
	alSource3f(_src, AL_VELOCITY,		    _vel[0], _vel[1], _vel[2]);
	alSourcef (_src, AL_ROLLOFF_FACTOR,	    EOSAudioDevice::instance()->_rolloffFactor);
	alSourcef (_src, AL_MAX_DISTANCE,	    EOSAudioDevice::instance()->_maxDistance);
	alSourcef (_src, AL_REFERENCE_DISTANCE, EOSAudioDevice::instance()->_referenceDistance);

    _loop = loop;
    _dumbLoop = loop;

    if(!dumbPlayback())
    {
        LOGERROR("[eos.audio] EOSSoundDUMB::play() - dumb refused to play.");
        return;
    }

    _thread = new tthread::thread(threadRun, (void *)this);
    
    if(_thread == NULL)
        LOGERROR("[eos.audio] EOSSoundDUMB::play() - Thread error.");
    else
        LOGINFO("[eos.audio] EOSSoundDUMB::play() - Thread started.");
}

void EOSSoundDUMB::replay()
{
    if(!_dumbLoaded) return;
    //
    stop();
    //
    int relative;
    float gain;
    alGetSourcei(_src, AL_SOURCE_RELATIVE, &relative);
    alGetSourcef(_src, AL_GAIN, &gain);
    play(gain, relative, _loop);
}

void EOSSoundDUMB::pause()
{
    _dumbPause = true;
}

void EOSSoundDUMB::unpause()
{
   _dumbPause = false;
}

void EOSSoundDUMB::stop()
{
    if(_dumbStop) return;

    _dumbPause = false;
    _dumbStop = true;

    _thread->detach();
    LOGINFO("[eos.audio] EOSSoundDUMB::stop() - Waiting thread to complete its task...");
    
    dumbRelease();
}

void EOSSoundDUMB::setPosition(float px, float py, float pz)
{
    _pos[0] = px * EOSAudioDevice::instance()->_distanceFactor;
    _pos[1] = py * EOSAudioDevice::instance()->_distanceFactor;
    _pos[2] = pz * EOSAudioDevice::instance()->_distanceFactor;

	if(alIsSource(_src) == AL_TRUE)
		alSource3f(_src, AL_POSITION, _pos[0], _pos[1], _pos[2]);
}

void EOSSoundDUMB::translate(float px, float py, float pz, float vx, float vy, float vz)
{
    _pos[0] = px * EOSAudioDevice::instance()->_distanceFactor;
    _pos[1] = py * EOSAudioDevice::instance()->_distanceFactor;
    _pos[2] = pz * EOSAudioDevice::instance()->_distanceFactor;
    _vel[0] = vx;
    _vel[1] = vy;
    _vel[2] = vz;

	if(alIsSource(_src) == AL_TRUE)
	{
		alSource3f(_src, AL_POSITION, _pos[0], _pos[1], _pos[2]);
		alSource3f(_src, AL_VELOCITY, _vel[0], _vel[1], _vel[2]);
	}
}

void EOSSoundDUMB::setPitch(float pitch)
{
    if(pitch == 0)
        pitch = SOURCE_PITCHMIN;

    _pitch = pitch;
	if(alIsSource(_src) == AL_TRUE)
		alSourcef(_src, AL_PITCH, _pitch);
}

void EOSSoundDUMB::setGain(float gain)
{
    if(alIsSource(_src) == AL_TRUE)
		alSourcef(_src, AL_GAIN, gain);
}

void EOSSoundDUMB::flush()
{
    stop();
}

bool EOSSoundDUMB::isPlaying()
{
    if(!_dumbLoaded) return false;
	return dumbPlaying();
}

bool EOSSoundDUMB::isLooping()
{
   return _loop;
}

bool EOSSoundDUMB::dumbOpen(char *filename)
{
    std::string pathfile = GETLUAVALUE("eos.application.path");
    pathfile += "/media/sounds/";
    pathfile += filename;
    std::string file = filename;

    std::string ext = filename;
    ext = ext.substr(ext.length()-3, 3);
    transform(ext.begin(), ext.end(), ext.begin(), tolower);

    if(ext == ".it")
        _duh = dumb_load_it_quick(pathfile.c_str());
    else if(ext == ".xm")
        _duh = dumb_load_xm_quick(pathfile.c_str());
    else if(ext == "s3m")
        _duh = dumb_load_s3m_quick(pathfile.c_str());
    else if(ext == "mod")
        _duh = dumb_load_mod_quick(pathfile.c_str());

    if(!_duh)
    {
        LOGERROR("[eos.audio] EOSSoundDUMB::load() - Could not open DUMB file.");
        return false;
    }

    _renderer = duh_start_sigrenderer(_duh, 0, 1, 0);

    _format = AL_FORMAT_MONO16;
    _samplingRate = 44100;
    _delta = 65536.0f / _samplingRate;

    alGenBuffers(BUFFER_NUM, _buffers);
    checkErrors("dumbOpen()");

    _src = EOSAudioDevice::instance()->getAvailableSource();
    if(_src == SOURCE_NONE) return false;

    _dumbStop = true;
    _dumbPause = false;

    LOGINFO("[eos.audio] Sound '%s' loaded", filename);

    return true;
}

void EOSSoundDUMB::dumbRelease()
{
    // stop source
    if(alIsSource(_src) == AL_TRUE)
        alSourceStop(_src);

    // unqueue buffers
    dumbEmpty();

    // delete buffers
    alDeleteBuffers(BUFFER_NUM, _buffers);
    checkErrors("dumbRelease()");

    // terminate renderer
    duh_end_sigrenderer(_renderer);

    LOGINFO("[eos.audio] Renderer terminated, DumbStream released.");
}

bool EOSSoundDUMB::dumbPlayback()
{
    if(dumbPlaying())
        return true;

    for(int i=0; i<BUFFER_NUM; i++)
        if(!dumbStream(_buffers[i]))
            return false;

    alSourceQueueBuffers(_src, BUFFER_NUM, _buffers);
    alSourcePlay(_src);

    return true;
}

bool EOSSoundDUMB::dumbPlaying()
{
    ALenum state;
    alGetSourcei(_src, AL_SOURCE_STATE, &state);

    return(state == AL_PLAYING);
}

bool EOSSoundDUMB::dumbUpdate()
{
    int processed;
    bool active = true;

    alGetSourcei(_src, AL_BUFFERS_PROCESSED, &processed);

    while(processed--)
    {
        ALuint buffer;
        alSourceUnqueueBuffers(_src, 1, &buffer);
        if(!checkErrors("dumbUpdate() - Unqueue")) _dumbStop = true;
        active = dumbStream(buffer);
        alSourceQueueBuffers(_src, 1, &buffer);
        if(!checkErrors("dumbUpdate() - Queue")) _dumbStop = true;
    }

    return active;
}

bool EOSSoundDUMB::dumbStream(unsigned int buffer)
{
    int buffersize = EOSAudioDevice::instance()->_bufferSize;
    char pcm[buffersize];
    int  size = 0,
         section;
    long result;

    memset(pcm, 0, buffersize);
    result = duh_render(_renderer, 16, 0, 0.5f, _delta, buffersize/2, pcm);

    if(result < buffersize/2)
        return false;

    checkErrors("dumbStream()");

    alBufferData(buffer, _format, pcm, buffersize, (ALsizei)_samplingRate);

    if(!checkErrors("dumbStream()"))
        return false;

    return true;
}

void EOSSoundDUMB::dumbSeek(double seconds)
{
    _seek = (long)(seconds * 65536);
    duh_end_sigrenderer(_renderer);
    _renderer = duh_start_sigrenderer(_duh, 0, 1, _seek);
}

void EOSSoundDUMB::dumbEmpty()
{
    ALint queued;
    alGetSourcei(_src, AL_BUFFERS_QUEUED, &queued);

    while(queued--)
    {
        ALuint buffer;
        alSourceUnqueueBuffers(_src, 1, &buffer);
        checkErrors("dumbEmpty()");
    }
}

bool EOSSoundDUMB::checkErrors(const char *funcname)
{
	int alError = alGetError();
	if(alError != AL_NO_ERROR)
	{
		LOGERROR("[eos.audio] ERROR: %s in EOSSoundDUMB::%s", (char *)alGetString(alError), funcname);
		return false;
	}
	return true;
}

void EOSSoundDUMB::threadRun(void *dumbSound)
{
    EOSSoundDUMB *_dumb = (EOSSoundDUMB *)dumbSound;
    
    while(!_dumb->_dumbStop)
    {
        if(!_dumb->_dumbPause)
        {
            if(!_dumb->dumbUpdate() && _dumb->_dumbLoop)
                _dumb->dumbSeek(0);       // if loop then rewind

            if(!_dumb->dumbPlaying())
            {
                if(!_dumb->dumbPlayback())
                    LOGERROR("[eos.audio] dumb abruptly stopped.");
                else
                    LOGERROR("[eos.audio] dumb stream was interrupted.");

                _dumb->_dumbStop = true;
            }
        }
        tthread::this_thread::sleep_for(tthread::chrono::milliseconds(16));
    }
}
