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
CND_PLATFORM=GNU-Linux
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
	${OBJECTDIR}/GUI/GUI.o \
	${OBJECTDIR}/GUI/GUI_Element.o \
	${OBJECTDIR}/GUI/GUI_Linux.o \
	${OBJECTDIR}/GUI/GUI_Primitives.o \
	${OBJECTDIR}/GUI/font.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/tools.o \
	${OBJECTDIR}/webcam.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-m64 -DUNICODE
CXXFLAGS=-m64 -DUNICODE

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clicker

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clicker: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/clicker ${OBJECTFILES} ${LDLIBSOPTIONS} -pthread -lX11 -lXtst

${OBJECTDIR}/GUI/GUI.o: GUI/GUI.cpp
	${MKDIR} -p ${OBJECTDIR}/GUI
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GUI/GUI.o GUI/GUI.cpp

${OBJECTDIR}/GUI/GUI_Element.o: GUI/GUI_Element.cpp
	${MKDIR} -p ${OBJECTDIR}/GUI
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GUI/GUI_Element.o GUI/GUI_Element.cpp

${OBJECTDIR}/GUI/GUI_Linux.o: GUI/GUI_Linux.cpp
	${MKDIR} -p ${OBJECTDIR}/GUI
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GUI/GUI_Linux.o GUI/GUI_Linux.cpp

${OBJECTDIR}/GUI/GUI_Primitives.o: GUI/GUI_Primitives.cpp
	${MKDIR} -p ${OBJECTDIR}/GUI
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GUI/GUI_Primitives.o GUI/GUI_Primitives.cpp

${OBJECTDIR}/GUI/font.o: GUI/font.cpp
	${MKDIR} -p ${OBJECTDIR}/GUI
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/GUI/font.o GUI/font.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/tools.o: tools.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/tools.o tools.cpp

${OBJECTDIR}/webcam.o: webcam.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/webcam.o webcam.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
