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


#include "ImageManager.h"

// --------------------------------------------------------------------

ImageManager::ImageManager()
	{	
	FreeImage_Initialise();
	}
	
// --------------------------------------------------------------------

ImageManager::~ImageManager()
	{
	FreeImage_DeInitialise();
	}
	
// --------------------------------------------------------------------

void ImageManager::clearAll()
	{
	unsigned int i;
	for(i=0; i<loadedImages.size(); i++)
		delete loadedImages[i];
	loadedImages.clear();
	//
	for(i=0; i<sessionObjects.size(); i++)
		delete sessionObjects[i];
	sessionObjects.clear();
	}

// --------------------------------------------------------------------

Image* ImageManager::loadImage(const char* filename)
	{
	Image* newImage = new Image();
	if(newImage->loadFromFile(filename))
		{
		loadedImages.push_back(newImage);
		return newImage;
		}
	else
		delete newImage;
	//
	return NULL;
	}

// --------------------------------------------------------------------

EOSObject* ImageManager::getSessionObject(Image* image)
	{
	EOSObject* sessionObject = NULL;
	if(image != NULL)
		{
		sessionObject = new EOSObject();
		sessionObject->setUserData((void*)image->getSharedImagebuffer());
		sessionObject->setUserDataSize(sizeof(eos_imagedata));
		//
		this->sessionObjects.push_back(sessionObject);
		}
	return sessionObject;
	}

// --------------------------------------------------------------------

/*Image* ImageManager::imageById(unsigned int imageId)
	{
	Image* image = NULL;
	if(imageId <= this->loadedImages.size())
		image = this->loadedImages[imageId-1];
	//
	return image;
	}*/

// --------------------------------------------------------------------
