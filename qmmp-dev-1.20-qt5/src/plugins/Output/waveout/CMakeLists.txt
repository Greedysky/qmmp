project(libwaveout)

INCLUDE(CheckIncludeFile)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# Win32 waveout
CHECK_INCLUDE_FILE(windows.h WINDOWS_H_FOUND)

SET(libwaveout_SRCS
  outputwaveout.cpp
  outputwaveoutfactory.cpp
)

SET(libwaveout_HDRS
  outputwaveout.h
)

SET(libwaveout_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libwaveout_RCC_SRCS ${libwaveout_RCCS})


IF(WINDOWS_H_FOUND)
ADD_LIBRARY(waveout MODULE ${libwaveout_SRCS} ${libwaveout_RCC_SRCS} ${libwaveout_HDRS})
add_dependencies(waveout qmmp)
target_link_libraries(waveout Qt5::Widgets -lqmmp -lwinmm)
install(TARGETS waveout DESTINATION ${LIB_DIR}/qmmp/Output)
ENDIF(WINDOWS_H_FOUND)

