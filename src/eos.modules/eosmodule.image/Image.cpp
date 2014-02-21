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


#include "Image.h"

// --------------------------------------------------------------------

Image::Image()
	{
	this->imageFormat          = FIF_UNKNOWN;
	this->width                = 0;
	this->height               = 0;
	this->imagePtr             = 0;
	this->imageBits            = 0;
	this->sharedImagedata.buffer = NULL;
	}
	
// --------------------------------------------------------------------

Image::~Image()
	{
    // free FreeImage's copy of the data
	if(this->imagePtr != 0)	
		FreeImage_Unload(this->imagePtr); 
	
	if(this->sharedImagedata.buffer != NULL)
		delete this->sharedImagedata.buffer;
	}
	
// --------------------------------------------------------------------

bool Image::loadFromFile(const char* filename)
	{
    // check if file exists
	if(!fileExists(filename))
        {
		printf("image file not found!\n");
		return false;
		}

	// check the file signature and deduce its format
	this->imageFormat = FreeImage_GetFileType(filename, 0);
        
    //if still unknown, try to guess the file format from the file extension
	if(this->imageFormat == FIF_UNKNOWN)
		this->imageFormat = FreeImage_GetFIFFromFilename(filename);
        
    //if still unkown, return failure
	if(this->imageFormat == FIF_UNKNOWN)
		{
		printf("unknown image format\n");
		return false;
		}
        
	// check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(this->imageFormat))
		this->imagePtr = FreeImage_Load(this->imageFormat, filename);
        
	// if the image failed to load, return failure
	if(!this->imagePtr)
		{
		printf("plugin can't read image\n");
		return false;
		}
	
    // retrieve the image data
	this->imageBits = FreeImage_GetBits(this->imagePtr);
        
	// get the image width and height
	this->width = FreeImage_GetWidth(this->imagePtr);
	this->height = FreeImage_GetHeight(this->imagePtr);
	
    // if this somehow one of these failed (they shouldn't), return failure
	if((this->imageBits == 0) || (this->width == 0) || (this->height == 0))
		{
		printf("wrong bits/width/height\n");
		return false;
		}
	
	return true;
	}

// --------------------------------------------------------------------

bool Image::resize(unsigned int newWidth, unsigned int newHeight)
	{
	printf("**** resize - params: %d, %d\n", newWidth, newHeight);
	FIBITMAP* newImagePtr = FreeImage_Rescale(this->imagePtr, newWidth, newHeight, FILTER_CATMULLROM);
	if(newImagePtr != NULL)
		{
		FreeImage_Unload(this->imagePtr);
		this->imagePtr = newImagePtr;
		printf("**** resize - dimensions before: %d, %d\n", this->width, this->height);
		this->width = newWidth;
		this->height = newHeight;
		printf("**** resize - dimensions after: %d, %d\n", this->width, this->height);
		return true;
		}
	return false;
	}

// --------------------------------------------------------------------

bool Image::fileExists(const char* filename)
    {
    if(FILE *file = fopen(filename, "r"))
        {
        fclose(file);
        return true;
        }
    return false;
    }

// --------------------------------------------------------------------

void Image::to32bpp()
	{
	// convert image to 32 bpp if not
	if(FreeImage_GetBPP(this->imagePtr) != 32)
		{
		FIBITMAP* newImagePtr = FreeImage_ConvertTo32Bits(this->imagePtr);
		if(newImagePtr != NULL)
			{
			FreeImage_Unload(this->imagePtr);
			this->imagePtr = newImagePtr;
			}
		}
	}

// --------------------------------------------------------------------

void Image::fillSharedBuffer()
	{
	// converts image data to 32 bpp, if not
	this->to32bpp();
	//
    this->sharedImagedata.imageType = EOS_IMAGETYPE_RGBA_BUFFER;
	this->sharedImagedata.bufferSize = this->width * this->height * 4;
	this->sharedImagedata.buffer = new unsigned char[this->sharedImagedata.bufferSize];
	this->sharedImagedata.width = this->width;
	this->sharedImagedata.height = this->height;    
    this->sharedImagedata.bufferStatic = true;
    this->sharedImagedata.glTextureHandle = 0;
	//
	// fills buffer with RGBA image data
	unsigned char* bufferPtr = this->sharedImagedata.buffer;
	unsigned int x, y;
	for(y=0; y<this->height; y++)
		{
		unsigned char* bits = FreeImage_GetScanLine(this->imagePtr, y);
		for(x=0; x<this->width; x++)
			{
			bufferPtr[0] = bits[FI_RGBA_RED];
			bufferPtr[1] = bits[FI_RGBA_GREEN];
			bufferPtr[2] = bits[FI_RGBA_BLUE];
			bufferPtr[3] = bits[FI_RGBA_ALPHA];
			//
			bufferPtr += 4;
			bits += 4;
			}
		}
	}

// --------------------------------------------------------------------

eos_imagedata* Image::getSharedImagebuffer()
	{
	if(this->sharedImagedata.buffer == NULL) // imagebuffer not yet generated
		fillSharedBuffer();
	//
	return &(this->sharedImagedata);
	}

// --------------------------------------------------------------------
