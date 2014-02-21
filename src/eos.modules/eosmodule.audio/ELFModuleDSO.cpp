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


#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT
#endif

#include <stdio.h>
#include <string>

#include "ALAudioDevice.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "../ELFModuleSDK/ELFModuleInfo.h"
#include "../ELFModuleSDK/ELFLuaRuntime.h"

/**
<module name="ELFModule_OpenAL" namespace="ELFOpenAL" version="0.9.92">
*/

struct ELFModuleInfo moduleInfo;

// ============================ LIBRARY FUNCTIONS ==============================

/**
<function name="initialize">
    <language code="en">
        <brief text="Initializes OpenAL module. This is the first function you must call to use the module." />
        <description text="" />
        <params>
        </params>
        <returns>
            <return name=""  type="Boolean" desc="true in case of success, false otherwise" />
        </returns>
    </language>
    <language code="it">
        <brief text="Inizializza il modulo OpenAL. Questa è la prima funzione che devi chiamare per usare il modulo." />
        <description text="" />
        <params>
        </params>
        <returns>
            <return name=""  type="Boolean" desc="true in caso di successo, false altrimenti" />
        </returns>
    </language>
</function>
*/
static int initialize(lua_State *luaRuntime)
{
	ALAudioDevice::instance()->initialize();
	return 0;
}

/**
<function name="shutdown">
    <language code="en">
        <brief text="Shuts down OpenAL module." />
        <description text="" />
        <params>
        </params>
        <returns>
            <return name=""  type="Boolean" desc="true in case of success, false otherwise" />
        </returns>
    </language>
    <language code="it">
        <brief text="Arresta il modulo OpenAL." />
        <description text="" />
        <params>
        </params>
        <returns>
            <return name=""  type="Boolean" desc="true in caso di successo, false altrimenti" />
        </returns>
    </language>
</function>
*/
static int shutdown(lua_State *luaRuntime)
{
	ALAudioDevice::instance()->shutdown();
	return 0;
}

