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
	${OBJECTDIR}/SkeltrackManager.o


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
LDLIBSOPTIONS=-Wl,-rpath,./lib ../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a -lskeltrack-0.1 -lgfreenect-0.1 -lgdk_pixbuf-2.0 -lclutter-1.0

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../../bin/eosmodule.skeltrack

../../../bin/eosmodule.skeltrack: ../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a

../../../bin/eosmodule.skeltrack: ${OBJECTFILES}
	${MKDIR} -p ../../../bin
	${LINK.cc} -o ../../../bin/eosmodule.skeltrack ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -s -fPIC

${OBJECTDIR}/EOSModuleDSO.o: EOSModuleDSO.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../../../../eos-deps/lua-5.1.5/src -I../modulesdk -I/usr/include/glib-2.0/ -I/usr/local/include/skeltrack-0.1/ -I/usr/lib/glib-2.0/include -I/usr/include/gfreenect-0.1 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/cairo -I/usr/include/clutter-1.0 -I/usr/include/cogl -I/usr/include/pango-1.0 -I/usr/include/atk-1.0 -I/usr/include/json-glib-1.0 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSModuleDSO.o EOSModuleDSO.cpp

${OBJECTDIR}/SkeltrackManager.o: SkeltrackManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../../../../eos-deps/lua-5.1.5/src -I../modulesdk -I/usr/include/glib-2.0/ -I/usr/local/include/skeltrack-0.1/ -I/usr/lib/glib-2.0/include -I/usr/include/gfreenect-0.1 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/cairo -I/usr/include/clutter-1.0 -I/usr/include/cogl -I/usr/include/pango-1.0 -I/usr/include/atk-1.0 -I/usr/include/json-glib-1.0 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/SkeltrackManager.o SkeltrackManager.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../../bin/eosmodule.skeltrack

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
