
#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT
#endif

#include <stdio.h>
#include <string>

#include "Vector3.h"
#include "Quaternion.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "ELFModuleInfo.h"
#include "ELFLuaRuntime.h"


/**
<module name="ELFModule_3DMath" namespace="ELF3DMath" version="0.8.1.2">
*/

// -----------------------------------------------------------------------------

struct ELFModuleInfo moduleInfo;

// utility functions -----------------------------------------------------

/*bool checkLuaParameters(lua_State* luaRuntime, int parametersRequired, const char* functionName)
{
    int i = 0;
    do{
        i++;
        if(lua_isnil(luaRuntime, i))
            break;
    }
    while(i < parametersRequired);
    //
    if(i != parametersRequired)
    {
        ELFLuaRuntime::getInstance()->logErrorMessage("[ELFSDL2D] Error: bad parameters to '%s'. Required parameters: %d.", functionName, parametersRequired);
        return false;
    }
    return true;
}

void logModuleInfo(ELFModuleInfo& moduleInfo)
{
	char linebuffer[256];
	ELFLuaRuntime::getInstance()->logInfoMessage(" -[ ELF Dynamic Module Information Report ]- - - - - - - - - - - - - - - - - - - - - - - - - - - -");
	ELFLuaRuntime::getInstance()->logInfoMessage("|");
	snprintf(linebuffer, 255, "| %s version %d.%d.%d (release date: %s)", moduleInfo.moduleName, moduleInfo.moduleVerMajor, moduleInfo.moduleVerMinor, moduleInfo.moduleBuild, moduleInfo.moduleReleaseDate);
	ELFLuaRuntime::getInstance()->logInfoMessage(linebuffer);
	ELFLuaRuntime::getInstance()->logInfoMessage("|");
	snprintf(linebuffer, 255, "| Module namespace: %s", moduleInfo.moduleNamespace);
	ELFLuaRuntime::getInstance()->logInfoMessage(linebuffer);
	snprintf(linebuffer, 255, "| Description     : %s", moduleInfo.moduleShortDesc);
	ELFLuaRuntime::getInstance()->logInfoMessage(linebuffer);
	snprintf(linebuffer, 255, "|                 : %s", moduleInfo.moduleDescription1);
	ELFLuaRuntime::getInstance()->logInfoMessage(linebuffer);
	snprintf(linebuffer, 255, "|                 : %s", moduleInfo.moduleDescription2);
	ELFLuaRuntime::getInstance()->logInfoMessage(linebuffer);
	snprintf(linebuffer, 255, "| Vendor          : %s (mailto: %s)", moduleInfo.moduleVendor, moduleInfo.moduleVendorEmail);
	ELFLuaRuntime::getInstance()->logInfoMessage(linebuffer);
	snprintf(linebuffer, 255, "| Vendor homepage : %s", moduleInfo.moduleVendorHomepage);
	ELFLuaRuntime::getInstance()->logInfoMessage(linebuffer);
	ELFLuaRuntime::getInstance()->logInfoMessage(" - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -");
}*/

// end utility functions -------------------------------------------------


// ============================ LIBRARY FUNCTIONS ==============================

// -----------------------------------------------------------------------------

/*int ELFGfxModule::angleRadianToDegree(lua_State* luaRuntime)
	{
	Ogre::Real result = 0;
	if(ELFModule::checkLuaParameters(luaRuntime, 1, "Graphics:angleRadianToDegree", this))
		{
		Ogre::Radian angle = Ogre::Radian((Ogre::Real)luaL_checknumber(luaRuntime, 1));
		result = angle.valueDegrees();
		}
	lua_pushnumber(luaRuntime, result);
	return 1;
	}*/

// -----------------------------------------------------------------------------

/*int ELFGfxModule::angleDegreeToRadian(lua_State* luaRuntime)
	{
	Ogre::Real result = 0;
	if(ELFModule::checkLuaParameters(luaRuntime, 1, "Graphics:angleDegreeToRadian", this))
		{
		Ogre::Degree angle = Ogre::Degree((Ogre::Real)luaL_checknumber(luaRuntime, 1));
		result = angle.valueRadians();
		}
	lua_pushnumber(luaRuntime, result);
	return 1;
	}*/

// -----------------------------------------------------------------------------

/**
	<function name="distance">
		<language code="en">
			<brief text="Returns the distance between two points in 3D space" />
			<description text="" />
			<params>
				<param name="x1" type="Numeric" desc="X component of the first point" />
				<param name="y1" type="Numeric" desc="Y component of the first point" />
				<param name="z1" type="Numeric" desc="Z component of the first point" />
				<param name="x2" type="Numeric" desc="X component of the second point" />
				<param name="y2" type="Numeric" desc="Y component of the second point" />
				<param name="z2" type="Numeric" desc="Z component of the second point" />
			</params>
			<returns>
				<return name="distance" type="Numeric" desc="the distance between the points" />
			</returns>
		</language>
		<language code="it">
			<brief text="Ritorna la distanza tra due punti nello spazio 3D" />
			<description text="" />
			<params>
				<param name="x1" type="Numeric" desc="componente X del primo punto" />
				<param name="y1" type="Numeric" desc="componente Y del primo punto" />
				<param name="z1" type="Numeric" desc="componente Z del primo punto" />
				<param name="x2" type="Numeric" desc="componente X del secondo punto" />
				<param name="y2" type="Numeric" desc="componente Y del secondo punto" />
				<param name="z2" type="Numeric" desc="componente Z del secondo punto" />
			</params>
			<returns>
				<return name="distanza" type="Numeric" desc="la distanza tra i punti" />
			</returns>
		</language>
	</function>
	*/
int distance(lua_State* luaRuntime)
	{
	Vector3 vectorA = Vector3::ZERO;
	Vector3 vectorB = Vector3::ZERO;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 6, "distance"))
		{
		vectorA.x = lua_tonumber(luaRuntime, 1);
		vectorA.y = lua_tonumber(luaRuntime, 2);
		vectorA.z = lua_tonumber(luaRuntime, 3);
		vectorB.x = lua_tonumber(luaRuntime, 4);
		vectorB.y = lua_tonumber(luaRuntime, 5);
		vectorB.z = lua_tonumber(luaRuntime, 6);
		}
	//
	lua_pushnumber(luaRuntime, (vectorB - vectorA).length());
	return 1;
	}

// -----------------------------------------------------------------------------

