project(liblyrics)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmpui & libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(liblyrics_SRCS
    lyrics.cpp
    lyricsfactory.cpp
    lyricswindow.cpp
)

SET(liblyrics_RCCS 
    translations/translations.qrc)

QT5_ADD_RESOURCES(liblyrics_RCC_SRCS ${liblyrics_RCCS})

# user interface

SET(liblyrics_UIS
    lyricswindow.ui
)

QT5_WRAP_UI(liblyrics_UIS_H ${liblyrics_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(lyrics MODULE ${liblyrics_SRCS} ${liblyrics_UIS_H} ${liblyrics_RCC_SRCS})
add_dependencies(lyrics qmmpui)
target_link_libraries(lyrics Qt5::Widgets Qt5::Network -lqmmpui -lqmmp)
install(TARGETS lyrics DESTINATION ${LIB_DIR}/qmmp/General)
