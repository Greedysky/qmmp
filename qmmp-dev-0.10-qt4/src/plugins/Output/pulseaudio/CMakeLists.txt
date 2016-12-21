project(libpulseaudio)


if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

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

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# pulseaudio
pkg_check_modules(PULSE libpulse-simple>=0.9.15)

include_directories(${PULSE_INCLUDE_DIRS})
link_directories(${PULSE_LIBRARY_DIRS})
ADD_DEFINITIONS(${PULSE_CFLAGS})

SET(libpulseaudio_SRCS
  outputpulseaudio.cpp
  outputpulseaudiofactory.cpp
)

SET(libpulseaudio_HDRS
  outputpulseaudio.h
)

SET(libpulseaudio_RCCS translations/translations.qrc)

QT4_ADD_RESOURCES(libpulseaudio_RCC_SRCS ${libpulseaudio_RCCS})

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(PULSE_FOUND)
ADD_LIBRARY(pulseaudio MODULE ${libpulseaudio_SRCS} ${libpulseaudio_UIS_H}
  ${libpulseaudio_RCC_SRCS} ${libpulseaudio_HDRS})
add_dependencies(pulseaudio qmmp)
target_link_libraries(pulseaudio ${QT_LIBRARIES} libqmmp ${PULSE_LDFLAGS})
install(TARGETS pulseaudio DESTINATION ${LIB_DIR}/qmmp/Output)
ENDIF(PULSE_FOUND)