/**
	<function name="squaredDistance">
		<language code="en">
			<brief text="Returns the squared distance between two points in 3D space" />
			<description text="(Computing squared distance consumes less cpu-time, because avoids computation of square root)" />
			<params>
				<param name="x1" type="Numeric" desc="X component of the first point" />
				<param name="y1" type="Numeric" desc="Y component of the first point" />
				<param name="z1" type="Numeric" desc="Z component of the first point" />
				<param name="x2" type="Numeric" desc="X component of the second point" />
				<param name="y2" type="Numeric" desc="Y component of the second point" />
				<param name="z2" type="Numeric" desc="Z component of the second point" />
			</params>
			<returns>
				<return name="distance" type="Numeric" desc="the squared distance between the points" />
			</returns>
		</language>
		<language code="it">
			<brief text="Ritorna la distanza al quadrato tra due punti nello spazio 3D" />
			<description text="(Calcolare la distanza al quadrato richiede meno tempo macchina, perchè si evita il calcolo della radice quadrata)" />
			<params>
				<param name="x1" type="Numeric" desc="componente X del primo punto" />
				<param name="y1" type="Numeric" desc="componente Y del primo punto" />
				<param name="z1" type="Numeric" desc="componente Z del primo punto" />
				<param name="x2" type="Numeric" desc="componente X del secondo punto" />
				<param name="y2" type="Numeric" desc="componente Y del secondo punto" />
				<param name="z2" type="Numeric" desc="componente Z del secondo punto" />
			</params>
			<returns>
				<return name="distanza" type="Numeric" desc="la distanza al quadrato tra i punti" />
			</returns>
		</language>
	</function>
	*/
int squaredDistance(lua_State* luaRuntime)
	{
	Vector3 vectorA = Vector3::ZERO;
	Vector3 vectorB = Vector3::ZERO;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 6, "squaredDistance"))
		{
		vectorA.x = lua_tonumber(luaRuntime, 1);
		vectorA.y = lua_tonumber(luaRuntime, 2);
		vectorA.z = lua_tonumber(luaRuntime, 3);
		vectorB.x = lua_tonumber(luaRuntime, 4);
		vectorB.y = lua_tonumber(luaRuntime, 5);
		vectorB.z = lua_tonumber(luaRuntime, 6);
		}
	//
	lua_pushnumber(luaRuntime, (vectorB - vectorA).squaredLength());
	return 1;
	}

// -----------------------------------------------------------------------------

/**
	<function name="vectorForScalar">
		<language code="en">
			<brief text="Computes a vector for scalar product, returning the resulting vector" />
			<description text="" />
			<params>
				<param name="x"      type="Numeric" desc="X component of the Vector" />
				<param name="y"      type="Numeric" desc="Y component of the Vector" />
				<param name="z"      type="Numeric" desc="Z component of the Vector" />
				<param name="scalar" type="Numeric" desc="scalar value" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="X component of the resulting Vector" />
				<return name="y" type="Numeric" desc="Y component of the resulting Vector" />
				<return name="z" type="Numeric" desc="Z component of the resulting Vector" />
			</returns>
		</language>
		<language code="it">
			<brief text="Calcola il prodotto di un vettore per uno scalare" />
			<description text="" />
			<params>
				<param name="x"      type="Numeric" desc="componente X del Vettore" />
				<param name="y"      type="Numeric" desc="componente Y del Vettore" />
				<param name="z"      type="Numeric" desc="componente Z del Vettore" />
				<param name="scalar" type="Numeric" desc="valore scalare" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="componente X del Vettore risultante" />
				<return name="y" type="Numeric" desc="componente Y del Vettore risultante" />
				<return name="z" type="Numeric" desc="componente Z del Vettore risultante" />
			</returns>
		</language>
	</function>
	*/
int vectorForScalar(lua_State* luaRuntime)
	{
	Vector3 resultingVector = Vector3::ZERO;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 4, "vectorForScalar"))
		{
		resultingVector.x = lua_tonumber(luaRuntime, 1);
		resultingVector.y = lua_tonumber(luaRuntime, 2);
		resultingVector.z = lua_tonumber(luaRuntime, 3);
		resultingVector *= lua_tonumber(luaRuntime, 4);
		}
	//
	lua_pushnumber(luaRuntime, resultingVector.x);
	lua_pushnumber(luaRuntime, resultingVector.y);
	lua_pushnumber(luaRuntime, resultingVector.z);
	return 3;
	}

// -----------------------------------------------------------------------------

/**
	<function name="vectorDotProduct">
		<language code="en">
			<brief text="computes dot (scalar) product between two vectors: result = firstVector.dotProduct(secondVector)" />
			<description text="" />
			<params>
				<param name="x1" type="Numeric" desc="X component of the first Vector" />
				<param name="y1" type="Numeric" desc="Y component of the first Vector" />
				<param name="z1" type="Numeric" desc="Z component of the first Vector" />
				<param name="x2" type="Numeric" desc="X component of the second Vector" />
				<param name="y2" type="Numeric" desc="Y component of the second Vector" />
				<param name="z2" type="Numeric" desc="Z component of the second Vector" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="X component of the resulting Vector" />
				<return name="y" type="Numeric" desc="Y component of the resulting Vector" />
				<return name="z" type="Numeric" desc="Z component of the resulting Vector" />
			</returns>
		</language>
		<language code="it">
			<brief text="Calcola il dot product tra due vettori: risultato = primoVettore.dotProduct(secondoVettore)" />
			<description text="" />
			<params>
				<param name="x1" type="Numeric" desc="componente X del primo Vettore" />
				<param name="y1" type="Numeric" desc="componente Y del primo Vettore" />
				<param name="z1" type="Numeric" desc="componente Z del primo Vettore" />
				<param name="x2" type="Numeric" desc="componente X del secondo Vettore" />
				<param name="y2" type="Numeric" desc="componente Y del secondo Vettore" />
				<param name="z2" type="Numeric" desc="componente Z del secondo Vettore" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="componente X del Vettore risultante" />
				<return name="y" type="Numeric" desc="componente Y del Vettore risultante" />
				<return name="z" type="Numeric" desc="componente Z del Vettore risultante" />
			</returns>
		</language>
	</function>
	*/
int vectorDotProduct(lua_State* luaRuntime)
	{
	Vector3 firstVector = Vector3::ZERO;
	Vector3 secondVector = Vector3::ZERO;
	double result = 0.0;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 6, "vectorDotProduct"))
		{
		firstVector.x  = lua_tonumber(luaRuntime, 1);
		firstVector.y  = lua_tonumber(luaRuntime, 2);
		firstVector.z  = lua_tonumber(luaRuntime, 3);
		firstVector.normalise();
		secondVector.x = lua_tonumber(luaRuntime, 4);
		secondVector.y = lua_tonumber(luaRuntime, 5);
		secondVector.z = lua_tonumber(luaRuntime, 6);
		secondVector.normalise();
		result = firstVector.dotProduct(secondVector);
		}
	//
	lua_pushnumber(luaRuntime, result);
	return 1;
	}

// -----------------------------------------------------------------------------

