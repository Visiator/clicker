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
	${OBJECTDIR}/_ext/57b43f/analiz.o \
	${OBJECTDIR}/_ext/57b43f/fragments_queue.o \
	${OBJECTDIR}/_ext/57b43f/frame.o \
	${OBJECTDIR}/_ext/57b43f/pcap.o \
	${OBJECTDIR}/_ext/57b43f/program.o \
	${OBJECTDIR}/_ext/57b43f/sessions.o \
	${OBJECTDIR}/GUI/GUI.o \
	${OBJECTDIR}/GUI/GUI_Element.o \
	${OBJECTDIR}/GUI/GUI_Linux.o \
	${OBJECTDIR}/GUI/GUI_Primitives.o \
	${OBJECTDIR}/GUI/font.o \
	${OBJECTDIR}/clicker.o \
	${OBJECTDIR}/global.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/mikrotik.o \
	${OBJECTDIR}/sniffer.o \
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

${OBJECTDIR}/_ext/57b43f/analiz.o: /prj/clicker/analiz.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/57b43f
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/57b43f/analiz.o /prj/clicker/analiz.cpp

${OBJECTDIR}/_ext/57b43f/fragments_queue.o: /prj/clicker/fragments_queue.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/57b43f
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/57b43f/fragments_queue.o /prj/clicker/fragments_queue.cpp

${OBJECTDIR}/_ext/57b43f/frame.o: /prj/clicker/frame.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/57b43f
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/57b43f/frame.o /prj/clicker/frame.cpp

${OBJECTDIR}/_ext/57b43f/pcap.o: /prj/clicker/pcap.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/57b43f
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/57b43f/pcap.o /prj/clicker/pcap.cpp

${OBJECTDIR}/_ext/57b43f/program.o: /prj/clicker/program.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/57b43f
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/57b43f/program.o /prj/clicker/program.cpp

${OBJECTDIR}/_ext/57b43f/sessions.o: /prj/clicker/sessions.cpp
	${MKDIR} -p ${OBJECTDIR}/_ext/57b43f
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/57b43f/sessions.o /prj/clicker/sessions.cpp

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

${OBJECTDIR}/clicker.o: clicker.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/clicker.o clicker.cpp

${OBJECTDIR}/global.o: global.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/global.o global.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/mikrotik.o: mikrotik.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/mikrotik.o mikrotik.cpp

${OBJECTDIR}/sniffer.o: sniffer.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -std=c++14 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sniffer.o sniffer.cpp

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