/**
<function name="update">
    <language code="en">
        <brief text="Updates OpenAL module status." />
        <description text="In this moment this module does not need the call of this function in order to work, however use it in order to guarantee future compatibility." />
        <params>
            <param name="frameTime" type="Numeric" desc="update time, in seconds" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Aggiorna il modulo OpenAL." />
        <description text="In questo momento questo modulo non necessita la chiamata di questa funzione, comunque usala per garantire la compatibilità futura." />
        <params>
            <param name="frameTime" type="Numeric" desc="durata dell'ultima frame, in secondi" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int update(lua_State *luaRuntime)
{
	ALAudioDevice::instance()->update();
	return 0;
}

/**
<function name="listenerSetPosition">
    <language code="en">
        <brief text="Sets absolute listener position" />
        <description text="" />
        <params>
            <param name="x" type="Numeric" desc="absolute X position where to set the listener" />
            <param name="y" type="Numeric" desc="absolute Y position where to set the listener" />
            <param name="z" type="Numeric" desc="absolute Z position where to set the listener" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Imposta la posizione assoluta del listener" />
        <description text="" />
        <params>
            <param name="x" type="Numeric" desc="coordinata X assoluta dove posizionare il listener" />
            <param name="y" type="Numeric" desc="coordinata Y assoluta dove posizionare il listener" />
            <param name="z" type="Numeric" desc="coordinata Z assoluta dove posizionare il listener" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int listenerSetPosition(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 3, "listenerSetPosition"))
        ALAudioDevice::instance()->listenerSetPosition(lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3));

	return 0;
}

/**
<function name="listenerSetVelocity">
    <language code="en">
        <brief text="Sets absolute listener velocity" />
        <description text="" />
        <params>
            <param name="x" type="Numeric" desc="absolute velocity on X axis" />
            <param name="y" type="Numeric" desc="absolute velocity on Y axis" />
            <param name="z" type="Numeric" desc="absolute velocity on Z axis" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Imposta la velocità assoluta del listener" />
        <description text="" />
        <params>
            <param name="x" type="Numeric" desc="velocità assoluta sull'asse X" />
            <param name="y" type="Numeric" desc="velocità assoluta sull'asse Y" />
            <param name="z" type="Numeric" desc="velocità assoluta sull'asse Z" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int listenerSetVelocity(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 3, "listenerSetVelocity"))
        ALAudioDevice::instance()->listenerSetVelocity(lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3));

	return 0;
}

/**
<function name="listenerSetOrientation">
    <language code="en">
        <brief text="Sets listener orientation" />
        <description text="" />
        <params>
            <param name="atx" type="Numeric" desc="X component of the listener's AT vector" />
            <param name="aty" type="Numeric" desc="Y component of the listener's AT vector" />
            <param name="atz" type="Numeric" desc="Z component of the listener's AT vector" />
            <param name="upx" type="Numeric" desc="X component of the listener's UP vector" />
            <param name="upy" type="Numeric" desc="Y component of the listener's UP vector" />
            <param name="upz" type="Numeric" desc="Z component of the listener's UP vector" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Imposta l'orientamento del listener" />
        <description text="" />
        <params>
            <param name="atx" type="Numeric" desc="componente X del vettore AT del listener" />
            <param name="aty" type="Numeric" desc="componente Y del vettore AT del listener" />
            <param name="atz" type="Numeric" desc="componente Z del vettore AT del listener" />
            <param name="upx" type="Numeric" desc="componente X del vettore UP del listener" />
            <param name="upy" type="Numeric" desc="componente Y del vettore UP del listener" />
            <param name="upz" type="Numeric" desc="componente Z del vettore UP del listener" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int listenerSetOrientation(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 6, "listenerSetOrientation"))
        ALAudioDevice::instance()->listenerSetOrientation(lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4), lua_tonumber(luaRuntime, 5), lua_tonumber(luaRuntime, 6));

	return 0;
}

/**
<function name="listenerSetOrientationAt">
    <language code="en">
        <brief text="Sets listener AT orientation" />
        <description text="" />
        <params>
            <param name="atx" type="Numeric" desc="X component of the listener's AT vector" />
            <param name="aty" type="Numeric" desc="Y component of the listener's AT vector" />
            <param name="atz" type="Numeric" desc="Z component of the listener's AT vector" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Imposta l'orientamento AT del listener" />
        <description text="" />
        <params>
            <param name="atx" type="Numeric" desc="componente X del vettore AT del listener" />
            <param name="aty" type="Numeric" desc="componente Y del vettore AT del listener" />
            <param name="atz" type="Numeric" desc="componente Z del vettore AT del listener" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/static int listenerSetOrientationAt(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 3, "listenerSetOrientationAt"))
        ALAudioDevice::instance()->listenerSetOrientationAt(lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3));

	return 0;
}

/**
<function name="listenerSetOrientationUp">
    <language code="en">
        <brief text="Sets listener UP orientation" />
        <description text="" />
        <params>
            <param name="upx" type="Numeric" desc="X component of the listener's UP vector" />
            <param name="upy" type="Numeric" desc="Y component of the listener's UP vector" />
            <param name="upz" type="Numeric" desc="Z component of the listener's UP vector" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Imposta l'orientamento UP del listener" />
        <description text="" />
        <params>
            <param name="upx" type="Numeric" desc="componente X del vettore UP del listener" />
            <param name="upy" type="Numeric" desc="componente Y del vettore UP del listener" />
            <param name="upz" type="Numeric" desc="componente Z del vettore UP del listener" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int listenerSetOrientationUp(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 3, "listenerSetOrientationUp"))
        ALAudioDevice::instance()->listenerSetOrientationUp(lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3));

	return 0;
}

/**
<function name="listenerSetGain">
    <language code="en">
        <brief text="Sets listener volume gain" />
        <description text="" />
        <params>
            <param name="gain" type="Numeric" desc="listener volume gain - default value: 1.0" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Imposta il guadagno del volume del listener" />
        <description text="" />
        <params>
            <param name="gain" type="Numeric" desc="guadagno volume listener - valore default: 1.0" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int listenerSetGain(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "listenerSetGain"))
        ALAudioDevice::instance()->listenerSetGain(lua_tonumber(luaRuntime, 1));

	return 0;
}

/**
<function name="setDistanceFactor">
    <language code="en">
        <brief text="Sets distance factor" />
        <description text="Modify this value to adjust OpenAL distances to scale adopted in your application." />
        <params>
            <param name="distanceFactor" type="Numeric" desc="distance factor - default value: 1.0" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Imposta il distance factor" />
        <description text="Modifica questo valore per regolare le distanze in OpenAL alla scala adottata nella tua applicazione." />
        <params>
            <param name="distanceFactor" type="Numeric" desc="distance factor - valore default: 1.0" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int setDistanceFactor(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "setDistanceFactor"))
        ALAudioDevice::instance()->setDistanceFactor(lua_tonumber(luaRuntime, 1));

	return 0;
}

/**
<function name="setDopplerFactor">
    <language code="en">
        <brief text="Sets doppler factor" />
        <description text="Sets the OpenAL doppler factor value." />
        <params>
            <param name="dopplerFactor" type="Numeric" desc="doppler factor - default value: 1.0" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Imposta il doppler factor" />
        <description text="Imposta il fattore di scala del doppler in OpenAL." />
        <params>
            <param name="dopplerFactor" type="Numeric" desc="doppler factor - valore default: 1.0" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int setDopplerFactor(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "setDopplerFactor"))
        ALAudioDevice::instance()->setDopplerFactor(lua_tonumber(luaRuntime, 1));

	return 0;
}

/**
<function name="setRolloffFactor">
    <language code="en">
        <brief text="Sets rolloff factor" />
        <description text="Sets the rolloff rate for the audio sources" />
        <params>
            <param name="rolloffFactor" type="Numeric" desc="rolloff factor - default value: 1.0" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Imposta il rolloff factor" />
        <description text="Imposta il fattore di caduta delle sorgenti sonore." />
        <params>
            <param name="dopplerFactor" type="Numeric" desc="rolloff factor - valore default: 1.0" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int setRolloffFactor(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "setRolloffFactor"))
        ALAudioDevice::instance()->setRolloffFactor(lua_tonumber(luaRuntime, 1));

	return 0;
}

/**
<function name="setMaxDistance">
    <language code="en">
        <brief text="Sets max distance" />
        <description text="Sets the distance where there will no longer be any attenuation of the source." />
        <params>
            <param name="maxDistance" type="Numeric" desc="max distance - default value: 4000.0" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Imposta la distanza massima" />
        <description text="Imposta la distanza massima oltre la quale non verrà più applicata nessuna attenuazione alla sorgente sonora." />
        <params>
            <param name="maxDistance" type="Numeric" desc="distanza massima - valore default: 4000.0" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int setMaxDistance(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "setMaxDistance"))
        ALAudioDevice::instance()->setMaxDistance(lua_tonumber(luaRuntime, 1));

	return 0;
}

/**
<function name="setReferenceDistance">
    <language code="en">
        <brief text="Sets reference distance" />
        <description text="Sets the distance under which the volume for the source would normally drop by half." />
        <params>
            <param name="referenceDistance" type="Numeric" desc="reference distance - default value: 3000.0" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Imposta la distanza di riferimento" />
        <description text="Imposta la distanza oltre la quale il volume della sorgente sonora cadrebbe normalmente della metà." />
        <params>
            <param name="referenceDistance" type="Numeric" desc="distanza di riferimento - valore default: 3000.0" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int setReferenceDistance(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "setReferenceDistance"))
        ALAudioDevice::instance()->setReferenceDistance(lua_tonumber(luaRuntime, 1));

	return 0;
}

/**
<function name="soundLoad">
    <language code="en">
        <brief text="Loads a sound" />
        <description text="Actually supported formats: OGG, WAV." />
        <params>
            <param name="filename" type="String" desc="filename to load" />
        </params>
        <returns>
            <return name="soundId" type="Numeric" desc="the ID of loaded sound in case of success, -1 otherwise" />
        </returns>
    </language>
    <language code="it">
        <brief text="Carica un suono" />
        <description text="Formati attualmente supportati: OGG, WAV." />
        <params>
            <param name="filename" type="String" desc="nome del file da caricare" />
        </params>
        <returns>
            <return name="soundId" type="Numeric" desc="ritorna l'ID del suono letto in caso di successo, altrimenti -1" />
        </returns>
    </language>
</function>
*/
static int soundLoad(lua_State *luaRuntime)
{
    int id = -1;
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "soundLoad"))
		id = ALAudioDevice::instance()->soundLoad((char *)luaL_checkstring(luaRuntime, 1));

    lua_pushnumber(luaRuntime, id);
	return 1;
}