/**
	<function name="vectorCrossProduct">
		<language code="en">
			<brief text="computes cross product between two vectors: result = firstVector.crossProduct(secondVector)" />
			<description text="" />
			<params>
				<param name="x1" type="Numeric" desc="X component of the first Vector" />
				<param name="y1" type="Numeric" desc="Y component of the first Vector" />
				<param name="z1" type="Numeric" desc="Z component of the first Vector" />
				<param name="x2" type="Numeric" desc="X component of the second Vector" />
				<param name="y2" type="Numeric" desc="Y component of the second Vector" />
				<param name="z2" type="Numeric" desc="Z component of the second Vector" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="X component of the resulting Vector" />
				<return name="y" type="Numeric" desc="Y component of the resulting Vector" />
				<return name="z" type="Numeric" desc="Z component of the resulting Vector" />
			</returns>
		</language>
		<language code="it">
			<brief text="Calcola il cross product tra due vettori: risultato = primoVettore.crossProduct(secondoVettore)" />
			<description text="" />
			<params>
				<param name="x1" type="Numeric" desc="componente X del primo Vettore" />
				<param name="y1" type="Numeric" desc="componente Y del primo Vettore" />
				<param name="z1" type="Numeric" desc="componente Z del primo Vettore" />
				<param name="x2" type="Numeric" desc="componente X del secondo Vettore" />
				<param name="y2" type="Numeric" desc="componente Y del secondo Vettore" />
				<param name="z2" type="Numeric" desc="componente Z del secondo Vettore" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="componente X del Vettore risultante" />
				<return name="y" type="Numeric" desc="componente Y del Vettore risultante" />
				<return name="z" type="Numeric" desc="componente Z del Vettore risultante" />
			</returns>
		</language>
	</function>
	*/
int vectorCrossProduct(lua_State* luaRuntime)
	{
	Vector3 firstVector = Vector3::ZERO;
	Vector3 secondVector = Vector3::ZERO;
	Vector3 result = Vector3::ZERO;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 6, "vectorDotProduct"))
		{
		firstVector.x  = lua_tonumber(luaRuntime, 1);
		firstVector.y  = lua_tonumber(luaRuntime, 2);
		firstVector.z  = lua_tonumber(luaRuntime, 3);
		secondVector.x = lua_tonumber(luaRuntime, 4);
		secondVector.y = lua_tonumber(luaRuntime, 5);
		secondVector.z = lua_tonumber(luaRuntime, 6);
		result = firstVector.crossProduct(secondVector);
		}
	//
	lua_pushnumber(luaRuntime, result.x);
	lua_pushnumber(luaRuntime, result.y);
	lua_pushnumber(luaRuntime, result.z);
	return 3;
	}

// -----------------------------------------------------------------------------

/**
	<function name="vectorDifference">
		<language code="en">
			<brief text="Subtract a vector from another vector: resultVector = firstVector - secondVector" />
			<description text="" />
			<params>
				<param name="x1" type="Numeric" desc="X component of the first Vector" />
				<param name="y1" type="Numeric" desc="Y component of the first Vector" />
				<param name="z1" type="Numeric" desc="Z component of the first Vector" />
				<param name="x2" type="Numeric" desc="X component of the second Vector" />
				<param name="y2" type="Numeric" desc="Y component of the second Vector" />
				<param name="z2" type="Numeric" desc="Z component of the second Vector" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="X component of the resulting Vector" />
				<return name="y" type="Numeric" desc="Y component of the resulting Vector" />
				<return name="z" type="Numeric" desc="Z component of the resulting Vector" />
			</returns>
		</language>
		<language code="it">
			<brief text="Sottrae un vettore da un altro: resultVector = firstVector - secondVector" />
			<description text="" />
			<params>
				<param name="x1" type="Numeric" desc="componente X del primo Vettore" />
				<param name="y1" type="Numeric" desc="componente Y del primo Vettore" />
				<param name="z1" type="Numeric" desc="componente Z del primo Vettore" />
				<param name="x2" type="Numeric" desc="componente X del secondo Vettore" />
				<param name="y2" type="Numeric" desc="componente Y del secondo Vettore" />
				<param name="z2" type="Numeric" desc="componente Z del secondo Vettore" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="componente X del Vettore risultante" />
				<return name="y" type="Numeric" desc="componente Y del Vettore risultante" />
				<return name="z" type="Numeric" desc="componente Z del Vettore risultante" />
			</returns>
		</language>
	</function>
	*/
int vectorDifference(lua_State* luaRuntime)
	{
	Vector3 firstVector  = Vector3::ZERO;
	Vector3 secondVector = Vector3::ZERO;
	Vector3 resultVector = Vector3::ZERO;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 6, "vectorDifference"))
		{
		firstVector.x  = lua_tonumber(luaRuntime, 1);
		firstVector.y  = lua_tonumber(luaRuntime, 2);
		firstVector.z  = lua_tonumber(luaRuntime, 3);
		secondVector.x = lua_tonumber(luaRuntime, 4);
		secondVector.y = lua_tonumber(luaRuntime, 5);
		secondVector.z = lua_tonumber(luaRuntime, 6);
		resultVector = firstVector - secondVector;
		}
	//
	lua_pushnumber(luaRuntime, resultVector.x);
	lua_pushnumber(luaRuntime, resultVector.y);
	lua_pushnumber(luaRuntime, resultVector.z);
	return 3;
	}

// -----------------------------------------------------------------------------

/**
	<function name="vectorGetRotationTo">
		<language code="en">
			<brief text="Returns the shortest arc quaternion to rotate vector1 to the vector2" />
			<description text="" />
			<params>
				<param name="x1" type="Numeric" desc="X component of the first Vector" />
				<param name="y1" type="Numeric" desc="Y component of the first Vector" />
				<param name="z1" type="Numeric" desc="Z component of the first Vector" />
				<param name="x2" type="Numeric" desc="X component of the second Vector" />
				<param name="y2" type="Numeric" desc="Y component of the second Vector" />
				<param name="z2" type="Numeric" desc="Z component of the second Vector" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="X component of the resulting Quaternion" />
				<return name="y" type="Numeric" desc="Y component of the resulting Quaternion" />
				<return name="z" type="Numeric" desc="Z component of the resulting Quaternion" />
				<return name="w" type="Numeric" desc="W component of the resulting Quaternion" />
			</returns>
		</language>
		<language code="it">
			<brief text="Ritorna il quaternione che rappresenta l'arco più breve per ruotare il vettore1 al vettore2" />
			<description text="" />
			<params>
				<param name="x1" type="Numeric" desc="componente X del primo Vettore" />
				<param name="y1" type="Numeric" desc="componente Y del primo Vettore" />
				<param name="z1" type="Numeric" desc="componente Z del primo Vettore" />
				<param name="x2" type="Numeric" desc="componente X del secondo Vettore" />
				<param name="y2" type="Numeric" desc="componente Y del secondo Vettore" />
				<param name="z2" type="Numeric" desc="componente Z del secondo Vettore" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="componente X del Quaternione risultante" />
				<return name="y" type="Numeric" desc="componente Y del Quaternione risultante" />
				<return name="z" type="Numeric" desc="componente Z del Quaternione risultante" />
				<return name="w" type="Numeric" desc="componente W del Quaternione risultante" />
			</returns>
		</language>
	</function>
	*/
