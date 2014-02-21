#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/EOSG3D.o \
	${OBJECTDIR}/EOSModuleDSO.o \
	${OBJECTDIR}/EOSOdeCollisionObject.o \
	${OBJECTDIR}/EOSOdeCollisionWorld.o \
	${OBJECTDIR}/EOSOdeDebugObjects.o \
	${OBJECTDIR}/EOSOgreDotSceneLoader.o \
	${OBJECTDIR}/EOSOgreObject.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-DdSINGLE
CXXFLAGS=-DdSINGLE

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Wl,-rpath,./lib ../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a ../../../../eos-deps/ogre_build/lib/libOgreMain.so ../../../../eos-deps/tinyxml2/libtinyxml2.a ../../../../eos-deps/libode.so ../../../../eos-deps/lua-5.1.5/src/liblua.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../../bin/eosmodule.g3d

../../../bin/eosmodule.g3d: ../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a

../../../bin/eosmodule.g3d: ../../../../eos-deps/ogre_build/lib/libOgreMain.so

../../../bin/eosmodule.g3d: ../../../../eos-deps/tinyxml2/libtinyxml2.a

../../../bin/eosmodule.g3d: ../../../../eos-deps/libode.so

../../../bin/eosmodule.g3d: ../../../../eos-deps/lua-5.1.5/src/liblua.a

../../../bin/eosmodule.g3d: ${OBJECTFILES}
	${MKDIR} -p ../../../bin
	${LINK.cc} -o ../../../bin/eosmodule.g3d ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -s -fPIC

${OBJECTDIR}/EOSG3D.o: EOSG3D.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/ogre_src_v1-8-1/OgreMain/include -I../../../../eos-deps/ogre_build/include -I../../../../eos-deps/tinyxml2 -I../../../../eos-deps/lua-5.1.5/src -DdSINGLE -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSG3D.o EOSG3D.cpp

${OBJECTDIR}/EOSModuleDSO.o: EOSModuleDSO.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/ogre_src_v1-8-1/OgreMain/include -I../../../../eos-deps/ogre_build/include -I../../../../eos-deps/tinyxml2 -I../../../../eos-deps/lua-5.1.5/src -DdSINGLE -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSModuleDSO.o EOSModuleDSO.cpp

${OBJECTDIR}/EOSOdeCollisionObject.o: EOSOdeCollisionObject.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/ogre_src_v1-8-1/OgreMain/include -I../../../../eos-deps/ogre_build/include -I../../../../eos-deps/tinyxml2 -I../../../../eos-deps/lua-5.1.5/src -DdSINGLE -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSOdeCollisionObject.o EOSOdeCollisionObject.cpp

${OBJECTDIR}/EOSOdeCollisionWorld.o: EOSOdeCollisionWorld.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/ogre_src_v1-8-1/OgreMain/include -I../../../../eos-deps/ogre_build/include -I../../../../eos-deps/tinyxml2 -I../../../../eos-deps/lua-5.1.5/src -DdSINGLE -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSOdeCollisionWorld.o EOSOdeCollisionWorld.cpp

${OBJECTDIR}/EOSOdeDebugObjects.o: EOSOdeDebugObjects.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/ogre_src_v1-8-1/OgreMain/include -I../../../../eos-deps/ogre_build/include -I../../../../eos-deps/tinyxml2 -I../../../../eos-deps/lua-5.1.5/src -DdSINGLE -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSOdeDebugObjects.o EOSOdeDebugObjects.cpp

${OBJECTDIR}/EOSOgreDotSceneLoader.o: EOSOgreDotSceneLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/ogre_src_v1-8-1/OgreMain/include -I../../../../eos-deps/ogre_build/include -I../../../../eos-deps/tinyxml2 -I../../../../eos-deps/lua-5.1.5/src -DdSINGLE -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSOgreDotSceneLoader.o EOSOgreDotSceneLoader.cpp

${OBJECTDIR}/EOSOgreObject.o: EOSOgreObject.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/ogre_src_v1-8-1/OgreMain/include -I../../../../eos-deps/ogre_build/include -I../../../../eos-deps/tinyxml2 -I../../../../eos-deps/lua-5.1.5/src -DdSINGLE -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSOgreObject.o EOSOgreObject.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../../bin/eosmodule.g3d

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
