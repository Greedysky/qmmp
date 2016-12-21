project(libjack)


INCLUDE (CheckCXXSourceCompiles)

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

# libjack and libsoxr
pkg_check_modules(JACK jack soxr)

include_directories(${JACK_INCLUDE_DIRS})
link_directories(${JACK_LIBRARY_DIRS})
ADD_DEFINITIONS(${JACK_CFLAGS})


IF(JACK_FOUND)
CHECK_CXX_SOURCE_COMPILES("
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <jack/jack.h>

int main (int argc, char **argv)
{
   jack_latency_range_t range;
   exit(0);
}" JACK_NEW_API)

ENDIF(JACK_FOUND)

IF(JACK_NEW_API)
ADD_DEFINITIONS(-DJACK_NEW_API)
ENDIF(JACK_NEW_API)


SET(libjack_SRCS
  outputjackfactory.cpp
  outputjack.cpp
  bio2jack.c
)

SET(libjack_HDRS
  bio2jack.h
  outputjack.h
)

SET(libjack_RCCS translations/translations.qrc)

QT4_ADD_RESOURCES(libjack_RCC_SRCS ${libjack_RCCS})


IF(JACK_FOUND)
ADD_LIBRARY(jack MODULE ${libjack_SRCS} ${libjack_RCC_SRCS} ${libjack_HDRS})
add_dependencies(jack qmmp)
set_target_properties(jack PROPERTIES
                           COMPILE_FLAGS -pthread
                           LINK_FLAGS -pthread
)
target_link_libraries(jack ${QT_LIBRARIES} libqmmp ${JACK_LDFLAGS})
install(TARGETS jack DESTINATION ${LIB_DIR}/qmmp/Output)
ENDIF(JACK_FOUND)

