project(libflac)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
    cmake_policy(SET CMP0005 NEW)
endif(COMMAND cmake_policy) 

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# libflac
pkg_search_module(FLAC flac)

IF(NOT FLAC_LINK_FLAGS)
  SET(FLAC_LINK_FLAGS -lFLAC)
ENDIF(NOT FLAC_LINK_FLAGS)

include_directories(${FLAC_INCLUDE_DIRS} ${TAGLIB_INCLUDE_DIRS})
link_directories(${FLAC_LIBRARY_DIRS} ${TAGLIB_LIBRARY_DIRS})
ADD_DEFINITIONS(${FLAC_CFLAGS})
ADD_DEFINITIONS(${TAGLIB_CFLAGS})

SET(libflac_SRCS
  decoder_flac.cpp
  decoderflacfactory.cpp
  flacmetadatamodel.cpp
  cueparser.cpp
  replaygainreader.cpp
)

SET(libflac_HDRS
  decoder_flac.h
  cueparser.h
  replaygainreader.h
)

SET(libflac_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libflac_RCC_SRCS ${libflac_RCCS})

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(FLAC_FOUND)
ADD_LIBRARY(flac MODULE ${libflac_SRCS} ${libflac_RCC_SRCS} ${libflac_HDRS})
add_dependencies(flac qmmp)
target_link_libraries(flac Qt5::Widgets -lqmmp ${FLAC_LDFLAGS} ${TAGLIB_LDFLAGS})
install(TARGETS flac DESTINATION ${LIB_DIR}/qmmp/Input)
ENDIF(FLAC_FOUND)
