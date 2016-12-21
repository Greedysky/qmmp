project(libvorbis)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
              
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# libvorbis and taglib
pkg_check_modules(VORBIS ogg vorbis vorbisfile)

include_directories(${VORBIS_INCLUDE_DIRS}  ${TAGLIB_INCLUDE_DIRS})
link_directories(${VORBIS_LIBRARY_DIRS} ${TAGLIB_LIBRARY_DIRS})
ADD_DEFINITIONS(${VORBIS_CFLAGS} ${TAGLIB_CFLAGS})

SET(libvorbis_SRCS
  decoder_vorbis.cpp
  decodervorbisfactory.cpp
  vorbismetadatamodel.cpp
  replaygainreader.cpp
)

SET(libvorbis_HDRS
  decoder_vorbis.h
  replaygainreader.h
)

SET(libvorbis_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libvorbis_RCC_SRCS ${libvorbis_RCCS})

# user interface

QT5_WRAP_UI(libvorbis_UIS_H ${libvorbis_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(VORBIS_FOUND)
ADD_LIBRARY(vorbis MODULE ${libvorbis_SRCS} ${libvorbis_RCC_SRCS} ${libvorbis_HDRS})
add_dependencies(vorbis qmmp)
target_link_libraries(vorbis Qt5::Widgets -lqmmp ${VORBIS_LDFLAGS} ${TAGLIB_LDFLAGS})
install(TARGETS vorbis DESTINATION ${LIB_DIR}/qmmp/Input)
ENDIF(VORBIS_FOUND)
