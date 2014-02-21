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

#ifndef VIEWSURFACE_H
#define VIEWSURFACE_H

#include <vector>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vmmlib/vector.hpp>
#include <vmmlib/quaternion.hpp>
#include <FreeImage.h>
#include "ViewDefines.h"
#include "ViewPlatformSupport.h"
#include "eos_data_containers.h"

#define VIEW_SURFACE_SRC_NONE            0
#define VIEW_SURFACE_SRC_TEXTURE         1
#define VIEW_SURFACE_SRC_DYNAMICBUFFER   2
#define VIEW_SURFACE_SRC_STATICBUFFER    3
#define VIEW_SURFACE_SRC_CAIROSURFACE    4

typedef vmml::vector<3, float> Vector3f ;

class ViewSurface
    {
    public:
        ViewSurface();
        ~ViewSurface();

        bool    create(uint width, uint height);
        bool    connectSource(eos_imagedata *data);
        bool    disconnectSource();
        void    paint();

        bool    addSubSurface(ViewSurface *surface);
        bool    removeSubSurface(ViewSurface *surface);
        bool    hadSubSurfaces()        { return (subSurfaces.size() > 0); }

        void    setPosition(float x, float y, float z);
        void    setOrientation(float x, float y, float z, float w);
        void    setScale(float x, float y, float z);
        void    setColor(float r, float g, float b, float a);

        void    rotateByAxisAngle(float x, float y, float z, float a);
        void    rotateByAngles(float x, float y, float z);

        vmml::vec3f getPosition();
        vmml::quaternionf getOrientation();

        void    setVisible(bool status) { this->visible = status; }
        bool    isVisible()             { return this->visible; }

        void    setId(uint surfaceId)   { this->id = surfaceId; }
        uint    getId(void)             { return this->id; }

        uint    getWidth()              { return this->width; }
        uint    getHeight()             { return this->height; }
        uint    getGLFboId()            { return this->fboId; }
        uint    getGLTextureId()        { return this->textureId; }
        
        bool    load(const char* filename, bool resizeImage);
        bool    save(const char* filename);

    private:
        ViewSurfaceData srfData;
        eos_imagedata* image;

        uint    width,
                height;
        int     connected;
        bool    visible;        
        uint    id;
        //
        vmml::vec3f position;
        vmml::quaternionf rotation;
        vmml::vec3f scale;
        float   color[4];
        //
        uint    fboId;
        uint    fboIdCopy;
        uint    textureId;
        uint    textureIdCopy;
        uint    glTextureType;      // GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE_ARB                
        uint    pboMode;
        int     index;
        //
        std::vector<ViewSurface*> subSurfaces;
        //
        bool connectSourceTexture(eos_imagedata *data);
        bool connectSourceBuffer(eos_imagedata *data);
        bool connectSourceCairoSurface(eos_imagedata *data);
        void updatePBO();
        void paintSubSurfaces();        
        bool glLoadTexture(uint *id, bool loadBuffer);
        void glUnloadTexture(uint *id);
        bool glLoadFBO();
        void glUnloadFBO();
        void glCopyTexture(uint inId, uint outId);
        uint glPow2(int value);
    };

#endif