int vectorGetRotationTo(lua_State* luaRuntime)
	{
	Vector3 firstVector  = Vector3::ZERO;
	Vector3 secondVector = Vector3::ZERO;
	Quaternion rotation  = Quaternion::IDENTITY;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 6, "vectorGetRotationTo"))
		{
		firstVector.x  = lua_tonumber(luaRuntime, 1);
		firstVector.y  = lua_tonumber(luaRuntime, 2);
		firstVector.z  = lua_tonumber(luaRuntime, 3);
		//
		secondVector.x = lua_tonumber(luaRuntime, 4);
		secondVector.y = lua_tonumber(luaRuntime, 5);
		secondVector.z = lua_tonumber(luaRuntime, 6);
		//
		rotation = firstVector.getRotationTo(secondVector);
		}
	//
	lua_pushnumber(luaRuntime, rotation.x);
	lua_pushnumber(luaRuntime, rotation.y);
	lua_pushnumber(luaRuntime, rotation.z);
	lua_pushnumber(luaRuntime, rotation.w);
	return 4;
	}

// -----------------------------------------------------------------------------

/**
	<function name="vectorNormalize">
		<language code="en">
			<brief text="Normalize the vector" />
			<description text="" />
			<params>
				<param name="x" type="Numeric" desc="X component of the Vector" />
				<param name="y" type="Numeric" desc="Y component of the Vector" />
				<param name="z" type="Numeric" desc="Z component of the Vector" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="X component of the normalized Vector" />
				<return name="y" type="Numeric" desc="Y component of the normalized Vector" />
				<return name="z" type="Numeric" desc="Z component of the normalized Vector" />
			</returns>
		</language>
		<language code="it">
			<brief text="Normalizza il vettore" />
			<description text="" />
			<params>
				<param name="x" type="Numeric" desc="componente X del Vettore" />
				<param name="y" type="Numeric" desc="componente Y del Vettore" />
				<param name="z" type="Numeric" desc="componente Z del Vettore" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="componente X del Vettore normalizzato" />
				<return name="y" type="Numeric" desc="componente Y del Vettore normalizzato" />
				<return name="z" type="Numeric" desc="componente Z del Vettore normalizzato" />
			</returns>
		</language>
	</function>
	*/
int vectorNormalize(lua_State* luaRuntime)
	{
	Vector3 vector = Vector3::ZERO;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 3, "vectorNormalize"))
		{
		vector.x  = lua_tonumber(luaRuntime, 1);
		vector.y  = lua_tonumber(luaRuntime, 2);
		vector.z  = lua_tonumber(luaRuntime, 3);
		//
		vector.normalise();
		}
	//
	lua_pushnumber(luaRuntime, vector.x);
	lua_pushnumber(luaRuntime, vector.y);
	lua_pushnumber(luaRuntime, vector.z);
	return 3;
	}

// -----------------------------------------------------------------------------

/**
	<function name="quaternionForVector">
		<language code="en">
			<brief text="Returns the product between a Quaternion and a Vector" />
			<description text="" />
			<params>
				<param name="qx" type="Numeric" desc="X component of the Quaternion" />
				<param name="qy" type="Numeric" desc="Y component of the Quaternion" />
				<param name="qz" type="Numeric" desc="Z component of the Quaternion" />
				<param name="qw" type="Numeric" desc="W component of the Quaternion" />
				<param name="vx" type="Numeric" desc="X component of the Vector" />
				<param name="vy" type="Numeric" desc="Y component of the Vector" />
				<param name="vz" type="Numeric" desc="Z component of the Vector" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="X component of the resulting Vector" />
				<return name="y" type="Numeric" desc="Y component of the resulting Vector" />
				<return name="z" type="Numeric" desc="Z component of the resulting Vector" />
			</returns>
		</language>
		<language code="it">
			<brief text="Ritorna il prodotto tra un Quaternione ed un Vettore" />
			<description text="" />
			<params>
				<param name="qx" type="Numeric" desc="componente X del Quaternione" />
				<param name="qy" type="Numeric" desc="componente Y del Quaternione" />
				<param name="qz" type="Numeric" desc="componente Z del Quaternione" />
				<param name="qw" type="Numeric" desc="componente W del Quaternione" />
				<param name="vx" type="Numeric" desc="componente X del Vettore" />
				<param name="vy" type="Numeric" desc="componente Y del Vettore" />
				<param name="vz" type="Numeric" desc="componente Z del Vettore" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="componente X del Vettore risultante" />
				<return name="y" type="Numeric" desc="componente Y del Vettore risultante" />
				<return name="z" type="Numeric" desc="componente Z del Vettore risultante" />
			</returns>
		</language>
	</function>
	*/
int quaternionForVector(lua_State* luaRuntime)
	{
	Quaternion quaternion = Quaternion::IDENTITY;
	Vector3 vector = Vector3::ZERO;
	Vector3 result = Vector3::ZERO;

	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 7, "quaternionForVector"))
		{
		quaternion.x = lua_tonumber(luaRuntime, 1);
		quaternion.y = lua_tonumber(luaRuntime, 2);
		quaternion.z = lua_tonumber(luaRuntime, 3);
		quaternion.w = lua_tonumber(luaRuntime, 4);
		vector.x = lua_tonumber(luaRuntime, 5);
		vector.y = lua_tonumber(luaRuntime, 6);
		vector.z = lua_tonumber(luaRuntime, 7);
		result = quaternion * vector;
		}
	//
	lua_pushnumber(luaRuntime, result.x);
	lua_pushnumber(luaRuntime, result.y);
	lua_pushnumber(luaRuntime, result.z);
	return 3;
	}

// -----------------------------------------------------------------------------

/**
	@param qx        (numeric) - quaternion x component
	@param qy        (numeric) - quaternion y component
	@param qz        (numeric) - quaternion z component
	@param qw        (numeric) - quaternion w component
	@return roll equivalent angle (in radians)
	*/

