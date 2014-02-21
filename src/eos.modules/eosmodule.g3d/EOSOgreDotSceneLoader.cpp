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


#include "EOSOgreDotSceneLoader.h"

// -----------------------------------------------------------------------------

/*EOSOgreDotSceneLoader::EOSOgreDotSceneLoader(EOSGfxModule* parentModulePtr)
        {
        this->parentModule = parentModulePtr;
        }*/

// -----------------------------------------------------------------------------

void EOSOgreDotSceneLoader::loadDotScene(const char* sceneFileName)
    {
    //printf("!!! - EOSOgreDotSceneLoader::loadDotScene() - >>>\n");
    if (sceneFileName == NULL)
        return;

    Ogre::DataStreamPtr pStream = Ogre::ResourceGroupManager::getSingleton().openResource(sceneFileName, "General");
    Ogre::String data = pStream->getAsString();
    //printf("\tdatastream opened...\n");

    XMLDocument* xmlDoc = new XMLDocument();
    xmlDoc->Parse(data.c_str());

    /* RESOURCE CLOSING REMOVED, due to Ogre 1.0.6 upgrade!
    pStream->close();
    pStream.setNull();*/

    if (!xmlDoc->Error())
        {
        XMLHandle docHandle(xmlDoc);

        /* parses <scene> element:
        <!ELEMENT scene (nodes?, externals?, environment?, terrain?, userDataReference?, octree?, light?, camera?)>
        <!ATTLIST scene
                formatVersion	CDATA	#REQUIRED
                id				ID		#IMPLIED
                sceneManager	CDATA	#IMPLIED
                minOgreVersion	CDATA	#IMPLIED
                author			CDATA	#IMPLIED
        > */

        XMLElement* element = docHandle.FirstChildElement("scene").FirstChild().ToElement();
        for (element; element; element = element->NextSiblingElement())
            {
            std::string elementValue = element->Value();
            //printf("!!! elementValue: '%s'\n", elementValue.c_str());
            //
            if (elementValue == "nodes")
                {
                XMLElement* subElement = element->FirstChildElement();
                for (subElement; subElement; subElement = subElement->NextSiblingElement())
                    {
                    std::string subElementValue = subElement->Value();
                    if (subElementValue == "node")
                        parseNodeElement(subElement);
                    }
                }
            else if (elementValue == "environment")
                parseEnvironmentElement(element);
                //
                /*
                else if(elementValue == "externals")
                        parentModule->logMessage(EOSModule::LOG_WARN, "[EOSOgreDotSceneLoader] - Warning: found an <externals> element as child of <scene>. Parsing not yet implemented, sorry. Skipping.\n");
                //
                //
                else if(elementValue == "terrain")
                        parentModule->logMessage(EOSModule::LOG_WARN, "[EOSOgreDotSceneLoader] - Warning: found a <terrain> element as child of <scene>. Parsing not yet implemented, sorry. Skipping.\n");
                //
                else if(elementValue == "octree")
                        parentModule->logMessage(EOSModule::LOG_WARN, "[EOSOgreDotSceneLoader] - Warning: found an <octree> element as child of <scene>. Parsing not yet implemented, sorry. Skipping.\n");
                //
                else if(elementValue == "light")
                        parentModule->logMessage(EOSModule::LOG_WARN, "[EOSOgreDotSceneLoader] - Warning: found a <light> element as child of <scene>. Parsing not yet implemented, sorry. Skipping.\n");
                //
                 */
            else if (elementValue == "camera")
                parseSceneCameraElement(element);
            //parentModule->logMessage(EOSModule::LOG_WARN, "[EOSOgreDotSceneLoader] - Warning: found a <camera> element as child of <scene>. Parsing not implemented, sorry. Skipping.\n");
            //
            /*
            else
                    parentModule->logMessage(EOSModule::LOG_WARN, "[EOSOgreDotSceneLoader] - Warning: unsupported element '%s' as child of <scene>. Skipping.\n", elementValue.c_str());
             */
            }
        }
    else
        EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Warning! '%s': invalid xml format. Cause: '%s' ('%s')\n", sceneFileName, xmlDoc->GetErrorStr1(), xmlDoc->GetErrorStr2());
        //parentModule->logMessage(EOSModule::LOG_WARN, "[EOSOgreDotSceneLoader] - Warning! '%s': invalid xml format.\n", sceneFileName);

    // Closes the XML File
    delete xmlDoc;

    //printf("!!! - EOSOgreDotSceneLoader::loadDotScene() - <<<\n");
    }

// -----------------------------------------------------------------------------

/** this method parses an <environment> element, in according with the DTD block below:
<!ELEMENT environment (fog?, skyBox?, skyDome?, skyPlane?, clipping?, colourAmbient?, colourBackground?, userDataReference?)>
 */
void EOSOgreDotSceneLoader::parseEnvironmentElement(XMLElement* envElement)
    {
    //EOSLuaRuntime::getInstance()->logUserMessage("!!! - EOSGfxModule::parseEnvironmentElement() - >>>");
    if (envElement != NULL)
        {
        XMLElement* subElement = envElement->FirstChildElement();
        for (subElement; subElement; subElement = subElement->NextSiblingElement())
            {
            std::string subElementValue = subElement->Value();
            //
            if (subElementValue == "fog")
                parseFogElement(subElement);
                //
            else if (subElementValue == "skyBox")
                parseSkyBoxElement(subElement);
                //
            else if (subElementValue == "skyDome")
                parseSkyDomeElement(subElement);
                //
            else if (subElementValue == "skyPlane")
                parseSkyPlaneElement(subElement);
                //
            else if (subElementValue == "colourAmbient")
                ogreWrapper->getSceneManager()->setAmbientLight(parseColourValue(subElement));
            //
            //else if(subElementValue == "clipping")
            //	parentModule->logMessage(EOSModule::LOG_WARN, "[EOSOgreDotSceneLoader] - Warning: <clipping> element as child of <environment> not yet supported. Sorry. :(\n");
            //
            //
            /* TODO
            else if(subElementValue == "colourBackground")
                    {
                    Ogre::ColourValue bgColor = parseColourValue(subElement);
                    Ogre::RenderWindow* renderWindow = parentModule->getRenderWindow();
                    for(int n = 0; n < renderWindow->getNumViewports(); n++)
                            renderWindow->getViewport(n)->setBackgroundColour(bgColor);
                    }
             */
            //
            //else
            //	parentModule->logMessage(EOSModule::LOG_WARN, "[EOSOgreDotSceneLoader] - Warning: unsupported element '%s' as child element of <node>. Skipping.\n", subElementValue.c_str());
            }
        }
    //EOSLuaRuntime::getInstance()->logUserMessage("!!! - EOSGfxModule::parseEnvironmentElement() - <<<");
    }