/**
<function name="soundUnload">
    <language code="en">
        <brief text="Unloads a previously loaded sound from memory" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Scarica un suono, precedentemente caricato, dalla memoria" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int soundUnload(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "soundUnload"))
        ALAudioDevice::instance()->soundUnload((int)lua_tonumber(luaRuntime, 1));

	return 0;
}

/**
<function name="soundPlay">
    <language code="en">
        <brief text="Plays a sound" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
            <param name="gain" type="Numeric" desc="volume gain of this sound - default value: 1.0" />
            <param name="isRelative" type="Boolean" desc="true if sound position is relative to listener" />
            <param name="loop" type="Boolean" desc="true if sound will be played in loop" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Suona un suono" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
            <param name="gain" type="Numeric" desc="guadagno del volume del suono - valore default: 1.0" />
            <param name="isRelative" type="Boolean" desc="impostare a true se la posizione del suono è relativa al listener" />
            <param name="loop" type="Boolean" desc="impostare a true se il suono sarà suonato in loop" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int soundPlay(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 4, "soundPlay"))
        ALAudioDevice::instance()->soundPlay((int)lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_toboolean(luaRuntime, 3), lua_toboolean(luaRuntime, 4));

	return 0;
}

/**
<function name="soundReplay">
    <language code="en">
        <brief text="Replays a sound already played" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Suona un suono già suonato" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int soundReplay(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "soundReplay"))
        ALAudioDevice::instance()->soundReplay((int)lua_tonumber(luaRuntime, 1));

	return 0;
}

/**
<function name="soundCrossLap">
    <language code="en">
        <brief text="CrossLaps current sound with another one" />
        <description text="Overlaps and blends the boundary at a transition between two separate sounds. Actually implemented only for OGG sounds." />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
            <param name="soundIdToCrossLap" type="Numeric" desc="a valid soundId, to crosslap with current sound" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Effettua un CrossLap tra il suono corrente ed un altro" />
        <description text="Sovrappone e smussa il contorno di una transizione tra due suoni separati. Attualmente implementato solo per i suoni OGG." />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
            <param name="soundIdToCrossLap" type="Numeric" desc="un soundId valido, con cui effettuare crosslap" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int soundCrossLap(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 2, "soundCrossLap"))
        ALAudioDevice::instance()->soundCrossLap((int)lua_tonumber(luaRuntime, 1), (int)lua_tonumber(luaRuntime, 2));

	return 0;
}

/**
<function name="soundPause">
    <language code="en">
        <brief text="Pauses a currently playing sound" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Mette in pausa un suono che sta suonando" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int soundPause(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "soundPause"))
        ALAudioDevice::instance()->soundPause((int)lua_tonumber(luaRuntime, 1));

	return 0;
}

/**
<function name="soundUnpause">
    <language code="en">
        <brief text="Resumes a sound in pause" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Riprende un suono in pausa" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int soundUnpause(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "soundUnpause"))
        ALAudioDevice::instance()->soundUnpause((int)lua_tonumber(luaRuntime, 1));

	return 0;
}

/**
<function name="soundMute">
    <language code="en">
        <brief text="Mute a currently playing sound" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Azzera il volume di un suono che sta suonando" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int soundMute(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "soundMute"))
        ALAudioDevice::instance()->soundMute((int)lua_tonumber(luaRuntime, 1));

	return 0;
}

/**
<function name="soundUnmute">
    <language code="en">
        <brief text="Resumes a muted sound" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Ripristina un suono dal volume azzerato" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int soundUnmute(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "soundUnmute"))
        ALAudioDevice::instance()->soundUnmute((int)lua_tonumber(luaRuntime, 1));

	return 0;
}

/**
<function name="soundStop">
    <language code="en">
        <brief text="Stops a currently playing sound" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Ferma un suono che sta suonando" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int soundStop(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "soundStop"))
        ALAudioDevice::instance()->soundStop((int)lua_tonumber(luaRuntime, 1));

	return 0;
}

/**
<function name="soundSetPosition">
    <language code="en">
        <brief text="Sets sound position" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
            <param name="x" type="Numeric" desc="X position where to set the sound" />
            <param name="y" type="Numeric" desc="Y position where to set the sound" />
            <param name="z" type="Numeric" desc="Z position where to set the sound" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Imposta la posizione di un suono" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
            <param name="x" type="Numeric" desc="coordinata X dove posizionare il suono" />
            <param name="y" type="Numeric" desc="coordinata Y dove posizionare il suono" />
            <param name="z" type="Numeric" desc="coordinata Z dove posizionare il suono" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int soundSetPosition(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 4, "soundSetPosition"))
        ALAudioDevice::instance()->soundSetPosition((int)lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4));

	return 0;
}

/**
<function name="soundTranslate">
    <language code="en">
        <brief text="Translates sound position" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
            <param name="px" type="Numeric" desc="X position where to set the sound" />
            <param name="py" type="Numeric" desc="Y position where to set the sound" />
            <param name="pz" type="Numeric" desc="Z position where to set the sound" />
            <param name="vx" type="Numeric" desc="absolute velocity on X axis" />
            <param name="vy" type="Numeric" desc="absolute velocity on Y axis" />
            <param name="vz" type="Numeric" desc="absolute velocity on Z axis" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Trasla la posizione di un suono" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
            <param name="px" type="Numeric" desc="coordinata X dove posizionare il suono" />
            <param name="py" type="Numeric" desc="coordinata Y dove posizionare il suono" />
            <param name="pz" type="Numeric" desc="coordinata Z dove posizionare il suono" />
            <param name="vx" type="Numeric" desc="velocità assoluta sull'asse X" />
            <param name="vy" type="Numeric" desc="velocità assoluta sull'asse Y" />
            <param name="vz" type="Numeric" desc="velocità assoluta sull'asse Z" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int soundTranslate(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 7, "soundTranslate"))
        ALAudioDevice::instance()->soundTranslate((int)lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3), lua_tonumber(luaRuntime, 4), lua_tonumber(luaRuntime, 5), lua_tonumber(luaRuntime, 6), lua_tonumber(luaRuntime, 7));

	return 0;
}

/**
<function name="soundSetPitch">
    <language code="en">
        <brief text="Sets the pitch of a sound" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
            <param name="pitch" type="Numeric" desc="sound pitch - default value: 1.0" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Imposta il pitch di un suono" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
            <param name="pitch" type="Numeric" desc="pitch del suono - valore default: 1.0" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int soundSetPitch(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "soundSetPitch"))
        ALAudioDevice::instance()->soundSetPitch((int)lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2));

	return 0;
}

/**
<function name="soundSetGain">
    <language code="en">
        <brief text="Sets the gain of a sound" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
            <param name="gain" type="Numeric" desc="volume gain of this sound - default value: 1.0" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Imposta il guadagno del volume di un suono" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
            <param name="gain" type="Numeric" desc="guadagno del volume del suono - valore default: 1.0" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int soundSetGain(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "soundSetGain"))
        ALAudioDevice::instance()->soundSetGain((int)lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2));

	return 0;
}

/**
<function name="soundFlush">
    <language code="en">
        <brief text="Flushes a sound" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
        </params>
        <returns>
        </returns>
    </language>
    <language code="it">
        <brief text="Scarica i buffer in coda di un suono" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
        </params>
        <returns>
        </returns>
    </language>
</function>
*/
static int soundFlush(lua_State *luaRuntime)
{
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "soundFlush"))
        ALAudioDevice::instance()->soundFlush((int)lua_tonumber(luaRuntime, 1));

	return 0;
}

