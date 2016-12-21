project(liblyrics)


SET(QT_USE_QTNETWORK TRUE)
INCLUDE(FindQt4)

include(${QT_USE_FILE})

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


# qt plugin
ADD_DEFINITIONS( -Wall )
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)
ADD_DEFINITIONS(-DQT_THREAD)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(QT_INCLUDES
  ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../
)

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

QT4_ADD_RESOURCES(liblyrics_RCC_SRCS ${liblyrics_RCCS})

# user interface

SET(liblyrics_UIS
    lyricswindow.ui
)

QT4_WRAP_UI(liblyrics_UIS_H ${liblyrics_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(lyrics MODULE ${liblyrics_SRCS} ${liblyrics_UIS_H} ${liblyrics_RCC_SRCS})
add_dependencies(lyrics qmmpui)
target_link_libraries(lyrics ${QT_LIBRARIES} qmmpui libqmmp)
install(TARGETS lyrics DESTINATION ${LIB_DIR}/qmmp/General)
