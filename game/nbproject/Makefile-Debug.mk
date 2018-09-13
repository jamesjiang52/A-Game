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
CND_PLATFORM=MinGW-Windows
CND_DLIB_EXT=dll
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/code/ActiveEffectClass.o \
	${OBJECTDIR}/code/ArmorClass.o \
	${OBJECTDIR}/code/Armors.o \
	${OBJECTDIR}/code/Combat.o \
	${OBJECTDIR}/code/DirectionClass.o \
	${OBJECTDIR}/code/Game.o \
	${OBJECTDIR}/code/GenericEnemyClass.o \
	${OBJECTDIR}/code/InteractableObjectClass.o \
	${OBJECTDIR}/code/Journal.o \
	${OBJECTDIR}/code/LocationClass.o \
	${OBJECTDIR}/code/MiscObjectClass.o \
	${OBJECTDIR}/code/MiscObjects.o \
	${OBJECTDIR}/code/PlayerClass.o \
	${OBJECTDIR}/code/PotionClass.o \
	${OBJECTDIR}/code/Potions.o \
	${OBJECTDIR}/code/Scenes.o \
	${OBJECTDIR}/code/ShieldClass.o \
	${OBJECTDIR}/code/Shields.o \
	${OBJECTDIR}/code/Utilities.o \
	${OBJECTDIR}/code/WeaponClass.o \
	${OBJECTDIR}/code/Weapons.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-O3 -Wall -Wextra -Wpedantic
CXXFLAGS=-O3 -Wall -Wextra -Wpedantic

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/game.exe

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/game.exe: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/game ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/code/ActiveEffectClass.o: code/ActiveEffectClass.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/ActiveEffectClass.o code/ActiveEffectClass.cpp

${OBJECTDIR}/code/ArmorClass.o: code/ArmorClass.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/ArmorClass.o code/ArmorClass.cpp

${OBJECTDIR}/code/Armors.o: code/Armors.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Armors.o code/Armors.cpp

${OBJECTDIR}/code/Combat.o: code/Combat.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Combat.o code/Combat.cpp

${OBJECTDIR}/code/DirectionClass.o: code/DirectionClass.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/DirectionClass.o code/DirectionClass.cpp

${OBJECTDIR}/code/Game.o: code/Game.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Game.o code/Game.cpp

${OBJECTDIR}/code/GenericEnemyClass.o: code/GenericEnemyClass.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/GenericEnemyClass.o code/GenericEnemyClass.cpp

${OBJECTDIR}/code/InteractableObjectClass.o: code/InteractableObjectClass.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/InteractableObjectClass.o code/InteractableObjectClass.cpp

${OBJECTDIR}/code/Journal.o: code/Journal.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Journal.o code/Journal.cpp

${OBJECTDIR}/code/LocationClass.o: code/LocationClass.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/LocationClass.o code/LocationClass.cpp

${OBJECTDIR}/code/MiscObjectClass.o: code/MiscObjectClass.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/MiscObjectClass.o code/MiscObjectClass.cpp

${OBJECTDIR}/code/MiscObjects.o: code/MiscObjects.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/MiscObjects.o code/MiscObjects.cpp

${OBJECTDIR}/code/PlayerClass.o: code/PlayerClass.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/PlayerClass.o code/PlayerClass.cpp

${OBJECTDIR}/code/PotionClass.o: code/PotionClass.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/PotionClass.o code/PotionClass.cpp

${OBJECTDIR}/code/Potions.o: code/Potions.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Potions.o code/Potions.cpp

${OBJECTDIR}/code/Scenes.o: code/Scenes.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Scenes.o code/Scenes.cpp

${OBJECTDIR}/code/ShieldClass.o: code/ShieldClass.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/ShieldClass.o code/ShieldClass.cpp

${OBJECTDIR}/code/Shields.o: code/Shields.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Shields.o code/Shields.cpp

${OBJECTDIR}/code/Utilities.o: code/Utilities.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Utilities.o code/Utilities.cpp

${OBJECTDIR}/code/WeaponClass.o: code/WeaponClass.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/WeaponClass.o code/WeaponClass.cpp

${OBJECTDIR}/code/Weapons.o: code/Weapons.cpp
	${MKDIR} -p ${OBJECTDIR}/code
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/code/Weapons.o code/Weapons.cpp

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
