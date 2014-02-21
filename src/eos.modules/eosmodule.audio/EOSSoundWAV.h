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

#ifndef EOSSOUNDWAV_H
#define EOSSOUNDWAV_H

#include "EOSSound.h"

#define PADOFF_VAR __attribute__((packed))

typedef struct                                  /* WAV File-header */
{
  ALubyte  Id[4]			PADOFF_VAR;
  ALsizei  Size				PADOFF_VAR;
  ALubyte  Type[4]			PADOFF_VAR;
} WAVFileHdr_Struct;

typedef struct                                  /* WAV Fmt-header */
{
  ALushort Format			PADOFF_VAR;
  ALushort Channels			PADOFF_VAR;
  ALuint   SamplesPerSec	PADOFF_VAR;
  ALuint   BytesPerSec		PADOFF_VAR;
  ALushort BlockAlign		PADOFF_VAR;
  ALushort BitsPerSample	PADOFF_VAR;
} WAVFmtHdr_Struct;

typedef struct									/* WAV FmtEx-header */
{
  ALushort Size				PADOFF_VAR;
  ALushort SamplesPerBlock	PADOFF_VAR;
} WAVFmtExHdr_Struct;

typedef struct                                  /* WAV Smpl-header */
{
  ALuint   Manufacturer		PADOFF_VAR;
  ALuint   Product			PADOFF_VAR;
  ALuint   SamplePeriod		PADOFF_VAR;
  ALuint   Note				PADOFF_VAR;
  ALuint   FineTune			PADOFF_VAR;
  ALuint   SMPTEFormat		PADOFF_VAR;
  ALuint   SMPTEOffest		PADOFF_VAR;
  ALuint   Loops			PADOFF_VAR;
  ALuint   SamplerData		PADOFF_VAR;
  struct
  {
    ALuint Identifier		PADOFF_VAR;
    ALuint Type				PADOFF_VAR;
    ALuint Start			PADOFF_VAR;
    ALuint End				PADOFF_VAR;
    ALuint Fraction			PADOFF_VAR;
    ALuint Count			PADOFF_VAR;
  }      Loop[1]			PADOFF_VAR;
} WAVSmplHdr_Struct;

typedef struct                                  /* WAV Chunk-header */
{
  ALubyte  Id[4]			PADOFF_VAR;
  ALuint   Size				PADOFF_VAR;
} WAVChunkHdr_Struct;

#ifdef PADOFF_VAR			    				/* Default alignment */
	#undef PADOFF_VAR
#endif


class EOSSoundWAV : public EOSSound
{
public:
    EOSSoundWAV(char *filename);
    ~EOSSoundWAV();

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

private:
    bool load(char *filename);

    unsigned int    _num;
    int             _format,
                    _size,
                    _freq;
    unsigned char  *_data;
};

#endif // ALSOUND_H
