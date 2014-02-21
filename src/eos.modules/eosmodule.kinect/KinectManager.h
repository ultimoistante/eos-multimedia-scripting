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

#ifndef EOS_KINECT_KINECTMANAGER_H
	#define EOS_KINECT_KINECTMANAGER_H

	#include "EOSObject.h"
	#include "eos_data_containers.h"
	#include "EOSLuaRuntime.h"


	#include <XnCppWrapper.h>
	#include <XnOpenNI.h>

	#include <vector>
	#include <string>

	#define DISPLAY_MODE_OVERLAY	1
	#define DISPLAY_MODE_DEPTH		2
	#define DISPLAY_MODE_IMAGE		3
	#define DEFAULT_DISPLAY_MODE	DISPLAY_MODE_IMAGE

	#define EVT_NEWUSER				1
	#define EVT_LOSTUSER				2
	#define EVT_POSEDETECTED		3
	#define EVT_CALIBRATIONSTART	4
	#define EVT_CALIBRATIONEND		5
	#define EVT_CALIBRATIONFAIL		6
	
using namespace xn;

/*typedef struct RGB24_t
	{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	} RGB24;*/

class KinectManager
	{
public:
	KinectManager();
	virtual ~KinectManager();
	static KinectManager *getInstance();
	bool powerOn();
	void shutdown();
	void update();
	//
	UserGenerator& getUserGenerator() { return this->userGen; }
	bool isPoseNeeded() { return this->needPose; }
	//
	EOSObject* getRGBFramebufferObject();

private:
	static bool instanceFlag;
	static KinectManager* instance;
	static char luaCmdBuffer[512];
	//
	Context         context;
	DepthGenerator  depthGen;
	ImageGenerator  imageGen;
	UserGenerator    userGen;
	DepthMetaData   depthMD;
	ImageMetaData   imageMD;
	unsigned int    nViewState;
	//
	eos_rgba_pixel* rgbaBuffer;
	unsigned long   rgbaBufferSize;
	unsigned int    bufferWidth;
	unsigned int    bufferHeight;
	uint            width;
	uint            height;
	eos_imagedata   shared_imagedata;
	//
	bool                 needPose;
	static char         strPose[20];
	static void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
	static void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
	static void XN_CALLBACK_TYPE UserPose_PoseDetected(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie);
	static void XN_CALLBACK_TYPE UserCalibration_CalibrationStart(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie);
	static void XN_CALLBACK_TYPE UserCalibration_CalibrationComplete(xn::SkeletonCapability& capability, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie);
	} ;

#endif