/**
	<function name="quaternionGetPitch">
		<language code="en">
			<brief text="Returns the pitch equivalent angle (in radians)" />
			<description text="" />
			<params>
				<param name="x" type="Numeric" desc="X component of the Quaternion" />
				<param name="y" type="Numeric" desc="Y component of the Quaternion" />
				<param name="z" type="Numeric" desc="Z component of the Quaternion" />
				<param name="w" type="Numeric" desc="W component of the Quaternion" />
			</params>
			<returns>
				<return name="pitch" type="Numeric" desc="Pitch equivalent angle (in radians)" />
			</returns>
		</language>
		<language code="it">
			<brief text="Ritorna l'angolo equivalente al pitch (in radianti)" />
			<description text="" />
			<params>
				<param name="x" type="Numeric" desc="componente X del Quaternione" />
				<param name="y" type="Numeric" desc="componente Y del Quaternione" />
				<param name="z" type="Numeric" desc="componente Z del Quaternione" />
				<param name="w" type="Numeric" desc="componente W del Quaternione" />
			</params>
			<returns>
				<return name="pitch" type="Numeric" desc="Angolo equivalente al pitch (in radianti)" />
			</returns>
		</language>
	</function>
	*/
int quaternionGetPitch(lua_State* luaRuntime)
	{
	Quaternion quaternion = Quaternion::ZERO;
	double angle = 0.0;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 4, "quaternionGetPitch"))
		{
		quaternion.x = lua_tonumber(luaRuntime, 1);
		quaternion.y = lua_tonumber(luaRuntime, 2);
		quaternion.z = lua_tonumber(luaRuntime, 3);
		quaternion.w = lua_tonumber(luaRuntime, 4);
		angle = (Real)quaternion.getPitch().valueRadians();
		}
	//
	lua_pushnumber(luaRuntime, angle);
	return 1;
	}

// -----------------------------------------------------------------------------

/**
	<function name="quaternionGetYaw">
		<language code="en">
			<brief text="Returns the yaw equivalent angle (in radians)" />
			<description text="" />
			<params>
				<param name="x" type="Numeric" desc="X component of the Quaternion" />
				<param name="y" type="Numeric" desc="Y component of the Quaternion" />
				<param name="z" type="Numeric" desc="Z component of the Quaternion" />
				<param name="w" type="Numeric" desc="W component of the Quaternion" />
			</params>
			<returns>
				<return name="yaw" type="Numeric" desc="Yaw equivalent angle (in radians)" />
			</returns>
		</language>
		<language code="it">
			<brief text="Ritorna l'angolo equivalente allo yaw (in radianti)" />
			<description text="" />
			<params>
				<param name="x" type="Numeric" desc="componente X del Quaternione" />
				<param name="y" type="Numeric" desc="componente Y del Quaternione" />
				<param name="z" type="Numeric" desc="componente Z del Quaternione" />
				<param name="w" type="Numeric" desc="componente W del Quaternione" />
			</params>
			<returns>
				<return name="yaw" type="Numeric" desc="Angolo equivalente allo yaw (in radianti)" />
			</returns>
		</language>
	</function>
	*/
int quaternionGetYaw(lua_State* luaRuntime)
	{
	Quaternion quaternion = Quaternion::ZERO;
	double angle = 0.0;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 4, "quaternionGetYaw"))
		{
		quaternion.x = lua_tonumber(luaRuntime, 1);
		quaternion.y = lua_tonumber(luaRuntime, 2);
		quaternion.z = lua_tonumber(luaRuntime, 3);
		quaternion.w = lua_tonumber(luaRuntime, 4);
		angle = (Real)quaternion.getYaw().valueRadians();
		}
	//
	lua_pushnumber(luaRuntime, angle);
	return 1;
	}

// -----------------------------------------------------------------------------

/**
	<function name="quaternionGetRoll">
		<language code="en">
			<brief text="Returns the roll equivalent angle (in radians)" />
			<description text="" />
			<params>
				<param name="x" type="Numeric" desc="X component of the Quaternion" />
				<param name="y" type="Numeric" desc="Y component of the Quaternion" />
				<param name="z" type="Numeric" desc="Z component of the Quaternion" />
				<param name="w" type="Numeric" desc="W component of the Quaternion" />
			</params>
			<returns>
				<return name="roll" type="Numeric" desc="Roll equivalent angle (in radians)" />
			</returns>
		</language>
		<language code="it">
			<brief text="Ritorna l'angolo equivalente al roll (in radianti)" />
			<description text="" />
			<params>
				<param name="x" type="Numeric" desc="componente X del Quaternione" />
				<param name="y" type="Numeric" desc="componente Y del Quaternione" />
				<param name="z" type="Numeric" desc="componente Z del Quaternione" />
				<param name="w" type="Numeric" desc="componente W del Quaternione" />
			</params>
			<returns>
				<return name="roll" type="Numeric" desc="Angolo equivalente al roll (in radianti)" />
			</returns>
		</language>
	</function>
	*/
int quaternionGetRoll(lua_State* luaRuntime)
	{
	Quaternion quaternion = Quaternion::ZERO;
	double angle = 0.0;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 4, "quaternionGetRoll"))
		{
		quaternion.x = lua_tonumber(luaRuntime, 1);
		quaternion.y = lua_tonumber(luaRuntime, 2);
		quaternion.z = lua_tonumber(luaRuntime, 3);
		quaternion.w = lua_tonumber(luaRuntime, 4);
		angle = (Real)quaternion.getRoll().valueRadians();
		}
	//
	lua_pushnumber(luaRuntime, angle);
	return 1;
	}

// -----------------------------------------------------------------------------

/**
	<function name="quaternionGetAxis">
		<language code="en">
			<brief text="Returns an axis from the quaternion" />
			<description text="" />
			<params>
				<param name="x" type="Numeric" desc="X component of the Quaternion" />
				<param name="y" type="Numeric" desc="Y component of the Quaternion" />
				<param name="z" type="Numeric" desc="Z component of the Quaternion" />
				<param name="w" type="Numeric" desc="W component of the Quaternion" />
				<param name="axisIndex" type="Numeric" desc="1 for X (side), 2 for Y (up), 3 for Z (front)" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="X component of the selected axis" />
				<return name="y" type="Numeric" desc="Y component of the selected axis" />
				<return name="z" type="Numeric" desc="Z component of the selected axis" />
			</returns>
		</language>
		<language code="it">
			<brief text="Ritorna un asse dal quaternione" />
			<description text="" />
			<params>
				<param name="x" type="Numeric" desc="componente X del Quaternione" />
				<param name="y" type="Numeric" desc="componente Y del Quaternione" />
				<param name="z" type="Numeric" desc="componente Z del Quaternione" />
				<param name="w" type="Numeric" desc="componente W del Quaternione" />
				<param name="axisIndex" type="Numeric" desc="1 per X (side), 2 per Y (up), 3 per Z (front)" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="componente X dell'asse selezionato" />
				<return name="y" type="Numeric" desc="componente Y dell'asse selezionato" />
				<return name="z" type="Numeric" desc="componente Z dell'asse selezionato" />
			</returns>
		</language>
	</function>
	*/
