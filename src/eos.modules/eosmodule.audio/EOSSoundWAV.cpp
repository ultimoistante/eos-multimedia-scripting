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


#include "EOSSoundWAV.h"
#include "EOSAudioDevice.h"

EOSSoundWAV::EOSSoundWAV(char *filename) : EOSSound(filename)
{
    _data = NULL;
    _loaded = load(filename);
    if(_loaded)
        _num = EOSAudioDevice::instance()->bindBuffer(_format, _size, _freq, _data);
}

EOSSoundWAV::~EOSSoundWAV()
{
    EOSAudioDevice::instance()->unbindBuffer(_num);
}

void EOSSoundWAV::play(float gain, bool isRelative, bool loop)
{
	// gain clipping
	if(gain < 0.02f)
		return;

	// get first available source
    _src = EOSAudioDevice::instance()->getAvailableSource();

	// no available sources
	if(alIsSource(_src) == AL_FALSE)
	{
		//LOGERROR("OpenAL: Invalid source [%i] in play()", _src);
		_src = SOURCE_NONE;
		return;
	}

	// check sound buffer
	if(alIsBuffer(_num) == AL_FALSE)
	{
		LOGERROR("OpenAL: Invalid sound buffer [%i] in play()", _num);
		_src = SOURCE_NONE;
		return;
	}

	// exit if source was playing
	int state;
	alGetSourcei(_src, AL_SOURCE_STATE, &state);
	if(state == AL_PLAYING)
		return;

	// set source parameters
	alSourcei (_src, AL_BUFFER,			    _num);
	alSourcef (_src, AL_PITCH,			    _pitch);
	alSourcef (_src, AL_GAIN,			    gain);
	alSourcei (_src, AL_LOOPING,		    loop);

	// set source position and velocity
	alSourcei (_src, AL_SOURCE_RELATIVE,    isRelative);
	alSource3f(_src, AL_POSITION,		    _pos[0], _pos[1], _pos[2]);
	alSource3f(_src, AL_VELOCITY,		    _vel[0], _vel[1], _vel[2]);
	alSourcef (_src, AL_ROLLOFF_FACTOR,	    EOSAudioDevice::instance()->_rolloffFactor);
	alSourcef (_src, AL_MAX_DISTANCE,	    EOSAudioDevice::instance()->_maxDistance);
	alSourcef (_src, AL_REFERENCE_DISTANCE, EOSAudioDevice::instance()->_referenceDistance);

	// let finally play!
	alSourcePlay(_src);

	// check for errors in this function
	if(!checkErrors("play()"))
	{
		_src = SOURCE_NONE;
		return;
	}
}

void EOSSoundWAV::replay()
{
    if(alIsSource(_src) == AL_TRUE)
        alSourcePlay(_src);
}

void EOSSoundWAV::pause()
{
	if(alIsSource(_src) == AL_TRUE)
	{
	    int state = -1;
		alGetSourcei(_src, AL_SOURCE_STATE, &state);
		if(state == AL_PLAYING)
			alSourcePause(_src);
	}
}

/* TODO (Administrator#1#): Controllare perch� nella funzione unpause() il controllo di stato in pausa � commentato. */
void EOSSoundWAV::unpause()
{
	if(alIsSource(_src) == AL_TRUE)
	{
	    int state = -1;
		alGetSourcei(_src, AL_SOURCE_STATE, &state);
		if(state == AL_PAUSED)
			alSourcePlay(_src);
	}
}

void EOSSoundWAV::stop()
{
	if(alIsSource(_src) == AL_TRUE)
	{
        int state = -1;
		alGetSourcei(_src, AL_SOURCE_STATE, &state);
		if(state == AL_PLAYING || state == AL_PAUSED)
		{
			alSourceStop(_src);
			_src = SOURCE_NONE;
		}
	}
}

void EOSSoundWAV::setPosition(float px, float py, float pz)
{
    _pos[0] = px * EOSAudioDevice::instance()->_distanceFactor;
    _pos[1] = py * EOSAudioDevice::instance()->_distanceFactor;
    _pos[2] = pz * EOSAudioDevice::instance()->_distanceFactor;

	if(alIsSource(_src) == AL_TRUE)
		alSource3f(_src, AL_POSITION, _pos[0], _pos[1], _pos[2]);
}

void EOSSoundWAV::translate(float px, float py, float pz, float vx, float vy, float vz)
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

