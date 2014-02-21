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
CND_CONF=Debug
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
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libeosmodule.g3d.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libeosmodule.g3d.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libeosmodule.g3d.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/EOSG3D.o: EOSG3D.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSG3D.o EOSG3D.cpp

${OBJECTDIR}/EOSModuleDSO.o: EOSModuleDSO.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSModuleDSO.o EOSModuleDSO.cpp

${OBJECTDIR}/EOSOdeCollisionObject.o: EOSOdeCollisionObject.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSOdeCollisionObject.o EOSOdeCollisionObject.cpp

${OBJECTDIR}/EOSOdeCollisionWorld.o: EOSOdeCollisionWorld.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSOdeCollisionWorld.o EOSOdeCollisionWorld.cpp

${OBJECTDIR}/EOSOdeDebugObjects.o: EOSOdeDebugObjects.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSOdeDebugObjects.o EOSOdeDebugObjects.cpp

${OBJECTDIR}/EOSOgreDotSceneLoader.o: EOSOgreDotSceneLoader.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSOgreDotSceneLoader.o EOSOgreDotSceneLoader.cpp

${OBJECTDIR}/EOSOgreObject.o: EOSOgreObject.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSOgreObject.o EOSOgreObject.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libeosmodule.g3d.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
