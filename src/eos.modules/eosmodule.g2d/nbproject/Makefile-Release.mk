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
	${OBJECTDIR}/G2D.o \
	${OBJECTDIR}/G2DElement.o \
	${OBJECTDIR}/G2DRenderTarget.o \
	${OBJECTDIR}/G2DSprite.o \
	${OBJECTDIR}/G2DSpriteAnim.o


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
LDLIBSOPTIONS=../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a -lGLee

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../../bin/eosmodule.g2d

../../../bin/eosmodule.g2d: ../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a

../../../bin/eosmodule.g2d: ${OBJECTFILES}
	${MKDIR} -p ../../../bin
	${LINK.cc} -o ../../../bin/eosmodule.g2d ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/EOSModuleDSO.o: EOSModuleDSO.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../modulesdk -I../../../../eos-deps/vmmlib-master/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSModuleDSO.o EOSModuleDSO.cpp

${OBJECTDIR}/G2D.o: G2D.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../modulesdk -I../../../../eos-deps/vmmlib-master/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/G2D.o G2D.cpp

${OBJECTDIR}/G2DElement.o: G2DElement.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../modulesdk -I../../../../eos-deps/vmmlib-master/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/G2DElement.o G2DElement.cpp

${OBJECTDIR}/G2DRenderTarget.o: G2DRenderTarget.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../modulesdk -I../../../../eos-deps/vmmlib-master/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/G2DRenderTarget.o G2DRenderTarget.cpp

${OBJECTDIR}/G2DSprite.o: G2DSprite.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../modulesdk -I../../../../eos-deps/vmmlib-master/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/G2DSprite.o G2DSprite.cpp

${OBJECTDIR}/G2DSpriteAnim.o: G2DSpriteAnim.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../modulesdk -I../../../../eos-deps/vmmlib-master/include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/G2DSpriteAnim.o G2DSpriteAnim.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../../bin/eosmodule.g2d

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
