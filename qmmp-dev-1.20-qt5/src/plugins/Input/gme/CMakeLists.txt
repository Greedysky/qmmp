project(libgme)

INCLUDE(CheckIncludeFileCXX)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# libgme
CHECK_INCLUDE_FILE_CXX(gme/gme.h GME_HEADER_FOUND)
FIND_LIBRARY(GME_LIB NAME gme PATHS /usr/lib /usr/lib32  /usr/local/lib /usr/local/lib32)

IF(GME_HEADER_FOUND AND (NOT GME_LIB MATCHES "^.*-NOTFOUND"))
SET(GME_FOUND TRUE CACHE INTERNAL "GME")
MESSAGE(STATUS "Found Game Music Emu: ${GME_LIB}")
ELSE(GME_HEADER_FOUND AND (NOT GME_LIB MATCHES "^.*-NOTFOUND"))
MESSAGE(STATUS "Could not find Game Music Emu library")
ENDIF(GME_HEADER_FOUND AND (NOT GME_LIB MATCHES "^.*-NOTFOUND"))


SET(libgme_SRCS
  decoder_gme.cpp
  decodergmefactory.cpp
  gmehelper.cpp
  settingsdialog.cpp
)

SET(libgme_HDRS
  decoder_gme.h
  gmehelper.h
)

SET(libgme_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libgme_RCC_SRCS ${libgme_RCCS})

# user interface

SET(libgme_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libgme_UIS_H ${libgme_UIS})

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(GME_FOUND)
ADD_LIBRARY(gme MODULE ${libgme_SRCS} ${libgme_RCC_SRCS} ${libgme_HDRS} ${libgme_UIS_H})
add_dependencies(gme qmmp)
target_link_libraries(gme Qt5::Widgets -lqmmp ${GME_LIB})
install(TARGETS gme DESTINATION ${LIB_DIR}/qmmp/Input)
ENDIF(GME_FOUND)
