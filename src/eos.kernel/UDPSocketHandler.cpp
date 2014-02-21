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

#include "UDPSocketHandler.h"

// -----------------------------------------------------------------------------

UDPSocketHandler::UDPSocketHandler(unsigned int port)
	{
	this->socketfd = 0;
	this->valid = open(port);
	}

// -----------------------------------------------------------------------------

UDPSocketHandler::~UDPSocketHandler()
	{
	}

// -----------------------------------------------------------------------------

bool UDPSocketHandler::open(unsigned int port)
	{
   this->socketfd = socket(AF_INET, SOCK_DGRAM, 0);
   if(this->socketfd < 0)
		printf("error opening socket\n");
	//
   bzero(&addr, sizeof(addr));
   //
   addr.sin_family      = AF_INET;
   addr.sin_addr.s_addr = INADDR_ANY;
   addr.sin_port        = htons(port);
   //
   if(bind(this->socketfd, (struct sockaddr*)&addr, sizeof(addr)) > 0)
		{
		int opts = fcntl(this->socketfd, F_GETFL);
		if(opts < 0)
			printf("error: fcntl(F_GETFL)\n");
		opts = (opts | O_NONBLOCK);
		if(fcntl(this->socketfd, F_SETFL, opts) < 0)
			printf("error: fcntl(F_SETFL)\n");
		//
		return true;
		}
	//
	return false;
	}

// -----------------------------------------------------------------------------

char* UDPSocketHandler::listen(int* dataLength)
	{
	// puts socketfd in the fd_set for the select() function
	FD_ZERO(&(this->readfds));
	FD_SET(this->socketfd, &(this->readfds));
	// sets read timeout to 0
	this->read_timeout.tv_sec  = 0;
	this->read_timeout.tv_usec = 0;

	int dataAvailable = select(this->socketfd+1, &(this->readfds), NULL, NULL, &(this->read_timeout));
	if(dataAvailable > 0)
		{
		if(FD_ISSET(this->socketfd, &(this->readfds)))
			{
			*dataLength = recvfrom (this->socketfd, this->dataBuffer, MAX_DATABUFFER_SIZE, 0, (struct sockaddr*)&(this->addr), &(this->addr_len));
			if(*dataLength > 0)
				{
				this->dataBuffer[*dataLength] = '\0';
				return this->dataBuffer;
				}
			}
		}
	//
	return NULL;
	}

// -----------------------------------------------------------------------------

void UDPSocketHandler::answer(const char* what)
	{
	int n = sendto(this->socketfd, what, strlen(what), 0, (struct sockaddr*)&(this->addr), this->addr_len);
	}

// -----------------------------------------------------------------------------
