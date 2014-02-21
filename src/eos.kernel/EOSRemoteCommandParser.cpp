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


#include "EOSRemoteCommandParser.h"

// -----------------------------------------------------------------------------

EOSRemoteCommandParser::EOSRemoteCommandParser()
	{
	}

// -----------------------------------------------------------------------------

EOSRemoteCommandParser::~EOSRemoteCommandParser()
	{
	}

// -----------------------------------------------------------------------------

bool EOSRemoteCommandParser::commandIsValid(const char* command)
	{
	if(command != NULL)
		{
		if(command[1] == '|')
			{
			/*if(command[0] == 'e' || // execute
			   command[0] == 'e' || //
			  )*/
			if(command[0] == 'e') // execute
				return true;
			}
		}
	return false;
	}

// -----------------------------------------------------------------------------

const char* EOSRemoteCommandParser::getCommandArgument(const char* command)
	{
	if(command != NULL)
		return command+2;
	return NULL;
	}

// -----------------------------------------------------------------------------

const char* EOSRemoteCommandParser::parseCommand(const char* command)
	{
	if(command != NULL && commandIsValid(command))
		return getCommandArgument(command);
		/*{
		const char* commandArgument = this->getCommandArgument(command);
		EOSKernel::getInstance()->getLuaEnvironment()->runBuffer((const unsigned char*)commandArgument, strlen(commandArgument), "EOSRemoteCommandParser:remoteCommand");
		}*/
	}