int quaternionGetAxis(lua_State* luaRuntime)
	{
	Quaternion quaternion = Quaternion::IDENTITY;
	Vector3 result = Vector3::ZERO;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 5, "quaternionGetAxis"))
		{
		quaternion.x = lua_tonumber(luaRuntime, 1);
		quaternion.y = lua_tonumber(luaRuntime, 2);
		quaternion.z = lua_tonumber(luaRuntime, 3);
		quaternion.w = lua_tonumber(luaRuntime, 4);
		int axisIndex = (int)lua_tonumber(luaRuntime, 5);
		//
		Vector3 axes[3];
		quaternion.ToAxes(axes);
		if(axisIndex > 0 && axisIndex < 4)
			result = axes[axisIndex-1];
		else
			LOGERROR("[quaternionGetAxis] - Error: Bad axis index (legal values are 1, 2 and 3).");
		}
	//
	lua_pushnumber(luaRuntime, result.x);
	lua_pushnumber(luaRuntime, result.y);
	lua_pushnumber(luaRuntime, result.z);
	return 3;
	}

// -----------------------------------------------------------------------------

/*int ELFGfxModule::computeDistance(lua_State* luaRuntime)
	{
	Ogre::Real distance = -1;
	//Ogre::Vector3 resultingVector = Ogre::Vector3::ZERO;
	if(checkLuaParameters(luaRuntime, 6, "Graphics:computeDistance"))
		{
		Ogre::Vector3 pointOne = Ogre::Vector3::ZERO;
		Ogre::Vector3 pointTwo = Ogre::Vector3::ZERO;
		pointOne.x = lua_tonumber(luaRuntime, 1);
		pointOne.y = lua_tonumber(luaRuntime, 2);
		pointOne.z = lua_tonumber(luaRuntime, 3);
		pointTwo.x = lua_tonumber(luaRuntime, 4);
		pointTwo.y = lua_tonumber(luaRuntime, 5);
		pointTwo.z = lua_tonumber(luaRuntime, 6);

		pointOne.x =
		mDirection = mDestination - mNode->getPosition( );
		mDistance = mDirection.normalise( );


		resultingVector.x = lua_tonumber(luaRuntime, 1);
		resultingVector.y = lua_tonumber(luaRuntime, 2);
		resultingVector.z = lua_tonumber(luaRuntime, 3);
		resultingVector *= lua_tonumber(luaRuntime, 4);
		}
	//
	lua_pushnumber(luaRuntime, resultingVector.x);
	lua_pushnumber(luaRuntime, resultingVector.y);
	lua_pushnumber(luaRuntime, resultingVector.z);
	return 3;
	}*/

// -----------------------------------------------------------------------------

/**
	<function name="quaternionSlerp">
		<language code="en">
			<brief text="Returns the result of a linear interpolation between two quaternions" />
			<description text="Slerp and nlerp are both linear interpolations. Slerp is more accurate, but slower." />
			<params>
                <param name="interpolation" type="Numeric" desc="interpolation amount (0.0 to 1.0)" />
				<param name="sx" type="Numeric" desc="X component of the source Quaternion" />
				<param name="sy" type="Numeric" desc="Y component of the source Quaternion" />
				<param name="sz" type="Numeric" desc="Z component of the source Quaternion" />
				<param name="sw" type="Numeric" desc="W component of the source Quaternion" />
				<param name="dx" type="Numeric" desc="X component of the destination Quaternion" />
				<param name="dy" type="Numeric" desc="Y component of the destination Quaternion" />
				<param name="dz" type="Numeric" desc="Z component of the destination Quaternion" />
				<param name="dw" type="Numeric" desc="W component of the destination Quaternion" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="X component of the resulting quaternion" />
				<return name="y" type="Numeric" desc="Y component of the resulting quaternion" />
				<return name="z" type="Numeric" desc="Z component of the resulting quaternion" />
				<return name="w" type="Numeric" desc="W component of the resulting quaternion" />
			</returns>
		</language>
		<language code="it">
			<brief text="Ritorna il risultato dell'interpolazione lineare tra due quaternioni" />
			<description text="Slerp e nlerp sono entrambe interpolazioni lineari. Slerp è più precisa, ma più lenta." />
			<params>
				<param name="interpolation" type="Numeric" desc="quantità di interpolazione (0.0 a 1.0)" />
				<param name="sx" type="Numeric" desc="componente X del Quaternione sorgente" />
				<param name="sy" type="Numeric" desc="componente Y del Quaternione sorgente" />
				<param name="sz" type="Numeric" desc="componente Z del Quaternione sorgente" />
				<param name="sw" type="Numeric" desc="componente W del Quaternione sorgente" />
                <param name="dx" type="Numeric" desc="componente X del Quaternione destinazione" />
				<param name="dy" type="Numeric" desc="componente Y del Quaternione destinazione" />
				<param name="dz" type="Numeric" desc="componente Z del Quaternione destinazione" />
				<param name="dw" type="Numeric" desc="componente W del Quaternione destinazione" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="componente X del quaternione risultante" />
				<return name="y" type="Numeric" desc="componente Y del quaternione risultante" />
				<return name="z" type="Numeric" desc="componente Z del quaternione risultante" />
				<return name="w" type="Numeric" desc="componente W del quaternione risultante" />
			</returns>
		</language>
	</function>
	*/
int quaternionSlerp(lua_State* luaRuntime)
	{
	Quaternion srcQuaternion  = Quaternion::IDENTITY;
	Quaternion destQuaternion = Quaternion::IDENTITY;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 9, "quaternionSlerp"))
		{
		double interpolation = lua_tonumber(luaRuntime, 1);
		srcQuaternion.x = lua_tonumber(luaRuntime, 2);
		srcQuaternion.y = lua_tonumber(luaRuntime, 3);
		srcQuaternion.z = lua_tonumber(luaRuntime, 4);
		srcQuaternion.w = lua_tonumber(luaRuntime, 5);
		destQuaternion.x = lua_tonumber(luaRuntime, 6);
		destQuaternion.y = lua_tonumber(luaRuntime, 7);
		destQuaternion.z = lua_tonumber(luaRuntime, 8);
		destQuaternion.w = lua_tonumber(luaRuntime, 9);
		//
		srcQuaternion = Quaternion::Slerp(interpolation, srcQuaternion, destQuaternion);
		}
	//
	lua_pushnumber(luaRuntime, srcQuaternion.x);
	lua_pushnumber(luaRuntime, srcQuaternion.y);
	lua_pushnumber(luaRuntime, srcQuaternion.z);
	lua_pushnumber(luaRuntime, srcQuaternion.w);
	return 4;
	}

// -----------------------------------------------------------------------------