/**
<function name="soundIsPlaying">
    <language code="en">
        <brief text="Queries current play status of a sound" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
        </params>
        <returns>
            <return name="" type="Boolean" desc="true if sound was playing now, false otherwise" />
        </returns>
    </language>
    <language code="it">
        <brief text="Interroga lo stato corrente di esecuzione di un suono" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
        </params>
        <returns>
            <return name="" type="Boolean" desc="true se il suono è in esecuzione, altrimenti false" />
        </returns>
    </language>
</function>
*/
static int soundIsPlaying(lua_State *luaRuntime)
{
    bool ip = false;
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "soundIsPlaying"))
		ip = ALAudioDevice::instance()->soundIsPlaying((int)lua_tonumber(luaRuntime, 1));

    lua_pushboolean(luaRuntime, ip);
	return 1;
}

/**
<function name="soundIsLooping">
    <language code="en">
        <brief text="Queries current loop status of a sound" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
        </params>
        <returns>
            <return name="" type="Boolean" desc="true if sound was playing in loop, false otherwise" />
        </returns>
    </language>
    <language code="it">
        <brief text="Interroga lo stato corrente di esecuzione ciclica di un suono" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
        </params>
        <returns>
            <return name="" type="Boolean" desc="true se il suono è in esecuzione ciclica, altrimenti false" />
        </returns>
    </language>
</function>
*/
static int soundIsLooping(lua_State *luaRuntime)
{
    bool ip = false;
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "soundIsLooping"))
		ip = ALAudioDevice::instance()->soundIsLooping((int)lua_tonumber(luaRuntime, 1));

    lua_pushboolean(luaRuntime, ip);
	return 1;
}

