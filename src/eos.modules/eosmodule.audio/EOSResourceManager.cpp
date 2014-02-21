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

#include "EOSResourceManager.h"

static EOSResourceManager rManager;

EOSResourceManager::EOSResourceManager()
{
}

EOSResourceManager::~EOSResourceManager()
{
    std::map<std::string, unsigned char*>::iterator it;
    for(it=resources.begin(); it!=resources.end(); it++)
        delete it->second;
    resources.clear();
}

EOSResourceManager *EOSResourceManager::instance()
{
	return &rManager;
}

void *EOSResourceManager::loadFile(char *filename)
{
    std::string pathfile = GETLUAVALUE("eos.application.path");
    pathfile += "/media/sounds/";
    pathfile += filename;
    std::string file = filename;
    unsigned char *resource = NULL;

    std::map<std::string, unsigned char*>::iterator it;
    it = resources.find(file);
    if(it != resources.end())
    {
        resource = (*it).second;       
	}
    else
    {
        FILE *fp = fopen(pathfile.c_str(), "rb");
        if(fp)
        {
            long size = flen(fp);
            resource = new unsigned char[size];
            fread(resource, sizeof(unsigned char), size, fp);
            fclose(fp);

            if(filename != NULL && resource != NULL)
                resources.insert(std::pair<std::string, unsigned char*>(file, resource));
        }
    }

    return resource;
}


bool EOSResourceManager::resourceExists(char* filename)
{
    std::map<std::string, unsigned char*>::iterator it;
    std::string resourceName = filename;
    it = resources.find(resourceName);
    if(it != resources.end())
    	return true;
    //
	return false;
}
