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


#include "EOSSoundOGG.h"
#include "EOSAudioDevice.h"

EOSSoundOGG::EOSSoundOGG(char *filename) : EOSSound(filename)
{
    _thread = NULL;
    _oggStreamToCross = NULL;
    _oggLoaded = load(filename);
    _loaded = _oggLoaded;
    _thread = NULL;
}

EOSSoundOGG::~EOSSoundOGG()
{
    oggRelease();
}

void EOSSoundOGG::play(float gain, bool isRelative, bool loop)
{
    if(!_oggLoaded) return;

    _oggStop = false;
    _oggPause = false;

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
    _oggLoop = loop;

    if(!oggPlayback())
    {
        LOGERROR("[eos.audio] EOSSoundOGG::play() - Ogg refused to play.");
        return;
    }
    
    _thread = new tthread::thread(threadRun, (void *)this);
    
    if(_thread == NULL)
        LOGERROR("[eos.audio] EOSSoundOGG::play() - Thread error.");
    else
        LOGINFO("[eos.audio] EOSSoundOGG::play() - Thread started.");
}

void EOSSoundOGG::replay()
{
    if(!_oggLoaded) return;
    //
    stop();
    //
    int relative;
    float gain;
    alGetSourcei(_src, AL_SOURCE_RELATIVE, &relative);
    alGetSourcef(_src, AL_GAIN, &gain);
    play(gain, relative, _loop);
}

void EOSSoundOGG::crossLap(EOSSound *sound)
{
    _oggStreamToCross = (OggVorbis_File *)sound->getStream();
    if(_oggStreamToCross == NULL)
    {
        LOGERROR("[eos.audio] crossLap() - Can't CrossLap with specified sound, stream not present.");
        return;
    }

    int result = ov_crosslap(&_oggStream, _oggStreamToCross);
    if(result < 0)
        LOGERROR("[eos.audio] crossLap() - %s", oggErrorString(result));
}

void EOSSoundOGG::pause()
{
    _oggPause = true;
}

void EOSSoundOGG::unpause()
{
    _oggPause = false;
}

void EOSSoundOGG::stop()
{
    if(_oggStop) return;

    _oggPause = false;
    _oggStop = true;

    _thread->detach();
    LOGINFO("[eos.audio] EOSSoundOGG::stop() - Waiting thread to complete its task...");
}

void EOSSoundOGG::setPosition(float px, float py, float pz)
{
    _pos[0] = px * EOSAudioDevice::instance()->_distanceFactor;
    _pos[1] = py * EOSAudioDevice::instance()->_distanceFactor;
    _pos[2] = pz * EOSAudioDevice::instance()->_distanceFactor;

	if(alIsSource(_src) == AL_TRUE)
		alSource3f(_src, AL_POSITION, _pos[0], _pos[1], _pos[2]);
}

void EOSSoundOGG::translate(float px, float py, float pz, float vx, float vy, float vz)
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

void EOSSoundOGG::setPitch(float pitch)
{
    if(pitch == 0)
        pitch = SOURCE_PITCHMIN;

    _pitch = pitch;
	if(alIsSource(_src) == AL_TRUE)
		alSourcef(_src, AL_PITCH, _pitch);
}

void EOSSoundOGG::setGain(float gain)
{
    if(alIsSource(_src) == AL_TRUE)
		alSourcef(_src, AL_GAIN, gain);
}

void EOSSoundOGG::flush()
{
    if(_oggLoaded)
        stop();
}

bool EOSSoundOGG::isPlaying()
{
    if(!_oggLoaded) return false;
	return oggPlaying();
}

bool EOSSoundOGG::isLooping()
{
    return _loop;
}

void *EOSSoundOGG::getStream()
{
    return (void *)&_oggStream;
}

bool EOSSoundOGG::load(char *filename)
{
    if(oggOpen(filename))
    {
        LOGINFO("[eos.audio] Sound '%s' loaded", filename);
        //oggDisplay(); // disabled by jadlazarus
        return true;
    }
    return false;
}

