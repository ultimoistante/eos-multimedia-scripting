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


#include "EOSScriptManager.h"

// -----------------------------------------------------------------------------

EOSScriptManager::EOSScriptManager(const char* scriptDirectory)
	{
	this->scriptPath   = (scriptDirectory != NULL) ? scriptDirectory : "";
	}

// -----------------------------------------------------------------------------

EOSScriptManager::~EOSScriptManager()
	{
	// frees all allocated script buffers
	std::map<std::string, luaScriptBuffer*>::iterator it;
	for(it = scriptBuffers.begin(); it != scriptBuffers.end(); it++)
		{
		free(it->second->bufferPtr);
		delete it->second;
		}
	}

// -----------------------------------------------------------------------------

luaScriptBuffer* EOSScriptManager::requestScript(const char* scriptBaseName)
	{
	luaScriptBuffer* scriptBuffer = NULL;
	if(scriptBaseName != NULL)
		{
		std::map<std::string, luaScriptBuffer*>::iterator it;
		it = scriptBuffers.find(scriptBaseName);
		if(it != scriptBuffers.end()) // name found
			scriptBuffer = (*it).second;
		else
			scriptBuffer = loadScript(scriptBaseName);
		}
	return scriptBuffer;
	}

// -----------------------------------------------------------------------------

luaScriptBuffer* EOSScriptManager::loadScript(const char* scriptBaseName)
	{
	luaScriptBuffer* loadedBuffer = NULL;
	//
	if(scriptBaseName != NULL)
		{
		std::string scriptFileName = this->scriptPath;
		scriptFileName += scriptBaseName;
		scriptFileName += ".ecs";
		//
		FILE* scriptFile = fopen(scriptFileName.c_str(), "rb");
		if(scriptFile != NULL)
			{
			// reads file size
			fseek(scriptFile, 0, SEEK_END);
			unsigned long filesize = ftell(scriptFile);
			rewind(scriptFile);

			// allocates a temporary buffer to contain it
			char* fileBuffer = (char*)malloc(filesize * sizeof(char));
			// fills it with spaces
			memset(fileBuffer, 0x20, filesize);
			//
			if(fileBuffer != NULL)
				{
				// reads entire files putting it in the buffer
				if(fread(fileBuffer, sizeof(char), filesize, scriptFile) == filesize)
					{
					// inserts loaded buffer in the scriptBuffers map
					loadedBuffer = new luaScriptBuffer;
					loadedBuffer->bufferPtr = fileBuffer;
					loadedBuffer->bufferSize = filesize;
					scriptBuffers.insert(std::pair<std::string, luaScriptBuffer*>(scriptBaseName, loadedBuffer));
					}
				}
			fclose(scriptFile);
			}
		}
	return loadedBuffer;
	}

// -----------------------------------------------------------------------------
