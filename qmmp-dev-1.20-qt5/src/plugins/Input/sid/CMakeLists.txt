project(libsid)

INCLUDE(CheckIncludeFile)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# libsidplayfp
pkg_search_module(SIDPLAYFP libsidplayfp>=1.0.3)

include_directories(${SIDPLAYFP_INCLUDE_DIRS})
link_directories(${SIDPLAYFP_LIBRARY_DIRS})
ADD_DEFINITIONS(${SIDPLAYFP_CFLAGS})

SET(libsid_SRCS
  decoder_sid.cpp
  decodersidfactory.cpp
  settingsdialog.cpp
  sidhelper.cpp
)

SET(libsid_HDRS
  decoder_sid.h
  sidhelper.h
)

SET(libsid_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libsid_RCC_SRCS ${libsid_RCCS})

# user interface

SET(libsid_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libsid_UIS_H ${libsid_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(SIDPLAYFP_FOUND)
ADD_LIBRARY(sid MODULE ${libsid_SRCS} ${libsid_UIS_H}
  ${libsid_RCC_SRCS} ${libsid_HDRS})
add_dependencies(sid qmmp)
target_link_libraries(sid Qt5::Widgets -lqmmp ${SIDPLAYFP_LDFLAGS})
install(TARGETS sid DESTINATION ${LIB_DIR}/qmmp/Input)
ENDIF(SIDPLAYFP_FOUND)