/**
<function name="soundIsMuted">
    <language code="en">
        <brief text="Queries current mute status of a sound" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="a valid sound id of a previously loaded sound" />
        </params>
        <returns>
            <return name="" type="Boolean" desc="true if sound was muted, false otherwise" />
        </returns>
    </language>
    <language code="it">
        <brief text="Interroga lo stato silenziazione di un suono" />
        <description text="" />
        <params>
            <param name="soundId" type="Numeric" desc="un sound id di un suono precedentemente caricato" />
        </params>
        <returns>
            <return name="" type="Boolean" desc="true se il suono è silenziato, altrimenti false" />
        </returns>
    </language>
</function>
*/
static int soundIsMuted(lua_State *luaRuntime)
{
    bool ip = false;
    if(EOSLuaRuntime::checkLuaParameters(&moduleInfo, 1, "soundIsMuted"))
		ip = ALAudioDevice::instance()->soundIsMuted((int)lua_tonumber(luaRuntime, 1));

    lua_pushboolean(luaRuntime, ip);
	return 1;
}

// ========================== END LIBRARY FUNCTIONS ============================

void loadModuleSymbols()
    {
    }

// ============================ UTILITY FUNCTIONS ==============================

//static void executeLuaChunk(lua_State* luaState, const char* chunkString)

