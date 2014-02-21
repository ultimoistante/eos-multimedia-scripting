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
	${OBJECTDIR}/EOSWeb.o \
	${OBJECTDIR}/EOSWebPage.o \
	${OBJECTDIR}/tinythread.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-pthread -I/usr/include/webkitgtk-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/gtk-2.0 -I/usr/include/libsoup-2.4 -I/usr/lib/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng15 -I/usr/include/harfbuzz -I/usr/include/libxml2
CXXFLAGS=-pthread -I/usr/include/webkitgtk-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/gtk-2.0 -I/usr/include/libsoup-2.4 -I/usr/lib/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng15 -I/usr/include/harfbuzz -I/usr/include/libxml2

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-Wl,-rpath,./lib ../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../../../bin/eosmodule.web

../../../bin/eosmodule.web: ../modulesdk/dist/Release/GNU-Linux-x86/libmodulesdk.a

../../../bin/eosmodule.web: ${OBJECTFILES}
	${MKDIR} -p ../../../bin
	${LINK.cc} -o ../../../bin/eosmodule.web ${OBJECTFILES} ${LDLIBSOPTIONS} -lwebkitgtk-1.0 -lgtk-x11-2.0 -lsoup-2.4 -ljavascriptcoregtk-1.0 -lgdk-x11-2.0 -latk-1.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfreetype -lfontconfig -lgio-2.0 -lgobject-2.0 -lglib-2.0 -shared -s -fPIC

${OBJECTDIR}/EOSModuleDSO.o: EOSModuleDSO.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -pthread -I/usr/include/webkitgtk-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/gtk-2.0 -I/usr/include/libsoup-2.4 -I/usr/lib/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng15 -I/usr/include/harfbuzz -I/usr/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSModuleDSO.o EOSModuleDSO.cpp

${OBJECTDIR}/EOSWeb.o: EOSWeb.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -pthread -I/usr/include/webkitgtk-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/gtk-2.0 -I/usr/include/libsoup-2.4 -I/usr/lib/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng15 -I/usr/include/harfbuzz -I/usr/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSWeb.o EOSWeb.cpp

${OBJECTDIR}/EOSWebPage.o: EOSWebPage.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -pthread -I/usr/include/webkitgtk-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/gtk-2.0 -I/usr/include/libsoup-2.4 -I/usr/lib/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng15 -I/usr/include/harfbuzz -I/usr/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/EOSWebPage.o EOSWebPage.cpp

${OBJECTDIR}/tinythread.o: tinythread.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -s -I../modulesdk -pthread -I/usr/include/webkitgtk-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/gtk-2.0 -I/usr/include/libsoup-2.4 -I/usr/lib/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng15 -I/usr/include/harfbuzz -I/usr/include/libxml2 -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/tinythread.o tinythread.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../../../bin/eosmodule.web

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
