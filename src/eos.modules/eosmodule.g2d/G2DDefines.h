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

#ifndef PICASSODEFINES_H
#define	PICASSODEFINES_H

#include <sys/types.h>
#include <GLee.h>
#include "EOSLuaRuntime.h"
#include "../modulesdk/eos_data_containers.h"

#ifndef uint
#define uint    unsigned int
#endif

#ifndef ulong
#define ulong   unsigned long
#endif

#ifndef uchar
#define uchar   unsigned char
#endif

class G2DUtility
    {
    public:
    static inline bool checkGL(const char *func)
        {
        uint error = glGetError();
        if(error != GL_NO_ERROR)
            printf("[eos.picasso.%s] OpenGL error: %#x\n", func, error);
        return (error == GL_NO_ERROR);
        }

    static inline bool checkGLFramebuffer(uint target)
        {
        uint status = glCheckFramebufferStatus(target);
        if(status != GL_FRAMEBUFFER_COMPLETE)
            printf("[eos.picasso.checkGLFramebuffer] OpenGL FB target: %#x - status: %#x\n", target, status);
        return (status == GL_FRAMEBUFFER_COMPLETE);
        }
    };

#endif