// -----------------------------------------------------------------------------

// ========================== END UTILITY FUNCTIONS ============================


// ======================== LUA FUNCTIONS REGISTRATION =========================

// ==== Here we declare an array with all functions and their respective names.
// ==== This array has elements of type luaL_reg, which is a structure with
// ==== two fields: a string and a function pointer.

static const struct luaL_reg module_table [] = {
	{"initialize", initialize},
    {"shutdown", shutdown},
    {"update", update},
    //
    {"listenerSetPosition", listenerSetPosition},
    {"listenerSetVelocity", listenerSetVelocity},
    {"listenerSetOrientation", listenerSetOrientation},
    {"listenerSetOrientationAt", listenerSetOrientationAt},
    {"listenerSetOrientationUp", listenerSetOrientationUp},
    {"listenerSetGain", listenerSetGain},
    //
    {"setDistanceFactor", setDistanceFactor},
    {"setDopplerFactor", setDopplerFactor},
    {"setRolloffFactor", setRolloffFactor},
    {"setMaxDistance", setMaxDistance},
    {"setReferenceDistance", setReferenceDistance},
    //
    {"soundLoad", soundLoad},
    {"soundUnload", soundUnload},
    {"soundPlay", soundPlay},
    {"soundReplay", soundReplay},
    {"soundCrossLap", soundCrossLap},
    {"soundPause", soundPause},
    {"soundUnpause", soundUnpause},
    {"soundMute", soundMute},
    {"soundUnmute", soundUnmute},
    {"soundStop", soundStop},
    {"soundSetPosition", soundSetPosition},
    {"soundTranslate", soundTranslate},
    {"soundSetPitch", soundSetPitch},
    {"soundSetGain", soundSetGain},
    {"soundFlush", soundFlush},
    {"soundIsPlaying", soundIsPlaying},
    {"soundIsLooping", soundIsLooping},
    {"soundIsMuted", soundIsMuted},
    {NULL, NULL}  // <-- end-of-table sentinel
	};

