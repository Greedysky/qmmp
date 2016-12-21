project(libmad)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
    cmake_policy(SET CMP0005 NEW)
endif(COMMAND cmake_policy) 

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# libmad
pkg_search_module(MAD libmad mad)

include_directories(${MAD_INCLUDE_DIRS} ${TAGLIB_INCLUDE_DIRS})
link_directories(${MAD_LIBRARY_DIRS} ${TAGLIB_LIBRARY_DIRS})
ADD_DEFINITIONS(${MAD_CFLAGS} ${TAGLIB_CFLAGS})

SET(libmad_SRCS
  decoder_mad.cpp
  decodermadfactory.cpp
  settingsdialog.cpp
  tagextractor.cpp
  mpegmetadatamodel.cpp
  replaygainreader.cpp
)

SET(libmad_HDRS
  decoder_mad.h
  tagextractor.h
  replaygainreader.h
)

SET(libmad_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libmad_RCC_SRCS ${libmad_RCCS})

# user interface


SET(libmad_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libmad_UIS_H ${libmad_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(MAD_FOUND)
ADD_LIBRARY(mad MODULE ${libmad_SRCS} ${libmad_UIS_H} ${libmad_RCC_SRCS} ${libmad_HDRS})
add_dependencies(mad qmmp)
target_link_libraries(mad Qt5::Widgets -lqmmp ${MAD_LDFLAGS} ${TAGLIB_LDFLAGS})
install(TARGETS mad DESTINATION ${LIB_DIR}/qmmp/Input)
ENDIF(MAD_FOUND)