/**
	<function name="quaternionNlerp">
		<language code="en">
			<brief text="Returns the result of a linear interpolation between two quaternions" />
			<description text="Slerp and nlerp are both linear interpolations. Nlerp is more faster, but less accurate." />
			<params>
                <param name="interpolation" type="Numeric" desc="interpolation amount (0.0 to 1.0)" />
				<param name="sx" type="Numeric" desc="X component of the source Quaternion" />
				<param name="sy" type="Numeric" desc="Y component of the source Quaternion" />
				<param name="sz" type="Numeric" desc="Z component of the source Quaternion" />
				<param name="sw" type="Numeric" desc="W component of the source Quaternion" />
				<param name="dx" type="Numeric" desc="X component of the destination Quaternion" />
				<param name="dy" type="Numeric" desc="Y component of the destination Quaternion" />
				<param name="dz" type="Numeric" desc="Z component of the destination Quaternion" />
				<param name="dw" type="Numeric" desc="W component of the destination Quaternion" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="X component of the resulting quaternion" />
				<return name="y" type="Numeric" desc="Y component of the resulting quaternion" />
				<return name="z" type="Numeric" desc="Z component of the resulting quaternion" />
				<return name="w" type="Numeric" desc="W component of the resulting quaternion" />
			</returns>
		</language>
		<language code="it">
			<brief text="Ritorna il risultato dell'interpolazione lineare tra due quaternioni" />
			<description text="Slerp e nlerp sono entrambe interpolazioni lineari. Nlerp è più veloce, ma meno precisa." />
			<params>
				<param name="interpolation" type="Numeric" desc="quantità di interpolazione (0.0 a 1.0)" />
				<param name="sx" type="Numeric" desc="componente X del Quaternione sorgente" />
				<param name="sy" type="Numeric" desc="componente Y del Quaternione sorgente" />
				<param name="sz" type="Numeric" desc="componente Z del Quaternione sorgente" />
				<param name="sw" type="Numeric" desc="componente W del Quaternione sorgente" />
                <param name="dx" type="Numeric" desc="componente X del Quaternione destinazione" />
				<param name="dy" type="Numeric" desc="componente Y del Quaternione destinazione" />
				<param name="dz" type="Numeric" desc="componente Z del Quaternione destinazione" />
				<param name="dw" type="Numeric" desc="componente W del Quaternione destinazione" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="componente X del quaternione risultante" />
				<return name="y" type="Numeric" desc="componente Y del quaternione risultante" />
				<return name="z" type="Numeric" desc="componente Z del quaternione risultante" />
				<return name="w" type="Numeric" desc="componente W del quaternione risultante" />
			</returns>
		</language>
	</function>
	*/
int quaternionNlerp(lua_State* luaRuntime)
	{
	Quaternion srcQuaternion  = Quaternion::IDENTITY;
	Quaternion destQuaternion = Quaternion::IDENTITY;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 9, "quaternionNlerp"))
		{
		double interpolation = lua_tonumber(luaRuntime, 1);
		srcQuaternion.x = lua_tonumber(luaRuntime, 2);
		srcQuaternion.y = lua_tonumber(luaRuntime, 3);
		srcQuaternion.z = lua_tonumber(luaRuntime, 4);
		srcQuaternion.w = lua_tonumber(luaRuntime, 5);
		destQuaternion.x = lua_tonumber(luaRuntime, 6);
		destQuaternion.y = lua_tonumber(luaRuntime, 7);
		destQuaternion.z = lua_tonumber(luaRuntime, 8);
		destQuaternion.w = lua_tonumber(luaRuntime, 9);
		//
		srcQuaternion = Quaternion::nlerp(interpolation, srcQuaternion, destQuaternion);
		}
	//
	lua_pushnumber(luaRuntime, srcQuaternion.x);
	lua_pushnumber(luaRuntime, srcQuaternion.y);
	lua_pushnumber(luaRuntime, srcQuaternion.z);
	lua_pushnumber(luaRuntime, srcQuaternion.w);
	return 4;
	}

// -----------------------------------------------------------------------------

/**
	@param RightX  (Numeric) - x component of the Right (X) vector
	@param RightY  (Numeric) - y component of the Right (X) vector
	@param RightZ  (Numeric) - z component of the Right (X) vector
	@param UpX     (Numeric) - x component of the Up (Y) vector
	@param UpY     (Numeric) - y component of the Up (Y) vector
	@param UpZ     (Numeric) - z component of the Up (Y) vector
	@param FrontX  (Numeric) - x component of the Front (Z) vector
	@param FrontY  (Numeric) - y component of the Front (Z) vector
	@param FrontZ  (Numeric) - z component of the Front (Z) vector
	@return x, y, z, w of the quaternion constructed from given vectors
	*/

/**
	<function name="quaternionFromAxes">
		<language code="en">
			<brief text="Returns a quaternion from three axes" />
			<description text="" />
			<params>
				<param name="rx" type="Numeric" desc="X component of the right (X) vector" />
				<param name="ry" type="Numeric" desc="Y component of the right (X) vector" />
				<param name="rz" type="Numeric" desc="Z component of the right (X) vector" />
				<param name="ux" type="Numeric" desc="X component of the up (Y) vector" />
				<param name="uy" type="Numeric" desc="Y component of the up (Y) vector" />
				<param name="uz" type="Numeric" desc="Z component of the up (Y) vector" />
				<param name="fx" type="Numeric" desc="X component of the front (Z) vector" />
				<param name="fy" type="Numeric" desc="Y component of the front (Z) vector" />
				<param name="fz" type="Numeric" desc="Z component of the front (Z) vector" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="X component of the resulting quaternion" />
				<return name="y" type="Numeric" desc="Y component of the resulting quaternion" />
				<return name="z" type="Numeric" desc="Z component of the resulting quaternion" />
				<return name="w" type="Numeric" desc="W component of the resulting quaternion" />
			</returns>
		</language>
		<language code="it">
			<brief text="Ritorna un quaternione da tre assi" />
			<description text="" />
			<params>
				<param name="rx" type="Numeric" desc="componente X del vettore right (X)" />
				<param name="ry" type="Numeric" desc="componente Y del vettore right (X)" />
				<param name="rz" type="Numeric" desc="componente X del vettore right (X)" />
				<param name="ux" type="Numeric" desc="componente X del vettore up (Y)" />
				<param name="uy" type="Numeric" desc="componente Y del vettore up (Y)" />
				<param name="uz" type="Numeric" desc="componente Z del vettore up (Y)" />
				<param name="fx" type="Numeric" desc="componente X del vettore front (Z)" />
				<param name="fy" type="Numeric" desc="componente Y del vettore front (Z)" />
				<param name="fz" type="Numeric" desc="componente Z del vettore front (Z)" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="componente X del quaternione risultante" />
				<return name="y" type="Numeric" desc="componente Y del quaternione risultante" />
				<return name="z" type="Numeric" desc="componente Z del quaternione risultante" />
				<return name="w" type="Numeric" desc="componente W del quaternione risultante" />
			</returns>
		</language>
	</function>
	*/
