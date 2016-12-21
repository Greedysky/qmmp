project(libmodplug)

INCLUDE(CheckIncludeFile)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# modplug
pkg_search_module(MODPLUG libmodplug)

# add modplug defines
CHECK_INCLUDE_FILE(stdint.h STDINT_FOUND)
IF(STDINT_FOUND)
ADD_DEFINITIONS(-DHAVE_STDINT_H)
ENDIF(STDINT_FOUND)

CHECK_INCLUDE_FILE(inttypes.h INTTYPES_FOUND)
IF(INTTYPES_FOUND)
ADD_DEFINITIONS(-DHAVE_INTTYPES_H)
ENDIF(INTTYPES_FOUND)


include_directories(${MODPLUG_INCLUDE_DIRS})
link_directories(${MODPLUG_LIBRARY_DIRS})
ADD_DEFINITIONS(${MODPLUG_CFLAGS})

SET(libmodplug_SRCS
  decoder_modplug.cpp
  decodermodplugfactory.cpp
  modplugmetadatamodel.cpp
  settingsdialog.cpp
  archivereader.cpp
)

SET(libmodplug_HDRS
  decoder_modplug.h
)

SET(libmodplug_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libmodplug_RCC_SRCS ${libmodplug_RCCS})

# user interface

SET(libmodplug_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libmodplug_UIS_H ${libmodplug_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(MODPLUG_FOUND)
ADD_LIBRARY(modplug MODULE ${libmodplug_SRCS} ${libmodplug_UIS_H}
  ${libmodplug_RCC_SRCS} ${libmodplug_HDRS})
add_dependencies(modplug qmmp)
target_link_libraries(modplug Qt5::Widgets -lqmmp ${MODPLUG_LDFLAGS})
install(TARGETS modplug DESTINATION ${LIB_DIR}/qmmp/Input)
ENDIF(MODPLUG_FOUND)


