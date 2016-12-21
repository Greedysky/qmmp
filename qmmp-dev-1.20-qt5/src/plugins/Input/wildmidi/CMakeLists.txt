project(libwildmidi)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
    cmake_policy(SET CMP0005 NEW)
endif(COMMAND cmake_policy)

INCLUDE(CheckIncludeFile)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)


SET(libwildmidi_SRCS
  decoder_wildmidi.cpp
  decoderwildmidifactory.cpp
  settingsdialog.cpp
  wildmidihelper.cpp
)

SET(libwildmidi_HDRS
  decoder_wildmidi.h
)

# libwildmidi
CHECK_INCLUDE_FILE(wildmidi_lib.h WILDMIDI_HEADER_FOUND)
FIND_LIBRARY(WILDMIDI_LIB NAME WildMidi PATHS /usr/lib /usr/lib32  /usr/local/lib /usr/local/lib32)

IF(WILDMIDI_HEADER_FOUND AND (NOT WILDMIDI_LIB MATCHES "^.*-NOTFOUND"))
SET(WILDMIDI_FOUND TRUE CACHE INTERNAL "wildmidi")
MESSAGE(STATUS "Found WildMidi: ${WILDMIDI_LIB}")
ELSE(WILDMIDI_HEADER_FOUND AND (NOT WILDMIDI_LIB MATCHES "^.*-NOTFOUND"))
MESSAGE(STATUS "Could not find WildMidi library")
ENDIF(WILDMIDI_HEADER_FOUND AND (NOT WILDMIDI_LIB MATCHES "^.*-NOTFOUND"))

SET(libwildmidi_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libwildmidi_RCC_SRCS ${libwildmidi_RCCS})

# user interface
SET(libwildmidi_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libwildmidi_UIS_H ${libwildmidi_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(WILDMIDI_FOUND)
ADD_LIBRARY(wildmidi MODULE ${libwildmidi_SRCS} ${libwildmidi_UIS_H} ${libwildmidi_RCC_SRCS}
${libwildmidi_HDRS})
add_dependencies(wildmidi qmmp)
target_link_libraries(wildmidi Qt5::Widgets -lqmmp ${WILDMIDI_LIB})
install(TARGETS wildmidi DESTINATION ${LIB_DIR}/qmmp/Input)
ENDIF(WILDMIDI_FOUND)
