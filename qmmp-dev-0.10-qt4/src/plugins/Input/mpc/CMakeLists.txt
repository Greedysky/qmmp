project(libmpc)

INCLUDE(CheckIncludeFileCXX)

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

# libmpc
CHECK_INCLUDE_FILE_CXX(mpc/mpcdec.h MPC_HEADER_NEW_FOUND)
IF(NOT MPC_HEADER_NEW_FOUND)
CHECK_INCLUDE_FILE_CXX(mpcdec/mpcdec.h MPC_HEADER_OLD_FOUND)
ADD_DEFINITIONS(-DMPC_OLD_API)
ENDIF(NOT MPC_HEADER_NEW_FOUND)

FIND_LIBRARY(MPC_LIB NAME mpcdec PATHS /usr/lib /usr/lib32  /usr/local/lib /usr/local/lib32)

IF((MPC_HEADER_NEW_FOUND OR MPC_HEADER_OLD_FOUND) AND (NOT MPC_LIB MATCHES "^.*-NOTFOUND"))
SET(MPC_FOUND TRUE CACHE INTERNAL "mpc")
MESSAGE(STATUS "Found MusePack: ${MPC_LIB}")
ELSE((MPC_HEADER_NEW_FOUND OR MPC_HEADER_OLD_FOUND) AND (NOT MPC_LIB MATCHES "^.*-NOTFOUND"))
MESSAGE(STATUS "Could not find MusePack library")
ENDIF((MPC_HEADER_NEW_FOUND OR MPC_HEADER_OLD_FOUND) AND (NOT MPC_LIB MATCHES "^.*-NOTFOUND"))

include_directories(${TAGLIB_INCLUDE_DIRS})
link_directories(${TAGLIB_LIBRARY_DIRS})
ADD_DEFINITIONS(${TAGLIB_CFLAGS})

SET(libmpc_SRCS
  decoder_mpc.cpp
  decodermpcfactory.cpp
  mpcmetadatamodel.cpp
)

SET(libmpc_HDRS
  decoder_mpc.h
)

SET(libmpc_RCCS translations/translations.qrc)

QT4_ADD_RESOURCES(libmpc_RCC_SRCS ${libmpc_RCCS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(MPC_FOUND)
ADD_LIBRARY(mpc MODULE ${libmpc_SRCS} ${libmpc_RCC_SRCS} ${libmpc_HDRS})
add_dependencies(mpc qmmp)
target_link_libraries(mpc ${QT_LIBRARIES} libqmmp ${MPC_LIB} ${TAGLIB_LDFLAGS})
install(TARGETS mpc DESTINATION ${LIB_DIR}/qmmp/Input)
ENDIF(MPC_FOUND)