// -----------------------------------------------------------------------------

/** this method parses a <fog> element, in according with the DTD block below:
<!ELEMENT fog (colourDiffuse?)>
<!ATTLIST fog
        expDensity	CDATA	#DEFAULT	"0.001"
        linearStart CDATA	#DEFAULT	"0.0"
        linearEnd	CDATA	#DEFAULT	"1.0"
        mode		(none | exp | exp2 | linear) "none"
>
 */
void EOSOgreDotSceneLoader::parseFogElement(XMLElement* fogElement)
    {
    //printf("!!! - EOSGfxModule::parseFogElement() - >>>\n");
    if (fogElement != NULL)
        {
        //# attributes
        //
        Ogre::Real expDensity = 0.001;
        if (fogElement->Attribute("expDensity"))
            expDensity = Ogre::StringConverter::parseReal(fogElement->Attribute("expDensity"));
        //printf("\texpDensity: '%f'\n", expDensity);
        //
        Ogre::Real linearStart = 0.0;
        if (fogElement->Attribute("linearStart"))
            linearStart = Ogre::StringConverter::parseReal(fogElement->Attribute("linearStart"));
        //printf("\tlinearStart: '%f'\n", linearStart);
        //
        Ogre::Real linearEnd = 1.0;
        if (fogElement->Attribute("linearEnd"))
            linearEnd = Ogre::StringConverter::parseReal(fogElement->Attribute("linearEnd"));
        //printf("\tlinearEnd: '%f'\n", linearEnd);
        //
        Ogre::FogMode mode = Ogre::FOG_NONE;
        if (fogElement->Attribute("mode"))
            {
            std::string fogModeString = fogElement->Attribute("mode");
            std::transform(fogModeString.begin(), fogModeString.end(), fogModeString.begin(), toupper);
            //printf("\tfogMode: '%s'\n", fogModeString.c_str());
            if (fogModeString == "NONE")
                mode = Ogre::FOG_NONE;
            else if (fogModeString == "EXP")
                mode = Ogre::FOG_EXP;
            else if (fogModeString == "EXP2")
                mode = Ogre::FOG_EXP2;
            else if (fogModeString == "LINEAR")
                mode = Ogre::FOG_LINEAR;
            else
                EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Element <fog>, warning: unsupported fog mode '%s'. Using default value ('NONE').", fogModeString.c_str());
            //parentModule->logMessage(EOSModule::LOG_WARN, "[EOSOgreDotSceneLoader] - Element <fog>, warning: unsupported fog mode '%s'. Using default value ('none').\n", fogModeString.c_str());
            }
        //
        Ogre::ColourValue color = Ogre::ColourValue::White;
        XMLElement* subElement = fogElement->FirstChildElement("colourDiffuse");
        if (subElement)
            color = parseColourValue(subElement);
        //
        ogreWrapper->getSceneManager()->setFog(mode, color, expDensity, linearStart, linearEnd);
        }
    }

// -----------------------------------------------------------------------------

/** this method parses a <skyBox> element, in according with the DTD block below:
<!ELEMENT skyBox (rotation?)>
<!ATTLIST skyBox
        material	CDATA #REQUIRED
        distance	CDATA #DEFAULT	"5000"
        drawFirst	(true | false)	"true"
>
 */
void EOSOgreDotSceneLoader::parseSkyBoxElement(XMLElement* skyboxElement)
    {
    if (skyboxElement != NULL)
        {
        //# attributes
        //
        std::string material = "";
        if (skyboxElement->Attribute("material"))
            material = skyboxElement->Attribute("material");
        else
            {
            EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Warning! Missing required 'material' attribute in element <skyBox>. Skipping.");
            //parentModule->logMessage(EOSModule::LOG_WARN, "[EOSOgreDotSceneLoader] - Warning! Missing required 'material' attribute in element <skyBox>. Skipping.\n");
            return;
            }
        //
        Ogre::Real distance = 5000;
        if (skyboxElement->Attribute("distance"))
            distance = Ogre::StringConverter::parseReal(skyboxElement->Attribute("distance"));
        //
        bool drawFirst = true;
        if (skyboxElement->Attribute("drawFirst") && std::string(skyboxElement->Attribute("drawFirst")) == "false")
            drawFirst = false;
        //
        Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
        XMLElement* subElement = skyboxElement->FirstChildElement("rotation");
        if (subElement)
            rotation = parseQuaternion(subElement);
        //
        //
        ogreWrapper->getSceneManager()->setSkyDome(false, "");
        ogreWrapper->getSceneManager()->setSkyPlane(false, Ogre::Plane(), "");
        ogreWrapper->getSceneManager()->setSkyBox(true, material, distance, drawFirst, rotation);
        //EOSLuaRuntime::getInstance()->logInfoMessage("[EOSOgreDotSceneLoader] - Loaded SkyBox: %s, %f", material.c_str(), distance);
        }
    }

// -----------------------------------------------------------------------------