bool EOSSoundOGG::checkErrors(const char *funcname)
{
	int alError = alGetError();
	if(alError != AL_NO_ERROR)
	{
		LOGERROR("[eos.audio] ERROR: %s in EOSSoundOGG::%s", (char *)alGetString(alError), funcname);
		return false;
	}
	return true;
}


bool EOSSoundOGG::oggOpen(char *filename)
{
    std::string pathfile = GETLUAVALUE("eos.application.path");
    pathfile += "/media/sounds/";
    pathfile += filename;
    int result;

    if(!(_oggFile = fopen(pathfile.c_str(), "rb")))
    {
        LOGERROR("[eos.audio] EOSSoundOGG::oggOpen() - Could not open Ogg file.");
        return false;
    }

    if((result = ov_open(_oggFile, &_oggStream, NULL, 0)) < 0)
    {
        fclose(_oggFile);
        LOGERROR("[eos.audio] Could not open Ogg stream. %s", oggErrorString(result));
        return false;
    }

    _vorbisInfo = ov_info(&_oggStream, -1);
    _vorbisComment = ov_comment(&_oggStream, -1);

    if(_vorbisInfo->channels == 1)
        _format = AL_FORMAT_MONO16;
    else
        _format = AL_FORMAT_STEREO16;

    alGenBuffers(BUFFER_NUM, _buffers);
    checkErrors("oggOpen()");

    _src = EOSAudioDevice::instance()->getAvailableSource();
    if(_src == SOURCE_NONE) return false;

    _oggStop = true;
    _oggPause = false;

    return true;
}

void EOSSoundOGG::oggRelease()
{
    int result;

    // stop source
    if(alIsSource(_src) == AL_TRUE)
        alSourceStop(_src);

    // unqueue buffers
    oggEmpty();

    // delete buffers
    alDeleteBuffers(BUFFER_NUM, _buffers);
    checkErrors("oggRelease()");

    // clear decode buffers
    if((result = ov_clear(&_oggStream)) < 0)
        LOGERROR("[eos.audio] Could not clear the decoder's buffers. %s", oggErrorString(result));
    else
        LOGINFO("[eos.audio] Decoder's buffers cleared, OggStream released.");

    // cross stream
    if(_oggStreamToCross != NULL)
        if((result = ov_clear(_oggStreamToCross)) < 0)
            LOGERROR("[eos.audio] Could not clear the decoder's buffers of crosslapped stream. %s", oggErrorString(result));
        else
            LOGINFO("[eos.audio] Decoder's buffers cleared, crosslapped OggStream released.");
}

void EOSSoundOGG::oggDisplay()
{
    LOGINFO("[eos.audio] Version %i - Channels %i - Rate (hz) %i", _vorbisInfo->version, _vorbisInfo->channels, _vorbisInfo->rate);
    LOGINFO("[eos.audio] Vendor: %s", _vorbisComment->vendor);
    for(int i=0; i<_vorbisComment->comments; i++)
        LOGINFO("[eos.audio]    %s", _vorbisComment->user_comments[i]);
}

bool EOSSoundOGG::oggPlayback()
{
    if(oggPlaying())
        return true;

    for(int i=0; i<BUFFER_NUM; i++)
        if(!oggStream(_buffers[i]))
            return false;

    alSourceQueueBuffers(_src, BUFFER_NUM, _buffers);
    alSourcePlay(_src);

    return true;
}

bool EOSSoundOGG::oggPlaying()
{
    ALenum state;
    alGetSourcei(_src, AL_SOURCE_STATE, &state);

    return(state == AL_PLAYING);
}