int quaternionFromAxes(lua_State* luaRuntime)
	{
	Quaternion quaternion = Quaternion::IDENTITY;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 9, "quaternionFromAxes"))
		{
		Vector3 right = Vector3(lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3));
		Vector3 up    = Vector3(lua_tonumber(luaRuntime, 4), lua_tonumber(luaRuntime, 5), lua_tonumber(luaRuntime, 6));
		Vector3 front = Vector3(lua_tonumber(luaRuntime, 7), lua_tonumber(luaRuntime, 8), lua_tonumber(luaRuntime, 9));
		quaternion.FromAxes(right, up, front);
		}
	//
	lua_pushnumber(luaRuntime, quaternion.x);
	lua_pushnumber(luaRuntime, quaternion.y);
	lua_pushnumber(luaRuntime, quaternion.z);
	lua_pushnumber(luaRuntime, quaternion.w);
	return 4;
	}

// -----------------------------------------------------------------------------

/**
	<function name="pointToScreenProjection">
		<language code="en">
			<brief text="Projects a 3D point on the two-dimensional screen plane" />
			<description text="" />
			<params>
				<param name="x" type="Numeric" desc="X component of point to project" />
				<param name="y" type="Numeric" desc="Y component of point to project" />
				<param name="z" type="Numeric" desc="Z component of point to project" />
				<param name="f" type="Numeric" desc="projection distance" />
				<param name="screenWidth" type="Numeric" desc="pixel width of the screen" />
				<param name="screenHeight" type="Numeric" desc="pixel height of the screen" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="X component of the point projected on the two-dimensional plane" />
				<return name="y" type="Numeric" desc="Y component of the point projected on the two-dimensional plane" />
			</returns>
		</language>
		<language code="it">
			<brief text="Proietta un punto 3D sul piano bidimensionale dello schermo" />
			<description text="" />
			<params>
				<param name="x" type="Numeric" desc="componente X del punto da proiettare" />
				<param name="y" type="Numeric" desc="componente Y del punto da proiettare" />
				<param name="z" type="Numeric" desc="componente Z del punto da proiettare" />
				<param name="f" type="Numeric" desc="distanza della proiezione" />
				<param name="screenWidth" type="Numeric" desc="larghezza dello schermo (in pixel)" />
				<param name="screenHeight" type="Numeric" desc="altezza dello schermo (in pixel)" />
			</params>
			<returns>
				<return name="x" type="Numeric" desc="componente X del punto proiettato sul piano bidimensionale" />
				<return name="y" type="Numeric" desc="componente Y del punto proiettato sul piano bidimensionale" />
			</returns>
		</language>
	</function>
	*/
int pointToScreenProjection(lua_State* luaRuntime)
	{
	Vector3 point = Vector3::ZERO;
	float x_2d = 0.0;
	float y_2d = 0.0;
	if(ELFLuaRuntime::checkLuaParameters(&moduleInfo, 6, "pointToScreenProjection"))
		{
		point = Vector3(lua_tonumber(luaRuntime, 1), lua_tonumber(luaRuntime, 2), lua_tonumber(luaRuntime, 3));
		//
		float F  = lua_tonumber(luaRuntime, 4);
		float xc = lua_tonumber(luaRuntime, 5) / 2;
		float yc = lua_tonumber(luaRuntime, 6) / 2;
		//
		x_2d = (point.x / point.z) * F + xc;
		y_2d = (point.y / point.z) * F + yc;
		}
	//
	lua_pushnumber(luaRuntime, x_2d);
	lua_pushnumber(luaRuntime, y_2d);
	return 2;
	}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// ========================== END LIBRARY FUNCTIONS ============================


// ======================== LUA FUNCTIONS REGISTRATION =========================

// ==== Here we declare an array with all functions and their respective names.
// ==== This array has elements of type luaL_reg, which is a structure with
// ==== two fields: a string and a function pointer.

static const struct luaL_reg module_table [] = {
	{"distance",            distance},
	{"squaredDistance",     squaredDistance},
	{"vectorForScalar",     vectorForScalar},
	{"vectorDotProduct",    vectorDotProduct},
	{"vectorCrossProduct",  vectorCrossProduct},
	{"vectorDifference",    vectorDifference},
	{"vectorGetRotationTo", vectorGetRotationTo},
	{"vectorNormalize",     vectorNormalize},
	{"quaternionForVector", quaternionForVector},
	{"quaternionGetPitch",  quaternionGetPitch},
	{"quaternionGetYaw",    quaternionGetYaw},
	{"quaternionGetRoll",   quaternionGetRoll},
	{"quaternionGetAxis",   quaternionGetAxis},
	{"quaternionSlerp",     quaternionSlerp},
	{"quaternionNlerp",     quaternionNlerp},
	{"quaternionFromAxes",  quaternionFromAxes},
	{"pointToScreenProjection",  pointToScreenProjection},
	{NULL, NULL}  // <-- end-of-table sentinel
	};

// ==== Finally, we declare a main (initialization) function, using luaL_openlib:

int _INIT_(lua_State* luaRuntime)
	{
	ELFLuaRuntime::getInstance()->setRuntime(luaRuntime);
	//
	// Edit this fields as you need:
	moduleInfo.moduleName           = "ELFModule_3DMath";
	moduleInfo.moduleVerMajor       = 0;
	moduleInfo.moduleVerMinor       = 8;
	moduleInfo.moduleBuild          = 1;
	moduleInfo.moduleReleaseDate    = "November 2, 2006";
	moduleInfo.moduleNamespace      = "ELF3DMath";
	moduleInfo.moduleShortDesc      = "ELF 3D Math Module";
	moduleInfo.moduleDescription1   = "This module provides useful mathematical functions used in 3D spaces.";
	moduleInfo.moduleDescription2   = "The code inside this module is based on Ogre3D Mathematical classes.";
	moduleInfo.moduleVendor         = "jadLazarus of Emthesis Development";
	moduleInfo.moduleVendorHomepage = "www.emthesis.com";
	moduleInfo.moduleVendorEmail    = "carotenuto@emthesis.com";
	//
	#ifdef OLDLUA
	luaL_openlib(luaRuntime, moduleInfo.moduleNamespace, module_table, 0);
	#else
	// Lua 5.1:
	luaL_register(luaRuntime, moduleInfo.moduleNamespace, module_table);
	#endif
	//
	ELFLuaRuntime::logModuleInfo(&moduleInfo);
	//
	//
	return 1;
	}

// -----------------------------------------------------------------------------

} // extern "C" closure

/**
</module>
*/

/* Changelog:

v. 0.8.1.2
	Dec 4, 2006:
		.added function pointToScreenProjection
v. 0.8.1.1
	Nov 2, 2006:
		.removed some dangerous 'assert' functions, which could block execution
v. 0.8.1
	Aug 9, 2006:
		.Added functions distance and squaredDistance
v. 0.8.0
	Feb 14, 2006:
		.First public release (in ELF DevKit 1.0)

*/