void EOSSoundWAV::setPitch(float pitch)
{
    if(pitch == 0)
        pitch = SOURCE_PITCHMIN;

    _pitch = pitch;
	if(alIsSource(_src) == AL_TRUE)
		alSourcef(_src, AL_PITCH, _pitch);
}

void EOSSoundWAV::setGain(float gain)
{
    if(alIsSource(_src) == AL_TRUE)
		alSourcef(_src, AL_GAIN, gain);
}

void EOSSoundWAV::flush()
{
    stop();
}

bool EOSSoundWAV::isPlaying()
{
    int state;
    if(alIsSource(_src) == AL_TRUE)
        alGetSourcei(_src, AL_SOURCE_STATE, &state);

	return (state == AL_PLAYING);
}

bool EOSSoundWAV::isLooping()
{
    int loop;
    if(alIsSource(_src) == AL_TRUE)
        alGetSourcei(_src, AL_LOOPING, &loop);

    return (loop);
}

bool EOSSoundWAV::load(char *filename)
{
    WAVChunkHdr_Struct ChunkHdr;
	WAVFmtExHdr_Struct FmtExHdr;
	WAVFileHdr_Struct FileHdr;
	WAVSmplHdr_Struct SmplHdr;
	WAVFmtHdr_Struct FmtHdr;
	ALbyte *stream;

    char *memory = (char *)EOSResourceManager::instance()->loadFile(filename);

    if(!memory)
    {
        LOGERROR("[eos.audio] EOSSoundWAV::load() - Could not open Wav file.");
        return false;
    }

	_format = AL_FORMAT_MONO16;
	_data = NULL;
	_size = 0;
	_freq = 22050;
	_loop = false;

    stream = (ALbyte*)memory;
    memcpy(&FileHdr, stream, sizeof(WAVFileHdr_Struct));
    stream += sizeof(WAVFileHdr_Struct);
    FileHdr.Size = ((FileHdr.Size+1)&~1)-4;
    while((FileHdr.Size!=0) && (memcpy(&ChunkHdr, stream, sizeof(WAVChunkHdr_Struct))))
    {
        stream += sizeof(WAVChunkHdr_Struct);
        if(!memcmp(ChunkHdr.Id, "fmt ", 4))
        {
            memcpy(&FmtHdr, stream, sizeof(WAVFmtHdr_Struct));
            if(FmtHdr.Format == 0x0001)
            {
                _format = (FmtHdr.Channels==1?
                    (FmtHdr.BitsPerSample==8?AL_FORMAT_MONO8:AL_FORMAT_MONO16):
                    (FmtHdr.BitsPerSample==8?AL_FORMAT_STEREO8:AL_FORMAT_STEREO16));
                _freq = FmtHdr.SamplesPerSec;
                stream += ChunkHdr.Size;
            }
            else
            {
                memcpy(&FmtExHdr, stream, sizeof(WAVFmtExHdr_Struct));
                stream += ChunkHdr.Size;
            }
        }
        else if(!memcmp(ChunkHdr.Id, "data", 4))
        {
            if(FmtHdr.Format == 0x0001)
            {
                _size = ChunkHdr.Size;
                _data = (unsigned char *)malloc(ChunkHdr.Size+31);
                if(_data)
                    memcpy(_data, stream, ChunkHdr.Size);
                memset(((char *)_data)+ChunkHdr.Size, 0, 31);
                stream += ChunkHdr.Size;
            }
            else if(FmtHdr.Format == 0x0011)
            {
                //IMA ADPCM
            }
            else if(FmtHdr.Format == 0x0055)
            {
                //MP3 WAVE
            }
        }
        else if(!memcmp(ChunkHdr.Id, "smpl", 4))
        {
            memcpy(&SmplHdr, stream, sizeof(WAVSmplHdr_Struct));
            _loop = (SmplHdr.Loops ? AL_TRUE : AL_FALSE);
            stream += ChunkHdr.Size;
        }
        else
            stream += ChunkHdr.Size;

        stream += ChunkHdr.Size&1;
        FileHdr.Size -= (((ChunkHdr.Size+1)&~1)+8);
    }

    LOGINFO("[eos.audio] Sound loaded %s, format: 0x%x, size %i, frequency %i, loop: %s", filename, _format, _size, _freq, (_loop)?"y":"n");
    return true;
}