/** this method parses a <skyDome> element, in according with the DTD block below:
<!ELEMENT skyDome (rotation?)>
<!ATTLIST skyDome
        material	CDATA #REQUIRED
        curvature	CDATA #DEFAULT	"10"
        tiling		CDATA #DEFAULT	"8"
        distance	CDATA #DEFAULT	"4000"
        drawFirst	(true | false) "true"
>
 */
void EOSOgreDotSceneLoader::parseSkyDomeElement(XMLElement* skydomeElement)
    {
    if (skydomeElement != NULL)
        {
        //# attributes
        //
        std::string material = "";
        if (skydomeElement->Attribute("material"))
            material = skydomeElement->Attribute("material");
        else
            {
            EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Warning! Missing required 'material' attribute in element <skyDome>. Skipping.");
            //parentModule->logMessage(EOSModule::LOG_WARN, "[EOSOgreDotSceneLoader] - Warning! Missing required 'material' attribute in element <skyDome>. Skipping.\n");
            return;
            }
        //
        Ogre::Real curvature = 10;
        if (skydomeElement->Attribute("curvature"))
            curvature = Ogre::StringConverter::parseReal(skydomeElement->Attribute("curvature"));
        //
        Ogre::Real tiling = 8;
        if (skydomeElement->Attribute("tiling"))
            tiling = Ogre::StringConverter::parseReal(skydomeElement->Attribute("tiling"));
        //
        Ogre::Real distance = 4000;
        if (skydomeElement->Attribute("distance"))
            distance = Ogre::StringConverter::parseReal(skydomeElement->Attribute("distance"));
        //
        bool drawFirst = true;
        if (skydomeElement->Attribute("drawFirst") && std::string(skydomeElement->Attribute("drawFirst")) == "false")
            drawFirst = false;
        //
        Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
        XMLElement* subElement = skydomeElement->FirstChildElement("rotation");
        if (subElement)
            rotation = parseQuaternion(subElement);
        //
        //
        ogreWrapper->getSceneManager()->setSkyBox(false, "");
        ogreWrapper->getSceneManager()->setSkyPlane(false, Ogre::Plane(), "");
        ogreWrapper->getSceneManager()->setSkyDome(true, material, curvature, tiling, distance, drawFirst, rotation);
        }
    }

// -----------------------------------------------------------------------------

/** this method parses a <skyPlane> element, in according with the DTD block below:
<!ELEMENT skyPlane EMPTY>
<!ATTLIST skyPlane
        material	CDATA	#REQUIRED
        planeX		CDATA	#DEFAULT	"0"
        planeY		CDATA	#DEFAULT	"-1"
        planeZ		CDATA	#DEFAULT	"0"
        planeD		CDATA	#DEFAULT	"5000"
        scale		CDATA	#DEFAULT	"1000"
        bow			CDATA	#DEFAULT	"0"
        tiling		CDATA	#DEFAULT	"10"
        drawFirst	(true | false)		"true"
>
 */
void EOSOgreDotSceneLoader::parseSkyPlaneElement(XMLElement* skyplaneElement)
    {
    if (skyplaneElement != NULL)
        {
        //# attributes
        //
        std::string material = "";
        if (skyplaneElement->Attribute("material"))
            material = skyplaneElement->Attribute("material");
        else
            {
            EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Warning! Missing required 'material' attribute in element <skyPlane>. Skipping.");
            //parentModule->logMessage(EOSModule::LOG_WARN, "[EOSOgreDotSceneLoader] - Warning! Missing required 'material' attribute in element <skyPlane>. Skipping.\n");
            return;
            }
        //
        Ogre::Real planeX = 0;
        if (skyplaneElement->Attribute("planeX"))
            planeX = Ogre::StringConverter::parseReal(skyplaneElement->Attribute("planeX"));
        //
        Ogre::Real planeY = -1;
        if (skyplaneElement->Attribute("planeY"))
            planeY = Ogre::StringConverter::parseReal(skyplaneElement->Attribute("planeY"));
        //
        Ogre::Real planeZ = 0;
        if (skyplaneElement->Attribute("planeZ"))
            planeZ = Ogre::StringConverter::parseReal(skyplaneElement->Attribute("planeZ"));
        //
        Ogre::Real planeD = 5000;
        if (skyplaneElement->Attribute("planeD"))
            planeD = Ogre::StringConverter::parseReal(skyplaneElement->Attribute("planeD"));
        //
        Ogre::Real scale = 1000;
        if (skyplaneElement->Attribute("scale"))
            scale = Ogre::StringConverter::parseReal(skyplaneElement->Attribute("scale"));
        //
        Ogre::Real bow = 0;
        if (skyplaneElement->Attribute("bow"))
            bow = Ogre::StringConverter::parseReal(skyplaneElement->Attribute("bow"));
        //
        Ogre::Real tiling = 10;
        if (skyplaneElement->Attribute("tiling"))
            tiling = Ogre::StringConverter::parseReal(skyplaneElement->Attribute("tiling"));
        //
        bool drawFirst = true;
        if (skyplaneElement->Attribute("drawFirst") && std::string(skyplaneElement->Attribute("drawFirst")) == "false")
            drawFirst = false;
        //
        //
        ogreWrapper->getSceneManager()->setSkyBox(false, "");
        ogreWrapper->getSceneManager()->setSkyDome(false, "");
        ogreWrapper->getSceneManager()->setSkyPlane(true, Ogre::Plane(Ogre::Vector3(planeX, planeY, planeZ), planeD), material, scale, tiling, drawFirst, bow);
        }
    }

// -----------------------------------------------------------------------------

/** this method parses a <camera> tag, in according with the DTD block below:

<!ELEMENT camera (clipping?, position?, rotation?, normal?, lookTarget?, trackTarget?, userDataReference?)>
<!ATTLIST camera
        name			CDATA	#IMPLIED
        id				ID		#IMPLIED
        fov				CDATA	#IMPLIED
        aspectRatio		CDATA	#IMPLIED
        projectionType	(perspective | orthographic)	"perspective"
>
 */