bool EOSSoundOGG::oggUpdate()
{
    int processed;
    bool active = true;

    alGetSourcei(_src, AL_BUFFERS_PROCESSED, &processed);

    while(processed--)
    {
        ALuint buffer;
        if(!checkErrors("oggUpdate() - Pre-Unqueue")) _oggStop = true;
        alSourceUnqueueBuffers(_src, 1, &buffer);
        if(!checkErrors("oggUpdate() - Unqueue")) _oggStop = true;
        active = oggStream(buffer);
        alSourceQueueBuffers(_src, 1, &buffer);
        if(!checkErrors("oggUpdate() - Queue")) _oggStop = true;
    }

    return active;
}

bool EOSSoundOGG::oggStream(unsigned int buffer)
{
    int buffersize = EOSAudioDevice::instance()->_bufferSize;
    char pcm[buffersize];
    int  size = 0,
         section,
         result;

    while(size < buffersize)
    {
        if(_oggStreamToCross == NULL)
            result = ov_read(&_oggStream, pcm+size, buffersize-size, 0, 2, 1, &section);
        else
            result = ov_read(_oggStreamToCross, pcm+size, buffersize-size, 0, 2, 1, &section);

        if(result > 0)
            size += result;
        else
            if(result < 0)
                LOGERROR("[eos.audio] oggStream() - %s", oggErrorString(result));
            else
                break;
    }

    if(size == 0)
        return false;

    checkErrors("oggStream()");

    alBufferData(buffer, _format, pcm, size, _vorbisInfo->rate);

    if(!checkErrors("oggStream()")) return false;

    return true;
}

void EOSSoundOGG::oggSeek(double seconds)
{
    int result = ov_time_seek(&_oggStream, seconds);
    if(result < 0)
        LOGERROR("oggSeek() - %s", oggErrorString(result));

    //ov_crosslap(&_oggStream, &_oggStream); // old, new
}

void EOSSoundOGG::oggEmpty()
{
    int queued;
    alGetSourcei(_src, AL_BUFFERS_QUEUED, &queued);

    while(queued--)
    {
        ALuint buffer;
        alSourceUnqueueBuffers(_src, 1, &buffer);
        checkErrors("oggEmpty()");
    }
}

const char *EOSSoundOGG::oggErrorString(int code)
{
    switch(code)
    {
        case OV_FALSE:          return "Not true, or no data available.";
        case OV_HOLE:           return "Vorbisfile encoutered missing or corrupt data in the bitstream. Recovery is normally automatic and this return code is for informational purposes only.";
        case OV_EREAD:          return "Read error while fetching compressed data for decode.";
        case OV_EFAULT:         return "Internal logic fault in decode state (bug or heap/stack corruption. Continuing is likely not possible.";
        case OV_EIMPL:          return "Feature not implemented.";
        case OV_EINVAL:         return "Invalid argument value; possibly called with an OggVorbis_File structure that isn't open.";
        case OV_ENOTVORBIS:     return "The given file/data was not recognized as Ogg Vorbis data.";
        case OV_EBADHEADER:     return "The file/data is apparently an Ogg Vorbis stream, but contains a corrupted or undecipherable header.";
        case OV_EVERSION:       return "The bitstream format revision of the given stream is not supported.";
        case OV_EBADLINK:       return "Invalid stream section supplied to libvorbisfile, or the requested link is corrupt.";
        case OV_ENOSEEK:        return "The given stream is not seekable.";
        default:                return "Panic. Unknown Ogg error.";
    }
}

void EOSSoundOGG::threadRun(void* oggSound)
{
    EOSSoundOGG *_ogg = (EOSSoundOGG *)oggSound;
    
    while(!_ogg->_oggStop)
    {
        if(!_ogg->_oggPause)
        {
            if(!_ogg->oggUpdate() && _ogg->_oggLoop)
                _ogg->oggSeek(0);       // if loop then rewind

            if(!_ogg->oggPlaying())
            {
                if(!_ogg->oggPlayback())
                    LOGERROR("[eos.audio] Ogg abruptly stopped.");
                else
                    LOGERROR("[eos.audio] Ogg stream was interrupted.");

                _ogg->_oggStop = true;
            }
        }
        tthread::this_thread::sleep_for(tthread::chrono::milliseconds(16));
    }
}