// ==== Finally, we declare a main (initialization) function, using luaL_openlib:

int _INIT_(lua_State* luaRuntime)
	{
	EOSLuaRuntime::getInstance()->setRuntime(luaRuntime);
    //
	moduleInfo.moduleName           = "EOSOpenAL";
	moduleInfo.moduleVerMajor       = 0;
	moduleInfo.moduleVerMinor       = 9;
	moduleInfo.moduleBuild          = 92;
	moduleInfo.moduleReleaseDate    = "December 1, 2006";
	moduleInfo.moduleNamespace      = "EOSOpenAL";
	moduleInfo.moduleShortDesc      = "EOS OpenAL Audio Module";
	moduleInfo.moduleDescription1   = "";
	moduleInfo.moduleDescription2   = "";
	moduleInfo.moduleVendor         = "alex9978 of Emthesis Development";
	moduleInfo.moduleVendorHomepage = "www.emthesis.com";
	moduleInfo.moduleVendorEmail    = "rasulo@emthesis.com";
	//EOSLuaRuntime::logModuleInfo(&moduleInfo);
	//
	/*#ifdef OLDLUA
	luaL_openlib(luaRuntime, moduleInfo.moduleNamespace, module_table, 0);
	#else*/
	// Lua 5.1:
	luaL_register(luaRuntime, moduleInfo.moduleNamespace, module_table);
	//#endif
	//
	EOSLuaRuntime::logModuleInfo(&moduleInfo);
	//
	// loads module symbols
	loadModuleSymbols();
	//
	return 1;
	}

// -----------------------------------------------------------------------------

} // extern "C" closure

/**
</module>

Changelog:

v.0.9.92 - Dec 1, 2006

    bugfix:
        stream thread deadlock fixed

v.0.9.91 - Nov 30, 2006

    bugfix:
        removed error log on no available source

    added:
        skipped sounds logged
        wav, ogg, dumb sounds now auto-terminate on error

v.0.9.9 - Nov 6, 2006

    bugfix:
        soundOgg stop function bug fixed

v.0.9.8 - Sep 25, 2006

    added:
        soundMute()
        soundUnmute()
        soundIsMuted()

v.0.9.7 - Sep 13, 2006

    bugfix

v.0.9.6 - Aug 25, 2006

    bugfix

v.0.9.5 - Aug 11, 2006

    added:
        support for .it, .xm, .s3m, .mod file types
*/