void EOSOgreDotSceneLoader::parseSceneCameraElement(XMLElement* cameraElement)
    {
    //printf("!!! - EOSGfxModule::parseSceneCameraElement() - >>>\n");
    if (cameraElement != NULL)
        {
        Ogre::SceneManager* currentSceneManager = ogreWrapper->getSceneManager();
        if (currentSceneManager != NULL)
            {
            // reads camera name
            std::string cameraName = cameraElement->Attribute("name");
            //printf("camera name: '%s'\n", cameraName.c_str());

            // creates the camera (if not already created)
            Ogre::Camera* theCamera = ogreWrapper->getDefaultCamera();
            if (theCamera == NULL)
                theCamera = currentSceneManager->createCamera(cameraName);

            //
            //# parses other camera attributes
            // camera fov...
            Ogre::Real value = 0;
            value = Ogre::StringConverter::parseReal((cameraElement->Attribute("fov") != NULL) ? cameraElement->Attribute("fov") : "0");
            theCamera->setFOVy(Ogre::Angle(value));
            //printf("FOV: '%f'\n", value);
            // camera aspect ratio...
            value = Ogre::StringConverter::parseReal((cameraElement->Attribute("aspectRatio") != NULL) ? cameraElement->Attribute("aspectRatio") : "1.3333");
            theCamera->setAspectRatio(value);
            //printf("AspectRatio: '%f'\n", value);
            // camera projection type...
            std::string projectionType = (cameraElement->Attribute("projectionType") != NULL) ? cameraElement->Attribute("projectionType") : "perspective";
            //printf("ProjectionType: '%s'\n", projectionType.c_str());
            if (projectionType == "perspective")
                theCamera->setProjectionType(Ogre::PT_PERSPECTIVE);
            else
                theCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);


            //std::string cameraType = cameraElement->Attribute("projectionType") == NULL ? "" : pNode->Attribute("projectionType");


            //
            //
            XMLElement* subElement = cameraElement->FirstChildElement();
            for (subElement; subElement; subElement = subElement->NextSiblingElement())
                {
                std::string subElementValue = subElement->Value();
                //
                if (subElementValue == "clipping")
                    {
                    Ogre::Real nearClip = 0;
                    Ogre::Real farClip = 0;
                    parseClipping(subElement, nearClip, farClip);
                    if (nearClip != 0)
                        {
                        theCamera->setNearClipDistance(nearClip);
                        //printf("NEAR clipping: '%f'\n", nearClip);
                        }
                    if (farClip != 0)
                        {
                        theCamera->setFarClipDistance(farClip);
                        //printf("FAR  clipping: '%f'\n", farClip);
                        }
                    }
                    //
                else if (subElementValue == "position")
                    theCamera->setPosition(parseVector3(subElement));
                    //
                else if (subElementValue == "rotation")
                    theCamera->setOrientation(parseQuaternion(subElement));
                    //
                else if (subElementValue == "normal")
                    theCamera->setDirection(parseVector3(subElement));
                    //
                else if (subElementValue == "lookTarget")
                    {
                    // todo
                    }
                    //
                else if (subElementValue == "trackTarget")
                    {
                    // todo
                    }
                    //
                else if (subElementValue == "userDataReference")
                    {
                    // todo
                    }

                /*
                // do fov and aspect ratio
                Real num = 0;
                if (this->_getAttribute(pNode, "fov", num))
                        pCam->setFOVy(Ogre::Angle(num));
                if (this->_getAttribute(pNode, "aspectRatio", num))
                        pCam->setAspectRatio(num);
                // do type
                if (type == "perspective")
                        pCam->setProjectionType(Ogre::PT_PERSPECTIVE);
                else
                        pCam->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
                // normal (direction)
                pElem = pNode->FirstChildElement("normal");
                if (pElem)
                        pCam->setDirection(xml::readNormal(pElem));
                // do position
                pElem = pNode->FirstChildElement("position");
                if (pElem)
                        pCam->setPosition(xml::readPosition(pElem));
                // lookAt target node
                XMLElement *laElem = pNode->FirstChildElement("lookTarget");
                if (laElem)
                        this->_doCamLTarget(laElem, pCam);
                // track target node
                XMLElement *taElem = pNode->FirstChildElement("trackTarget");
                if (taElem)
                        this->_doCamTTarget(taElem, pCam);
                // update the dotSceneInfo graph
                this->_updateDSI(pCam, _dslCurrPos, _dslCurrRot, _dslCurrSca, false);
                 */
                }
            }
        }
    //printf("!!! - EOSGfxModule::parseSceneCameraElement() - <<<\n");
    }

// -----------------------------------------------------------------------------

/** this method parses a <node> tag, in according with the DTD block below:

<!ELEMENT node (position?, rotation?, scale?, lookTarget?, trackTarget?, node*, entity*, light*, camera*, particleSystem*, billboardSet*, plane*, userDataReference?)>
<!ATTLIST node
        name		CDATA	#IMPLIED
        id			ID		#IMPLIED
        isTarget	(true | false) "true"
>
 */
