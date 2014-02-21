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

#ifndef PICASSOSPRITE_H
#define	PICASSOSPRITE_H

#include "../modulesdk/EOSObject.h"
#include "G2DDefines.h"
#include "G2DElement.h"
#include "G2DSpriteAnim.h"
#include <vmmlib/vector.hpp>
#include <map>

#define HOTSPOT_LEFTTOP         0
#define HOTSPOT_CENTERTOP       1
#define HOTSPOT_RIGHTTOP        2
#define HOTSPOT_CENTER          3
#define HOTSPOT_LEFTBOTTOM      4
#define HOTSPOT_CENTERBOTTOM    5
#define HOTSPOT_RIGHTBOTTOM     6

typedef vmml::vector<2, float> Vector2f ;

// -----------------------------------------------------------------------------

class G2DSprite : public G2DElement
    {
    public:
        G2DSprite();
        virtual ~G2DSprite();

        void paint();

        bool addAnim(const char *name, eos_imagedata *data, uint frames, uint frameWidth, uint frameHeight, float timeDelay, uint loopStart);
        bool setFixedFrame(eos_imagedata *data);
        bool animGoToFrame(uint frameNumber);
        bool setCurrentAnim(const char *name);
        char *getCurrentAnim();

        void setHotSpot(int hotspot);
        void setPosition(int x, int y);
        void setRotation(float angle);

        void bindToObject(EOSObject *obj);

        Vector2f getPosition();
        float getRotation();
        uint getWidth();
        uint getHeight();

    private:
        std::map<std::string, G2DSpriteAnim*> anims;
        G2DSpriteAnim* currentAnim;
        ulong               animTime;

        Vector2f      pos,
                            zoom,
                            hotSpot;

        bool    flipX,
                flipY;

        float   alpha,
                rotationAngle;

        uint    hotSpotId;

        eos_2d_object *controller;

        void    calculateHotSpot();
        bool    glLoadTextureFromBuffer(uint *id, uint width, uint height, void *buffer);
    };

#endif

