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


//#include <X11/Xmd.h>

#include "KinectManager.h"

// -----------------------------------------------------------------------------

bool KinectManager::instanceFlag = false;
KinectManager* KinectManager::instance = NULL;
char KinectManager::strPose[20];
char KinectManager::luaCmdBuffer[512];

// --------------------------------------------------------------------

KinectManager::KinectManager()
    {
    printf("KinectManager()\n");
    }

// --------------------------------------------------------------------

KinectManager::~KinectManager()
    {

    }

// -----------------------------------------------------------------------------

KinectManager* KinectManager::getInstance()
    {
    if (!instanceFlag)
        {
        instance = new KinectManager();
        instanceFlag = true;
        }
    return instance;
    }

// -----------------------------------------------------------------------------

bool KinectManager::powerOn()
    {
    this->nViewState = DEFAULT_DISPLAY_MODE;

    XnStatus nRetVal = XN_STATUS_OK;
    //
    // initializes context
    nRetVal = this->context.Init();
    if (nRetVal != XN_STATUS_OK)
        {
        printf("Initialize context failed: %s\n", xnGetStatusString(nRetVal));
        return false;
        }
    //
    // initializes depth generator
    nRetVal = this->depthGen.Create(this->context);
    if (nRetVal != XN_STATUS_OK)
        {
        printf("Create depth generator failed: %s\n", xnGetStatusString(nRetVal));
        return false;
        }
    //
    // initializes image generator
    nRetVal = this->imageGen.Create(this->context);
    if (nRetVal != XN_STATUS_OK)
        {
        printf("Create image generator failed: %s\n", xnGetStatusString(nRetVal));
        return false;
        }
    //
    // initializes user generator
    nRetVal = this->userGen.Create(this->context);
    if (nRetVal != XN_STATUS_OK)
        {
        printf("Create user generator failed: %s\n", xnGetStatusString(nRetVal));
        return false;
        }
    XnCallbackHandle h1, h2, h3, h4;
    this->userGen.RegisterUserCallbacks(KinectManager::User_NewUser, KinectManager::User_LostUser, NULL, h1);
    //this->userGen.GetSkeletonCap().RegisterCalibrationCallbacks(KinectManager::UserCalibration_CalibrationStart, KinectManager::UserCalibration_CalibrationComplete, NULL, h3); <- deprecated
    this->userGen.GetSkeletonCap().RegisterToCalibrationStart(KinectManager::UserCalibration_CalibrationStart, NULL, h3);
    this->userGen.GetSkeletonCap().RegisterToCalibrationComplete(KinectManager::UserCalibration_CalibrationComplete, NULL, h4);
    //this->userGen.GetPoseDetectionCap().RegisterToPoseCallbacks(KinectManager::UserPose_PoseDetected, NULL, NULL, h2);  <- deprecated
    if (this->userGen.GetSkeletonCap().NeedPoseForCalibration())
        {
        this->needPose = TRUE;
        if (!this->userGen.IsCapabilitySupported(XN_CAPABILITY_POSE_DETECTION))
            {
            printf("Pose required, but not supported\n");
            return 1;
            }
        this->userGen.GetPoseDetectionCap().RegisterToPoseDetected(UserPose_PoseDetected, NULL, h2);
        this->userGen.GetSkeletonCap().GetCalibrationPose(strPose);
        }
    this->userGen.GetSkeletonCap().SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
    //
    // start generating
    nRetVal = this->context.StartGeneratingAll();
    if (nRetVal != XN_STATUS_OK)
        {
        printf("StartGeneratingAll failed: %s\n", xnGetStatusString(nRetVal));
        return false;
        }


    this->depthGen.GetMetaData(this->depthMD);
    this->imageGen.GetMetaData(this->imageMD);

    // Hybrid mode isn't supported in this sample
    if (this->imageMD.FullXRes() != this->depthMD.FullXRes() || this->imageMD.FullYRes() != this->depthMD.FullYRes())
        {
        printf("The device depth and image resolution must be equal!\n");
        return 1;
        }

    // RGB is the only image format supported.
    if (this->imageMD.PixelFormat() != XN_PIXEL_FORMAT_RGB24)
        {
        printf("The device image format must be RGB24\n");
        return 1;
        }

    // rgba buffer init
    this->bufferWidth = (((unsigned short) (this->imageMD.FullXRes() - 1) / 512) + 1) * 512;
    this->bufferHeight = this->bufferWidth;
    //this->bufferHeight = (((unsigned short) (this->depthMD.FullYRes() - 1) / 512) + 1) * 512;
    this->rgbaBuffer = (eos_rgba_pixel*) malloc(this->bufferWidth * this->bufferHeight * sizeof (eos_rgba_pixel));

    printf("imageMD (w-h): %d - %d\n", this->imageMD.FullXRes(), this->imageMD.FullYRes());
    printf("buffer (w-h): %d - %d\n", this->bufferWidth, this->bufferHeight);

    return true;
    }