void EOSOgreDotSceneLoader::parseNodeElement(XMLElement* nodeElement)
    {
    if (nodeElement != NULL)
        {
        std::string nodeName = nodeElement->Attribute("name");
        //parentModule->logMessage(EOSModule::LOG_INFO, "[EOSOgreDotSceneLoader] - parsing node '%s'...\n", nodeName.c_str());

        // First create the new scene node
        CachedSceneNode* cachedSceneNode = ogreWrapper->requestCachedSceneNode(nodeName.c_str());
        //Ogre::SceneNode* newSceneNode = static_cast<Ogre::SceneNode*>( ogreWrapper->getSceneManager()->getRootSceneNode()->createChild( nodeName ) );

        XMLElement* subElement = nodeElement->FirstChildElement();
        for (subElement; subElement; subElement = subElement->NextSiblingElement())
            {
            std::string subElementValue = subElement->Value();
            //
            if (subElementValue == "position")
                cachedSceneNode->sceneNode->setPosition(parseVector3(subElement));
                //
            else if (subElementValue == "rotation")
                cachedSceneNode->sceneNode->setOrientation(parseQuaternion(subElement));
                //
            else if (subElementValue == "scale")
                cachedSceneNode->sceneNode->setScale(parseVector3(subElement));
                //
            else if (subElementValue == "lookTarget")
                EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - oops... found <lookTarget> as child of <node> element: parsing not yet implemented, sorry. :(");
                //
            else if (subElementValue == "trackTarget")
                EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - oops... found <trackTarget> as child of <node> element: parsing not yet implemented, sorry. :(");
                //
            else if (subElementValue == "node")
                EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - oops... found a <node> inside other <node> element: nested loading not yet implemented, sorry. :(");
                //
            else if (subElementValue == "entity")
                parseEntityElement(subElement, cachedSceneNode);
                //
            else if (subElementValue == "light")
                parseLightElement(subElement, cachedSceneNode);
                //
            else if (subElementValue == "camera")
                EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - oops... found <camera> element as child of <node>: parsing not yet implemented, sorry. :(");
                //
            else if (subElementValue == "particleSystem")
                parseParticleSystemElement(subElement, cachedSceneNode);
                //
            else if (subElementValue == "billboardSet")
                parseBillboardSetElement(subElement, cachedSceneNode);
                //
            else if (subElementValue == "plane")
                EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - oops... found <plane> element as child of <node>: parsing not yet implemented, sorry. :(");
                //
            else
                EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Warning: unsupported element '%s' as child element of <node>. Skipping.", subElementValue.c_str());
            }
        //EOSLuaRuntime::getInstance()->logInfoMessage("[EOSOgreDotSceneLoader] - ...'%s' node parsed.", nodeName.c_str());
        }
    //printf("!!! - EOSGfxModule::parseSceneNode() - <<<\n");
    }

// -----------------------------------------------------------------------------

/** this method parses an "entity" element:
<!ELEMENT entity (vertexBuffer?, indexBuffer?, userDataReference?)>
<!ATTLIST entity
        name			CDATA	#IMPLIED
        id				ID		#IMPLIED
        meshFile		CDATA	#REQUIRED
        materialFile	CDATA	#IMPLIED
        static			(true | false) "false"
        castShadows		(true | false) "true"
>
 */
void EOSOgreDotSceneLoader::parseEntityElement(XMLElement* entityElement, CachedSceneNode* cachedSceneNode)
    {
    if (entityElement == NULL || cachedSceneNode == NULL)
        return;

    std::string entityName = "";
    if (entityElement->Attribute("name"))
        entityName = entityElement->Attribute("name");
    //
    std::string meshFile = "";
    if (entityElement->Attribute("meshFile"))
        meshFile = entityElement->Attribute("meshFile");
    else
        {
        EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Warning! Missing required 'meshFile' attribute in element <entity>. Skipping.");
        return;
        }
    //
    ogreWrapper->objectLoad(entityName.c_str(), meshFile.c_str(), cachedSceneNode);
    //if(subElement->Attribute("static") && std::string(subElement->Attribute("static")) == "true")
    //	parentModule->setObjectStatic(objectName.c_str(), true);
    bool castShadows = true;
    if (entityElement->Attribute("castsShadow") && std::string(entityElement->Attribute("castsShadow")) == "false")
        castShadows = false;
    ogreWrapper->setObjectCastsShadow(entityName.c_str(), castShadows);
    //
    XMLElement* subElement = entityElement->FirstChildElement();
    for (subElement; subElement; subElement = subElement->NextSiblingElement())
        {
        std::string subElementValue = subElement->Value();
        //
        if (subElementValue == "vertexBuffer")
            EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - oops... found a <vertexBuffer> inside an <entity> element: loading not yet implemented, sorry. :(");
            //
        else if (subElementValue == "indexBuffer")
            EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - oops... found an <indexBuffer> inside an <entity> element: loading not yet implemented, sorry. :(");
            //
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Warning: unsupported element '%s' as child element of <entity>. Skipping.", subElementValue.c_str());
        }
    }

// -----------------------------------------------------------------------------

/** this method parses a "light" element:
<!ELEMENT light (position?, normal?, colourDiffuse?, colourSpecular?, lightRange?, lightAttenuation?, userDataReference?)>
<!ATTLIST light
        name			CDATA	#IMPLIED
        id				ID		#IMPLIED
        type			(point | directional | spot | radPoint) "point"
        visible			(true | false) "true"
        castShadows		(true | false) "true"
>
 */
void EOSOgreDotSceneLoader::parseLightElement(XMLElement* lightElement, CachedSceneNode* cachedSceneNode)
    {
    //printf("!!! - parseLightElement() - >>>\n");
    if (lightElement == NULL || cachedSceneNode == NULL)
        return;

    // Creates the light
    Ogre::Light* light = ogreWrapper->getSceneManager()->createLight(lightElement->Attribute("name"));
    if (light == NULL)
        return;
    cachedSceneNode->sceneNode->attachObject(light);
    //
    //
    // # node attributes:
    //
    // light type (point | directional | spot | radPoint) "point"
    std::string lightType = lightElement->Attribute("type");
    light->setType(Ogre::Light::LT_POINT); // default
    if (lightType == "directional")
        light->setType(Ogre::Light::LT_DIRECTIONAL);
    else if (lightType == "spot")
        light->setType(Ogre::Light::LT_SPOTLIGHT);
    else if (lightType == "radPoint")
        light->setType(Ogre::Light::LT_POINT);
    //
    // light visibility (true | false) "true"
    light->setVisible(true); // default
    if (lightElement->Attribute("visible") && std::string(lightElement->Attribute("visible")) == "false")
        light->setVisible(false);
    //
    // castShadows		(true | false) "true"
    light->setCastShadows(true); // default
    if (lightElement->Attribute("castShadows") && std::string(lightElement->Attribute("castShadows")) == "false")
        light->setCastShadows(false);
    //
    //
    // # light subelements:
    XMLElement* subElement = lightElement->FirstChildElement();
    for (subElement; subElement; subElement = subElement->NextSiblingElement())
        {
        std::string subElementValue = subElement->Value();
        //
        if (subElementValue == "position")
            light->setPosition(parseVector3(subElement));
            //
        else if (subElementValue == "normal")
            EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - oops... found a <normal> inside a <light> element: loading not yet implemented, sorry. :(");
            //
        else if (subElementValue == "colourDiffuse")
            light->setDiffuseColour(parseColourValue(subElement));
            //
        else if (subElementValue == "colourSpecular")
            light->setSpecularColour(parseColourValue(subElement));
            //
        else if (subElementValue == "lightRange")
            EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - oops... found a <lightRange> inside a <light> element: loading not yet implemented, sorry. :(");
            //
        else if (subElementValue == "lightAttenuation")
            {
            //get defaults in case not all values specified
            Ogre::Real range, constant, linear, quadratic;
            range = light->getAttenuationRange();
            constant = light->getAttenuationConstant();
            linear = light->getAttenuationLinear();
            quadratic = light->getAttenuationQuadric();
            //
            if (subElement->Attribute("range"))
                range = Ogre::StringConverter::parseReal(subElement->Attribute("range"));
            if (subElement->Attribute("constant"))
                constant = Ogre::StringConverter::parseReal(subElement->Attribute("constant"));
            if (subElement->Attribute("linear"))
                linear = Ogre::StringConverter::parseReal(subElement->Attribute("linear"));
            if (subElement->Attribute("quadratic"))
                quadratic = Ogre::StringConverter::parseReal(subElement->Attribute("quadratic"));
            //
            light->setAttenuation(range, constant, linear, quadratic);
            }
            //
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Warning: unsupported element '%s' as child element of <light>. Skipping.", subElementValue.c_str());
        }
    //printf("!!! - parseLightElement() - <<<\n");
    }

// -----------------------------------------------------------------------------

/** this method parses a "camera" element:
<!ELEMENT camera (clipping?, position?, rotation?, normal?, lookTarget?, trackTarget?, userDataReference?)>
<!ATTLIST camera
        name			CDATA	#IMPLIED
        id				ID		#IMPLIED
        fov				CDATA	#IMPLIED
        aspectRatio		CDATA	#IMPLIED
        projectionType	(perspective | orthographic)	"perspective"
>
 */
void EOSOgreDotSceneLoader::parseCameraElement(XMLElement* cameraElement, CachedSceneNode* cachedSceneNode)
    {
    if (cameraElement == NULL || cachedSceneNode == NULL)
        return;
    EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Warning: unsupported element <camera> as child element of <node>. Skipping.");
    }


// -----------------------------------------------------------------------------

/** this method parses a "particleSystem" element:
<!ELEMENT particleSystem (userDataReference?)>
<!ATTLIST particleSystem
        name	CDATA	#IMPLIED
        id		ID		#IMPLIED
        file	CDATA	#REQUIRED
>
 */
void EOSOgreDotSceneLoader::parseParticleSystemElement(XMLElement* particleSystemElement, CachedSceneNode* cachedSceneNode)
    {
    if (particleSystemElement == NULL || cachedSceneNode == NULL)
        return;

    std::string name = "";
    if (particleSystemElement->Attribute("name"))
        name = particleSystemElement->Attribute("name");
    std::string file = "";
    if (particleSystemElement->Attribute("file"))
        file = particleSystemElement->Attribute("file");
    else
        {
        EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Warning! Missing required 'file' attribute in element <particleSystem>. Skipping.");
        return;
        }
    ogreWrapper->particleSystemLoad(name.c_str(), file.c_str(), cachedSceneNode);
    }


// -----------------------------------------------------------------------------

/** this method parses a "billboardSet" element:
<!ELEMENT billboardSet (billboard*)>
<!ATTLIST billboardSet
        name		CDATA	#REQUIRED
        material	CDATA	#REQUIRED
        id			ID		#IMPLIED
        width		CDATA	#DEFAULT	"10"
        height		CDATA	#DEFAULT	"10"
        type		(orientedCommon | orientedSEOS | point) "point"
        origin		(bottomLeft | bottomCenter | bottomRight | left | center | right | topLeft | topCenter | topRight) "center"
>

<!ELEMENT billboard (position?, rotation?, colourDiffuse?)>
<!ATTLIST billboard
        id ID #IMPLIED
        width CDATA #IMPLIED
        height CDATA #IMPLIED
>
 */
void EOSOgreDotSceneLoader::parseBillboardSetElement(XMLElement* billboardSetElement, CachedSceneNode* cachedSceneNode)
    {
    if (billboardSetElement == NULL || cachedSceneNode == NULL)
        return;

    //printf("!!! - parseBillboardSetElement() - reading attributes...\n");
    //# billboard set attributes loading...
    //
    std::string name = "";
    if (billboardSetElement->Attribute("name"))
        name = billboardSetElement->Attribute("name");
    else
        {
        EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Warning! Missing required 'name' attribute in element <billboardSet>. Skipping.");
        return;
        }
    //
    std::string material = "";
    if (billboardSetElement->Attribute("material"))
        material = billboardSetElement->Attribute("material");
    else
        {
        EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Warning! Missing required 'material' attribute in element <billboardSet>. Skipping.");
        return;
        }
    //
    Ogre::Real width = 10; // <-- default value
    if (billboardSetElement->Attribute("width"))
        width = Ogre::StringConverter::parseReal(billboardSetElement->Attribute("width"));
    // height
    Ogre::Real height = 10; // <-- default value
    if (billboardSetElement->Attribute("height"))
        height = Ogre::StringConverter::parseReal(billboardSetElement->Attribute("height"));
    // type
    Ogre::BillboardType bt = Ogre::BBT_POINT;
    if (billboardSetElement->Attribute("type"))
        {
        std::string type = billboardSetElement->Attribute("type");
        if (type == "point")
            bt = Ogre::BBT_POINT;
        else if (type == "orientedCommon")
            bt = Ogre::BBT_ORIENTED_COMMON;
        else if (type == "orientedSEOS")
            bt = Ogre::BBT_ORIENTED_SELF;
        else
            {
            EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Warning: unknown BillboardSet type '%s'. Skipping.", type.c_str());
            return;
            }
        }
    //
    Ogre::BillboardOrigin bo = Ogre::BBO_CENTER;
    if (billboardSetElement->Attribute("origin"))
        {
        std::string origin = billboardSetElement->Attribute("origin");
        if (origin == "topLeft")
            bo = Ogre::BBO_TOP_LEFT;
        else if (origin == "topCenter")
            bo = Ogre::BBO_TOP_CENTER;
        else if (origin == "topRight")
            bo = Ogre::BBO_TOP_RIGHT;
        else if (origin == "centerLeft")
            bo = Ogre::BBO_CENTER_LEFT;
        else if (origin == "center")
            bo = Ogre::BBO_CENTER;
        else if (origin == "centerRight")
            bo = Ogre::BBO_CENTER_RIGHT;
        else if (origin == "bottomLeft")
            bo = Ogre::BBO_BOTTOM_LEFT;
        else if (origin == "bottomCenter")
            bo = Ogre::BBO_BOTTOM_CENTER;
        else if (origin == "bottomRight")
            bo = Ogre::BBO_BOTTOM_RIGHT;
        else
            EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Warning: unknown BillboardSet origin '%s'. Skipping.", origin.c_str());
        }
    //
    //# creates billboard set
    /*
    How to add a billboard/sprite to the scene
    A Billboard is a square polygon that is always pointed at the camera.
    It is also known as a sprite. To make one, you must first make a BillboardSet.
    Then the billboard can be added to it on a given position. The BillboardSet
    is a MovableObject and should therefore be added to a SceneNode.
    The whole procedure is as follows:
                    SceneNode* myNode = static_cast<SceneNode*>(mSceneMgr->getRootSceneNode()->createChild());
                    BillboardSet* mySet = mSceneMgr->createBillboardSet("mySet");
                    Billboard* myBillboard = mySet->createBillboard(Vector3(100, 0, 200));
                    myNode->attachObject(mySet);
     */
    //EOSOgreObject* newOgreObject = new EOSOgreObject(name.c_str(), ogreWrapper->getSceneManager(), parentModule->getPhysicWorld());
    EOSOgreObject* newOgreObject = new EOSOgreObject(name.c_str(), ogreWrapper->getSceneManager());
    newOgreObject->attachBillboardSet(ogreWrapper->getSceneManager()->createBillboardSet(name));
    if (newOgreObject->getBillboardSet() != NULL)
        {
        newOgreObject->getBillboardSet()->setBillboardType(bt);
        newOgreObject->getBillboardSet()->setBillboardOrigin(bo);
        newOgreObject->getBillboardSet()->setDefaultWidth(width);
        newOgreObject->getBillboardSet()->setDefaultHeight(height);
        newOgreObject->getBillboardSet()->setMaterialName(material);
        //newOgreObject->sceneNode = static_cast<Ogre::SceneNode*>(parentModule->getSceneManager()->getRootSceneNode()->createChild());
        newOgreObject->cachedSceneNode = cachedSceneNode;
        //newOgreObject->sceneNode->setPosition(0, 0, 0);
        newOgreObject->cachedSceneNode->sceneNode->attachObject(newOgreObject->getBillboardSet());
        //
        // billboard subnodes parsing
        XMLElement* billboardElement = billboardSetElement->FirstChildElement();
        for (billboardElement; billboardElement; billboardElement = billboardElement->NextSiblingElement())
            {
            std::string elementValue = billboardElement->Value();
            if (elementValue == "billboard")
                {
                // dimensions
                width = Ogre::StringConverter::parseReal(billboardElement->Attribute("width"));
                height = Ogre::StringConverter::parseReal(billboardElement->Attribute("height"));
                // position
                Ogre::Vector3 position = Ogre::Vector3::ZERO;
                XMLElement* subElement = billboardElement->FirstChildElement("position");
                if (subElement != NULL)
                    position = parseVector3(subElement);
                // diffuse color
                Ogre::ColourValue diffuseColor = Ogre::ColourValue::White;
                subElement = billboardElement->FirstChildElement("colourDiffuse");
                if (subElement != NULL)
                    diffuseColor = parseColourValue(subElement);
                // rotation
                Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY;
                subElement = billboardElement->FirstChildElement("rotation");
                if (subElement != NULL)
                    rotation = parseQuaternion(subElement);
                //
                // finally creates the billboard
                Ogre::Billboard* billboard = newOgreObject->getBillboardSet()->createBillboard(position, diffuseColor);
                billboard->setDimensions(width, height);
                billboard->setRotation(Ogre::Angle(rotation.w));
                }
            else
                {
                EOSLuaRuntime::getInstance()->logWarningMessage("[EOSOgreDotSceneLoader] - Warning: found unsupported element type '%s' as child of 'billboardSet'. Skipping.", elementValue.c_str());
                }
            }
        }
    }

// -----------------------------------------------------------------------------

/*bool EOSOgreDotSceneLoader::getAttribute(XMLElement* element, const char* attributeName, std::string* recipientString)
        {
        if(element != NULL && attributeName != NULL && recipientString != NULL)
                {
                if(element->Attribute(attributeName))
                        {
 *recipientString = element->Attribute(attributeName);
                        return true;
                        }
                }
        return false;
        }*/

// -----------------------------------------------------------------------------

/** this method parses a vector3 element like this:
<!ELEMENT position EMPTY>
<!ATTLIST position
        x CDATA #REQUIRED
        y CDATA #REQUIRED
        z CDATA #REQUIRED
>
 */
Ogre::Vector3& EOSOgreDotSceneLoader::parseVector3(XMLElement* vector3Element)
    {
    if (vector3Element != NULL)
        {
        tempVector3.x = Ogre::StringConverter::parseReal(vector3Element->Attribute("x"));
        tempVector3.y = Ogre::StringConverter::parseReal(vector3Element->Attribute("y"));
        tempVector3.z = Ogre::StringConverter::parseReal(vector3Element->Attribute("z"));
        }
    return this->tempVector3;
    }

// -----------------------------------------------------------------------------

Ogre::Quaternion& EOSOgreDotSceneLoader::parseQuaternion(XMLElement* quaternionElement)
    {
    this->tempQuaternion = Ogre::Quaternion::IDENTITY;
    if (quaternionElement == NULL)
        return this->tempQuaternion;

    if (quaternionElement->Attribute("qx") != NULL)
        {
        tempQuaternion.x = Ogre::StringConverter::parseReal(quaternionElement->Attribute("qx"));
        tempQuaternion.y = Ogre::StringConverter::parseReal(quaternionElement->Attribute("qy"));
        tempQuaternion.z = Ogre::StringConverter::parseReal(quaternionElement->Attribute("qz"));
        tempQuaternion.w = Ogre::StringConverter::parseReal(quaternionElement->Attribute("qw"));
        }
    else if (quaternionElement->Attribute("axisx") != NULL)
        {
        /* OLD CODE
        //parentModule->logMessage(EOSModule::LOG_INFO, "[EOSOgreDotSceneLoader] - reading single axis orientation...\n");
        Ogre::Vector3 axis;
        axis.x = Ogre::StringConverter::parseReal(quaternionElement->Attribute("axisx"));
        axis.y = Ogre::StringConverter::parseReal(quaternionElement->Attribute("axisy"));
        axis.z = Ogre::StringConverter::parseReal(quaternionElement->Attribute("axisz"));
        tempQuaternion.FromAxes(&axis);
        //parentModule->logMessage(EOSModule::LOG_INFO, "...done\n");
         */
        /* NEW CODE: based on SceneNode::setDirection */
        Ogre::Vector3 axis;
        axis.x = Ogre::StringConverter::parseReal(quaternionElement->Attribute("axisx"));
        axis.y = Ogre::StringConverter::parseReal(quaternionElement->Attribute("axisy"));
        axis.z = Ogre::StringConverter::parseReal(quaternionElement->Attribute("axisz"));

        // Adjust vector so that it is relative to local Z
        Ogre::Vector3 zAdjustVec;
        zAdjustVec = -axis;
        zAdjustVec.normalise();

        //Ogre::Quaternion targetOrientation;
        // Get axes from current quaternion
        this->tempQuaternion = Ogre::Quaternion::IDENTITY;
        Ogre::Vector3 axes[3];
        this->tempQuaternion.ToAxes(axes);
        Ogre::Quaternion rotQuaternion;
        if ((axes[2] + zAdjustVec).squaredLength() < 0.00005f)
            {
            // Oops, a 180 degree turn (infinite possible rotation axes)
            // Default to yaw i.e. use current UP
            //Ogre::Real dummy = PI;
            rotQuaternion.FromAngleAxis(Ogre::Radian(Ogre::Math::PI), axes[1]);
            }
        else
            {
            // Derive shortest arc to new direction
            rotQuaternion = axes[2].getRotationTo(zAdjustVec);
            }
        this->tempQuaternion = rotQuaternion * this->tempQuaternion;

        }
    else if (quaternionElement->Attribute("anglex") != NULL)
        {
        Ogre::Vector3 axis;
        axis.x = Ogre::StringConverter::parseReal(quaternionElement->Attribute("anglex"));
        axis.y = Ogre::StringConverter::parseReal(quaternionElement->Attribute("anglex"));
        axis.z = Ogre::StringConverter::parseReal(quaternionElement->Attribute("anglex"));
        Ogre::Real angle = Ogre::StringConverter::parseReal(quaternionElement->Attribute("angle"));
        ;
        tempQuaternion.FromAngleAxis(Ogre::Angle(angle), axis);
        }
    //
    return this->tempQuaternion;
    }

// -----------------------------------------------------------------------------

Ogre::ColourValue& EOSOgreDotSceneLoader::parseColourValue(XMLElement* colourValueElement)
    {
    if (colourValueElement != NULL)
        {
        tempColourValue.r = Ogre::StringConverter::parseReal(colourValueElement->Attribute("r"));
        tempColourValue.g = Ogre::StringConverter::parseReal(colourValueElement->Attribute("g"));
        tempColourValue.b = Ogre::StringConverter::parseReal(colourValueElement->Attribute("b"));
        tempColourValue.a = 1;
        }
    return this->tempColourValue;
    }

// -----------------------------------------------------------------------------

/*
<!ELEMENT clipping EMPTY>
<!ATTLIST clipping
        near	CDATA #REQUIRED
        far		CDATA #REQUIRED
>*/
void EOSOgreDotSceneLoader::parseClipping(XMLElement* clippingElement, Ogre::Real &nearClip, Ogre::Real &farClip)
    {
    nearClip = 0;
    farClip = 0;
    if (clippingElement != NULL)
        {
        nearClip = Ogre::StringConverter::parseReal(clippingElement->Attribute("near"));
        farClip = Ogre::StringConverter::parseReal(clippingElement->Attribute("far"));
        }
    }

// -----------------------------------------------------------------------------
