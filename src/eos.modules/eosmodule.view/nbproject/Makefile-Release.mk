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
	${OBJECTDIR}/View.o \
	${OBJECTDIR}/ViewPlatformSupport.o \
	${OBJECTDIR}/ViewSurface.o \
	${OBJECTDIR}/ViewWindow.o


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
LDLIBSOPTIONS=-Wl,-rpath,./lib ../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a ../../../../eos-deps/lua-5.1.5/src/liblua.a -lGL -lGLU -lGLee -lfreeimage -lXxf86vm

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../../bin/eosmodule.view

../../../bin/eosmodule.view: ../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a

../../../bin/eosmodule.view: ../../../../eos-deps/lua-5.1.5/src/liblua.a

../../../bin/eosmodule.view: ${OBJECTFILES}
	${MKDIR} -p ../../../bin
	${LINK.cc} -o ../../../bin/eosmodule.view ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -s -fPIC

${OBJECTDIR}/EOSModuleDSO.o: EOSModuleDSO.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/lua-5.1.5/src -I../../../../eos-deps/vmmlib-master/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSModuleDSO.o EOSModuleDSO.cpp

${OBJECTDIR}/View.o: View.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/lua-5.1.5/src -I../../../../eos-deps/vmmlib-master/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/View.o View.cpp

${OBJECTDIR}/ViewPlatformSupport.o: ViewPlatformSupport.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/lua-5.1.5/src -I../../../../eos-deps/vmmlib-master/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/ViewPlatformSupport.o ViewPlatformSupport.cpp

${OBJECTDIR}/ViewSurface.o: ViewSurface.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/lua-5.1.5/src -I../../../../eos-deps/vmmlib-master/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/ViewSurface.o ViewSurface.cpp

${OBJECTDIR}/ViewWindow.o: ViewWindow.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/lua-5.1.5/src -I../../../../eos-deps/vmmlib-master/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/ViewWindow.o ViewWindow.cpp

# Subprojects
.build-subprojects:
	cd ../modulesdk && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../../bin/eosmodule.view

# Subprojects
.clean-subprojects:
	cd ../modulesdk && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