// -----------------------------------------------------------------------------

void KinectManager::shutdown()
    {
    this->depthGen.Release();
    this->imageGen.Release();
    this->context.Release();
    }

// -----------------------------------------------------------------------------

void KinectManager::update()
    {
    //XnStatus nRetVal = this->context.WaitOneUpdateAll(this->depth);
    XnStatus nRetVal = this->context.WaitAnyUpdateAll();
    if (nRetVal != XN_STATUS_OK)
        {
        printf("UpdateData failed: %s\n", xnGetStatusString(nRetVal));
        }

    this->depthGen.GetMetaData(this->depthMD);
    this->imageGen.GetMetaData(this->imageMD);

    const XnDepthPixel* pDepthMap = this->depthMD.Data();
    const XnUInt8* pImage = this->imageMD.Data();

    // does thresholding and horizontal flipping
    //RGB24* source = (RGB24*)this->imageMD.RGB24Data();
    const XnRGB24Pixel* source = this->imageMD.RGB24Data();
    eos_rgba_pixel* dest = (eos_rgba_pixel*) rgbaBuffer;
    int srcWidth = this->imageMD.FullXRes();
    int srcHeight = this->imageMD.FullYRes();
    int x, y;
    //for (y = srcHeight - 1; y >= 0; y--)
    for (y = 0; y < srcHeight; y++)
        {
        eos_rgba_pixel* lineStart = dest + (y * 1024);
        //for (x = srcWidth - 1; x >= 0; x--)
        for (x = 0; x < srcWidth; x++)
            {
            source = this->imageMD.RGB24Data() + (srcWidth * y) + x;
            lineStart->red = source->nRed;
            lineStart->green = source->nGreen;
            lineStart->blue = source->nBlue;
            lineStart->alpha = 0xff;
            lineStart++;
            /*unsigned char* dest_b = (unsigned char*) dest;
             *(dest_b) = source->nRed;
             *(dest_b + 1) = source->nGreen;
             *(dest_b + 2) = source->nBlue;
             *(dest_b + 3) = 0xff;*/
            //
            //dest++;
            }
        }
    this->shared_imagedata.bufferUpdated = true;
    /*
        unsigned int nImageScale = GL_WIN_SIZE_X / this->depthMD.FullXRes();

        xnOSMemSet(pTexMap, 0, nTexMapX * nTexMapY * sizeof (XnRGB24Pixel));

        // nei seguenti due cicli devo praticamente convertire i valori che lui metterebbe
        // nel suo buffer rgb, invece direttamente nel nostro buffer rgba e poi abbiamo finito...
    
        // check if we need to draw image frame to texture
        if (nViewState == DISPLAY_MODE_OVERLAY || nViewState == DISPLAY_MODE_IMAGE)
            {
            const XnRGB24Pixel* pImageRow = imageMD.RGB24Data();
            XnRGB24Pixel* pTexRow = pTexMap + imageMD.YOffset() * nTexMapX;

            for (XnUInt y = 0; y < imageMD.YRes(); ++y)
                {
                const XnRGB24Pixel* pImage = pImageRow;
                XnRGB24Pixel* pTex = pTexRow + imageMD.XOffset();

                for (XnUInt x = 0; x < imageMD.XRes(); ++x, ++pImage, ++pTex)
                    {
     *pTex = *pImage;
                    }

                pImageRow += imageMD.XRes();
                pTexRow += nTexMapX;
                }
            }

        // check if we need to draw depth frame to texture
        if (nViewState == DISPLAY_MODE_OVERLAY || nViewState == DISPLAY_MODE_DEPTH)
            {
            const XnDepthPixel* pDepthRow = depthMD.Data();
            XnRGB24Pixel* pTexRow = pTexMap + depthMD.YOffset() * nTexMapX;

            for (XnUInt y = 0; y < depthMD.YRes(); ++y)
                {
                const XnDepthPixel* pDepth = pDepthRow;
                XnRGB24Pixel* pTex = pTexRow + depthMD.XOffset();

                for (XnUInt x = 0; x < depthMD.XRes(); ++x, ++pDepth, ++pTex)
                    {
                    if (*pDepth != 0)
                        {
                        int nHistValue = g_pDepthHist[*pDepth];
                        pTex->nRed = nHistValue;
                        pTex->nGreen = nHistValue;
                        pTex->nBlue = 0;
                        }
                    }

                pDepthRow += depthMD.XRes();
                pTexRow += nTexMapX;
                }
            }*/
    }


