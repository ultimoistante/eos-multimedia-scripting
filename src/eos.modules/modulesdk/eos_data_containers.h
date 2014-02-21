/*
-----------------------------------------------------------------------------
This source file is part of EOS Multimedia Scripting System
For the latest info, see https://github.com/ultimoistante/eos-multimedia-scripting

The MIT License (MIT)

Copyright (c) 2014-2019 of Salvatore Carotenuto aka ultimoistante of StartupSolutions
                          (mailto: ultimoistante@gmail.com | www.startupsolutions.it)

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


#ifndef EOS_DATA_CONTAINERS_H
#define	EOS_DATA_CONTAINERS_H

//  structure used to contain rgba pixel data
// 
typedef struct eos_rgba_pixel_t
{
	unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
} eos_rgba_pixel;

// -----------------------------------------------------------------------------

//  image type enum
//
enum eos_imagetype
    {
    EOS_IMAGETYPE_GLTEXTURE,
    EOS_IMAGETYPE_RGB_BUFFER,
    EOS_IMAGETYPE_RGBA_BUFFER,   
    EOS_IMAGETYPE_CAIROSURFACE
    };
    
//  structure used to contain image data
// 
typedef struct eos_container_imagedata_t
    {
    // image data
    eos_imagetype   imageType;
    unsigned int    width;
    unsigned int    height;
    // gl texture handle
    unsigned int    glTextureHandle;
    // buffer data
    unsigned char*  buffer;
    unsigned long   bufferSize;    
    bool            bufferStatic;
    bool            bufferUpdated;
    //
    } eos_imagedata;

// -----------------------------------------------------------------------------

// structure used to contain 2d object data
//
typedef struct eos_container_2d_object_t
    {
    float   x,
            y,
            angle;
    //
    } eos_2d_object;

// -----------------------------------------------------------------------------

// structure used to share generic 3D object data between modules
//
typedef struct eos_3dvector_t
    {
    float x;
    float y;
    float z;
    //
    } eos_3dvector;

typedef struct eos_quaternion_t
    {
    float x;
    float y;
    float z;
    float w;
    //
    } eos_quaternion;
    
typedef struct eos_container_3d_object_t
    {
    eos_3dvector   position;
    eos_quaternion orientation;
    eos_3dvector   size;
    //
    } eos_3dobject;

// -----------------------------------------------------------------------------

#endif

