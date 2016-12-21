project(libopus)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# libopus and libopusfile
pkg_check_modules(OPUS opus>=1.0.2 opusfile>=0.2)

include_directories(${OPUS_INCLUDE_DIRS}  ${TAGLIB_INCLUDE_DIRS})
link_directories(${OPUS_LIBRARY_DIRS} ${TAGLIB_LIBRARY_DIRS})
ADD_DEFINITIONS(${OPUS_CFLAGS} ${TAGLIB_CFLAGS})

SET(libopus_SRCS
  decoder_opus.cpp
  decoderopusfactory.cpp
  opusmetadatamodel.cpp
  replaygainreader.cpp
)

SET(libopus_HDRS
  decoder_opus.h
  replaygainreader.h
)

SET(libopus_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libopus_RCC_SRCS ${libopus_RCCS})

# user interface

QT5_WRAP_UI(libopus_UIS_H ${libopus_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(OPUS_FOUND)
ADD_LIBRARY(opus MODULE ${libopus_SRCS} ${libopus_RCC_SRCS} ${libopus_HDRS})
add_dependencies(opus qmmp)
target_link_libraries(opus Qt5::Widgets -lqmmp ${OPUS_LDFLAGS} ${TAGLIB_LDFLAGS})
install(TARGETS opus DESTINATION ${LIB_DIR}/qmmp/Input)
ENDIF(OPUS_FOUND)