// -----------------------------------------------------------------------------

EOSObject* KinectManager::getRGBFramebufferObject()
    {
    EOSObject *obj = NULL;
    obj = new EOSObject();
    obj->setType("RGBA_IMAGE_BUFFER");
    shared_imagedata.width = 1024;
    shared_imagedata.height = 1024;
    shared_imagedata.buffer = (unsigned char*) rgbaBuffer;
    shared_imagedata.bufferSize = bufferWidth * bufferHeight * sizeof (eos_rgba_pixel);
    shared_imagedata.bufferHasAlpha = false;
    shared_imagedata.bufferStatic = false; // non è statico perchè cambia... :)
    shared_imagedata.bufferUpdated = true; // questo va messo a true ogni volta che il buffer è stato aggiornato
    obj->setUserData(&shared_imagedata);
    obj->setUserDataSize(sizeof (eos_imagedata));
    //
    return obj;
    }


// -----------------------------------------------------------------------------


// Callback: New user was detected

void XN_CALLBACK_TYPE KinectManager::User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
    {
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    //printf("%d New User %d\n", epochTime, nId);
    // eos message generation
    sprintf(luaCmdBuffer, "eos.message.init(\"kinect\") eos.message.pushData(\"event\", %d) eos.message.pushData(\"userId\", %d) eos.message.pushData(\"time\", %d) eos.message.send(\"\")", EVT_NEWUSER, nId, epochTime);
    EOSLuaRuntime::getInstance()->executeLuaChunk(luaCmdBuffer);
    //
    // New user found
    if (KinectManager::getInstance()->isPoseNeeded())
        {
        generator.GetPoseDetectionCap().StartPoseDetection(strPose, nId);
        }
    else
        {
        generator.GetSkeletonCap().RequestCalibration(nId, TRUE);
        }
    }

// Callback: An existing user was lost
void XN_CALLBACK_TYPE KinectManager::User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie)
    {
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    //printf("%d Lost user %d\n", epochTime, nId);
    // eos message generation
    sprintf(luaCmdBuffer, "eos.message.init(\"kinect\") eos.message.pushData(\"event\", %d) eos.message.pushData(\"userId\", %d) eos.message.pushData(\"time\", %d) eos.message.send(\"\")", EVT_LOSTUSER, nId, epochTime);
    EOSLuaRuntime::getInstance()->executeLuaChunk(luaCmdBuffer);
    }

