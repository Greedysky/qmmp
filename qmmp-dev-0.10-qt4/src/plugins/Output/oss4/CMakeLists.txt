project(liboss4)


if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

INCLUDE(CheckIncludeFile)
INCLUDE(CheckCXXSourceCompiles)

# qt plugin
ADD_DEFINITIONS( -Wall )
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)
ADD_DEFINITIONS(-DQT_THREAD)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(QT_INCLUDES
  ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../
)
#oss4
include_directories(/usr/lib/oss/include /usr/local/lib/oss/include)
CHECK_INCLUDE_FILE(sys/soundcard.h SYS_SOUNDCARD_H_FOUND)
IF(SYS_SOUNDCARD_H_FOUND)
ADD_DEFINITIONS(-DHAVE_SYS_SOUNDCARD_H)
SET(CMAKE_REQUIRED_DEFINITIONS -DHAVE_SYS_SOUNDCARD_H)
ENDIF(SYS_SOUNDCARD_H_FOUND)


SET(CMAKE_REQUIRED_INCLUDES 
   /usr/lib/oss/include/
   /usr/local/lib/oss/include 
   ${CMAKE_REQUIRED_INCLUDES})

CHECK_CXX_SOURCE_COMPILES("
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_SYS_SOUNDCARD_H
#include <sys/soundcard.h>
#else
#include <soundcard.h>
#endif

#ifndef SNDCTL_DSP_COOKEDMODE
#error \\\"oss test failed\\\"
#endif


int main (int argc, char **argv)
{
   exit(0);
}" OSS4_SUPPORT)

IF(OSS4_SUPPORT)
SET(OSS4_FOUND TRUE CACHE INTERNAL "oss4")
ENDIF(OSS4_SUPPORT)


# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)


SET(liboss4_SRCS
  outputoss4factory.cpp
  outputoss4.cpp
  settingsdialog.cpp
)

SET(liboss4_HDRS
  outputoss4.h
)

SET(liboss4_RCCS translations/translations.qrc)

QT4_ADD_RESOURCES(liboss4_RCC_SRCS ${liboss4_RCCS})

SET(liboss4_UIS
    settingsdialog.ui
)

IF(OSS4_FOUND)
QT4_WRAP_UI(liboss4_UIS_H ${liboss4_UIS})

ADD_LIBRARY(oss4 MODULE ${liboss4_SRCS} ${liboss4_UIS_H} ${liboss4_RCC_SRCS} ${liboss4_HDRS})
add_dependencies(oss4 qmmp)
target_link_libraries(oss4 ${QT_LIBRARIES} libqmmp)
install(TARGETS oss4 DESTINATION ${LIB_DIR}/qmmp/Output)
ENDIF(OSS4_FOUND)
