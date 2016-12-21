project(libjack)

INCLUDE (CheckCXXSourceCompiles)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# libjack and libsoxr
pkg_check_modules(JACK jack>=1.9.8 soxr)

IF(NOT JACK_FOUND)
pkg_check_modules(JACK jack>=0.122.0 soxr)
ENDIF(NOT JACK_FOUND)

include_directories(${JACK_INCLUDE_DIRS})
link_directories(${JACK_LIBRARY_DIRS})
ADD_DEFINITIONS(${JACK_CFLAGS})


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

QT5_ADD_RESOURCES(libjack_RCC_SRCS ${libjack_RCCS})


IF(JACK_FOUND)
ADD_LIBRARY(jack MODULE ${libjack_SRCS} ${libjack_RCC_SRCS} ${libjack_HDRS})
add_dependencies(jack qmmp)
set_target_properties(jack PROPERTIES
                           COMPILE_FLAGS -pthread
                           LINK_FLAGS -pthread
)
target_link_libraries(jack Qt5::Widgets -lqmmp ${JACK_LDFLAGS})
install(TARGETS jack DESTINATION ${LIB_DIR}/qmmp/Output)
ENDIF(JACK_FOUND)

