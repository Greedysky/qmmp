project(libcdaudio)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
    cmake_policy(SET CMP0005 NEW)
endif(COMMAND cmake_policy) 

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# libcdaudio
pkg_check_modules(CDIO libcdio libcdio_cdda libcdio_paranoia libcddb)

include_directories(${CDIO_INCLUDE_DIRS})
link_directories(${CDIO_LIBRARY_DIRS})
ADD_DEFINITIONS(${CDIO_CFLAGS})

SET(libcdaudio_SRCS
  decoder_cdaudio.cpp
  decodercdaudiofactory.cpp
  settingsdialog.cpp
)

SET(libcdaudio_HDRS
  decoder_cdaudio.h
)

SET(libcdaudio_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libcdaudio_RCC_SRCS ${libcdaudio_RCCS})

# user interface

SET(libcdaudio_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libcdaudio_UIS_H ${libcdaudio_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(CDIO_FOUND)
ADD_LIBRARY(cdaudio MODULE ${libcdaudio_SRCS} ${libcdaudio_UIS_H} ${libcdaudio_RCC_SRCS}
${libcdaudio_HDRS})
add_dependencies(cdaudio qmmp)
target_link_libraries(cdaudio Qt5::Widgets -lqmmp ${CDIO_LDFLAGS})
install(TARGETS cdaudio DESTINATION ${LIB_DIR}/qmmp/Input)
ENDIF(CDIO_FOUND)
