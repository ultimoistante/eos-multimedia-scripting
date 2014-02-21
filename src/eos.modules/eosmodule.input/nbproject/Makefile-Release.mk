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
	${OBJECTDIR}/EOSModuleDSO.o \
	${OBJECTDIR}/InputManager.o


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
LDLIBSOPTIONS=-Wl,-rpath,./lib ../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a -lOIS -llua

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../../bin/eosmodule.input

../../../bin/eosmodule.input: ../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a

../../../bin/eosmodule.input: ${OBJECTFILES}
	${MKDIR} -p ../../../bin
	${LINK.cc} -o ../../../bin/eosmodule.input ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -s -fPIC

${OBJECTDIR}/EOSModuleDSO.o: EOSModuleDSO.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I/usr/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSModuleDSO.o EOSModuleDSO.cpp

${OBJECTDIR}/InputManager.o: InputManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I/usr/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/InputManager.o InputManager.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../../bin/eosmodule.input

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