// Callback: Detected a pose
void XN_CALLBACK_TYPE KinectManager::UserPose_PoseDetected(xn::PoseDetectionCapability& capability, const XnChar* strPose, XnUserID nId, void* pCookie)
    {
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    printf("%d Pose %s detected for user %d\n", epochTime, strPose, nId);
    // eos message generation
    sprintf(luaCmdBuffer, "eos.message.init(\"kinect\") eos.message.pushData(\"event\", %d) eos.message.pushData(\"pose\", \"%s\") eos.message.pushData(\"userId\", %d) eos.message.pushData(\"time\", %d) eos.message.send(\"\")", EVT_POSEDETECTED, strPose, nId, epochTime);
    EOSLuaRuntime::getInstance()->executeLuaChunk(luaCmdBuffer);
    //
    KinectManager::getInstance()->getUserGenerator().GetPoseDetectionCap().StopPoseDetection(nId);
    KinectManager::getInstance()->getUserGenerator().GetSkeletonCap().RequestCalibration(nId, TRUE);
    }
// Callback: Started calibration

void XN_CALLBACK_TYPE KinectManager::UserCalibration_CalibrationStart(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie)
    {
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    //printf("%d Calibration started for user %d\n", epochTime, nId);
    // eos message generation
    sprintf(luaCmdBuffer, "eos.message.init(\"kinect\") eos.message.pushData(\"event\", %d) eos.message.pushData(\"userId\", %d) eos.message.pushData(\"time\", %d) eos.message.send(\"\")", EVT_CALIBRATIONSTART, nId, epochTime);
    EOSLuaRuntime::getInstance()->executeLuaChunk(luaCmdBuffer);
    }

void XN_CALLBACK_TYPE KinectManager::UserCalibration_CalibrationComplete(xn::SkeletonCapability& capability, XnUserID nId, XnCalibrationStatus eStatus, void* pCookie)
    {
    XnUInt32 epochTime = 0;
    xnOSGetEpochTime(&epochTime);
    if (eStatus == XN_CALIBRATION_STATUS_OK)
        {
        // Calibration succeeded
        //printf("%d Calibration complete, start tracking user %d\n", epochTime, nId);
        // eos message generation
        sprintf(luaCmdBuffer, "eos.message.init(\"kinect\") eos.message.pushData(\"event\", %d) eos.message.pushData(\"userId\", %d) eos.message.pushData(\"time\", %d) eos.message.send(\"\")", EVT_CALIBRATIONEND, nId, epochTime);
        EOSLuaRuntime::getInstance()->executeLuaChunk(luaCmdBuffer);
        //
        KinectManager::getInstance()->getUserGenerator().GetSkeletonCap().StartTracking(nId);
        }
    else
        {
        // Calibration failed
        //printf("%d Calibration failed for user %d\n", epochTime, nId);
        // eos message generation
        sprintf(luaCmdBuffer, "eos.message.init(\"kinect\") eos.message.pushData(\"event\", %d) eos.message.pushData(\"userId\", %d) eos.message.pushData(\"time\", %d) eos.message.send(\"\")", EVT_CALIBRATIONFAIL, nId, epochTime);
        EOSLuaRuntime::getInstance()->executeLuaChunk(luaCmdBuffer);
        //
        if (eStatus == XN_CALIBRATION_STATUS_MANUAL_ABORT)
            {
            printf("Manual abort occured, stop attempting to calibrate!");
            return;
            }
        if (KinectManager::getInstance()->isPoseNeeded())
            {
            KinectManager::getInstance()->getUserGenerator().GetPoseDetectionCap().StartPoseDetection(strPose, nId);
            }
        else
            {
            KinectManager::getInstance()->getUserGenerator().GetSkeletonCap().RequestCalibration(nId, TRUE);
            }
        }


    }
