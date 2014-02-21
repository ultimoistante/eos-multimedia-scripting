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
	${OBJECTDIR}/EOSKernel.o \
	${OBJECTDIR}/EOSLauncher.o \
	${OBJECTDIR}/EOSLuaEnvironment.o \
	${OBJECTDIR}/EOSLuaScript.o \
	${OBJECTDIR}/EOSMessage.o \
	${OBJECTDIR}/EOSProcess.o \
	${OBJECTDIR}/EOSRemoteCommandParser.o \
	${OBJECTDIR}/EOSScriptManager.o \
	${OBJECTDIR}/UDPSocketHandler.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/eos.kernel

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/eos.kernel: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/eos.kernel ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/EOSKernel.o: EOSKernel.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSKernel.o EOSKernel.cpp

${OBJECTDIR}/EOSLauncher.o: EOSLauncher.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSLauncher.o EOSLauncher.cpp

${OBJECTDIR}/EOSLuaEnvironment.o: EOSLuaEnvironment.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSLuaEnvironment.o EOSLuaEnvironment.cpp

${OBJECTDIR}/EOSLuaScript.o: EOSLuaScript.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSLuaScript.o EOSLuaScript.cpp

${OBJECTDIR}/EOSMessage.o: EOSMessage.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSMessage.o EOSMessage.cpp

${OBJECTDIR}/EOSProcess.o: EOSProcess.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSProcess.o EOSProcess.cpp

${OBJECTDIR}/EOSRemoteCommandParser.o: EOSRemoteCommandParser.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSRemoteCommandParser.o EOSRemoteCommandParser.cpp

${OBJECTDIR}/EOSScriptManager.o: EOSScriptManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSScriptManager.o EOSScriptManager.cpp

${OBJECTDIR}/UDPSocketHandler.o: UDPSocketHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -MMD -MP -MF $@.d -o ${OBJECTDIR}/UDPSocketHandler.o UDPSocketHandler.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/eos.kernel

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
