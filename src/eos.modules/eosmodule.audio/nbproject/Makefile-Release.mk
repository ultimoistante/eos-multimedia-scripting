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
	${OBJECTDIR}/EOSAudioDevice.o \
	${OBJECTDIR}/EOSModuleDSO.o \
	${OBJECTDIR}/EOSResourceManager.o \
	${OBJECTDIR}/EOSSound.o \
	${OBJECTDIR}/EOSSoundDUMB.o \
	${OBJECTDIR}/EOSSoundFactory.o \
	${OBJECTDIR}/EOSSoundOGG.o \
	${OBJECTDIR}/EOSSoundWAV.o \
	${OBJECTDIR}/tinythread.o


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
LDLIBSOPTIONS=-Wl,-rpath,./lib ../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a -llua ../../../../eos-deps/openal-soft-1.14/build/libopenal.so -logg -lvorbis -lvorbisfile ../../../../eos-deps/dumb-0.9.3/lib/unix/libdumb.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../../bin/eosmodule.audio

../../../bin/eosmodule.audio: ../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a

../../../bin/eosmodule.audio: ../../../../eos-deps/openal-soft-1.14/build/libopenal.so

../../../bin/eosmodule.audio: ../../../../eos-deps/dumb-0.9.3/lib/unix/libdumb.a

../../../bin/eosmodule.audio: ${OBJECTFILES}
	${MKDIR} -p ../../../bin
	${LINK.cc} -o ../../../bin/eosmodule.audio ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -s -fPIC

${OBJECTDIR}/EOSAudioDevice.o: EOSAudioDevice.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/openal-soft-1.14/include -I../../../../eos-deps/dumb-0.9.3/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSAudioDevice.o EOSAudioDevice.cpp

${OBJECTDIR}/EOSModuleDSO.o: EOSModuleDSO.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/openal-soft-1.14/include -I../../../../eos-deps/dumb-0.9.3/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSModuleDSO.o EOSModuleDSO.cpp

${OBJECTDIR}/EOSResourceManager.o: EOSResourceManager.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/openal-soft-1.14/include -I../../../../eos-deps/dumb-0.9.3/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSResourceManager.o EOSResourceManager.cpp

${OBJECTDIR}/EOSSound.o: EOSSound.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/openal-soft-1.14/include -I../../../../eos-deps/dumb-0.9.3/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSSound.o EOSSound.cpp

${OBJECTDIR}/EOSSoundDUMB.o: EOSSoundDUMB.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/openal-soft-1.14/include -I../../../../eos-deps/dumb-0.9.3/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSSoundDUMB.o EOSSoundDUMB.cpp

${OBJECTDIR}/EOSSoundFactory.o: EOSSoundFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/openal-soft-1.14/include -I../../../../eos-deps/dumb-0.9.3/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSSoundFactory.o EOSSoundFactory.cpp

${OBJECTDIR}/EOSSoundOGG.o: EOSSoundOGG.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/openal-soft-1.14/include -I../../../../eos-deps/dumb-0.9.3/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSSoundOGG.o EOSSoundOGG.cpp

${OBJECTDIR}/EOSSoundWAV.o: EOSSoundWAV.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/openal-soft-1.14/include -I../../../../eos-deps/dumb-0.9.3/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSSoundWAV.o EOSSoundWAV.cpp

${OBJECTDIR}/tinythread.o: tinythread.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I../../../../eos-deps/openal-soft-1.14/include -I../../../../eos-deps/dumb-0.9.3/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/tinythread.o tinythread.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../../bin/eosmodule.audio

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
