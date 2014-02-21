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
	${OBJECTDIR}/_ext/942975184/Matrix4.o \
	${OBJECTDIR}/_ext/942975184/Matrix3.o \
	${OBJECTDIR}/_ext/942975184/Vector3.o \
	${OBJECTDIR}/_ext/942975184/Quaternion.o \
	${OBJECTDIR}/_ext/942975184/Vector4.o \
	${OBJECTDIR}/_ext/942975184/MathUtils.o \
	${OBJECTDIR}/_ext/942975184/Plane.o


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
LDLIBSOPTIONS=-Wl,-rpath,./lib ../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a -lavcodec -lavformat -lswscale -lavutil -lm -lz

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../../bin/eosmodule.video

../../../bin/eosmodule.video: ../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a

../../../bin/eosmodule.video: ${OBJECTFILES}
	${MKDIR} -p ../../../bin
	${LINK.cc} -shared -o ../../../bin/eosmodule.video -s -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/EOSModuleDSO.o: EOSModuleDSO.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I/usr/include/libavformat/ -I/usr/include/libswscale/ -I/usr/include/libavcodec/ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSModuleDSO.o EOSModuleDSO.cpp

${OBJECTDIR}/_ext/942975184/Matrix4.o: ../eosmodule.math/Matrix4.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/942975184
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I/usr/include/libavformat/ -I/usr/include/libswscale/ -I/usr/include/libavcodec/ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/942975184/Matrix4.o ../eosmodule.math/Matrix4.cpp

${OBJECTDIR}/_ext/942975184/Matrix3.o: ../eosmodule.math/Matrix3.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/942975184
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I/usr/include/libavformat/ -I/usr/include/libswscale/ -I/usr/include/libavcodec/ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/942975184/Matrix3.o ../eosmodule.math/Matrix3.cpp

${OBJECTDIR}/_ext/942975184/Vector3.o: ../eosmodule.math/Vector3.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/942975184
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I/usr/include/libavformat/ -I/usr/include/libswscale/ -I/usr/include/libavcodec/ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/942975184/Vector3.o ../eosmodule.math/Vector3.cpp

${OBJECTDIR}/_ext/942975184/Quaternion.o: ../eosmodule.math/Quaternion.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/942975184
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I/usr/include/libavformat/ -I/usr/include/libswscale/ -I/usr/include/libavcodec/ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/942975184/Quaternion.o ../eosmodule.math/Quaternion.cpp

${OBJECTDIR}/_ext/942975184/Vector4.o: ../eosmodule.math/Vector4.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/942975184
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I/usr/include/libavformat/ -I/usr/include/libswscale/ -I/usr/include/libavcodec/ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/942975184/Vector4.o ../eosmodule.math/Vector4.cpp

${OBJECTDIR}/_ext/942975184/MathUtils.o: ../eosmodule.math/MathUtils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/942975184
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I/usr/include/libavformat/ -I/usr/include/libswscale/ -I/usr/include/libavcodec/ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/942975184/MathUtils.o ../eosmodule.math/MathUtils.cpp

${OBJECTDIR}/_ext/942975184/Plane.o: ../eosmodule.math/Plane.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/942975184
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -I/usr/include/libavformat/ -I/usr/include/libswscale/ -I/usr/include/libavcodec/ -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/942975184/Plane.o ../eosmodule.math/Plane.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../../bin/eosmodule.video

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
