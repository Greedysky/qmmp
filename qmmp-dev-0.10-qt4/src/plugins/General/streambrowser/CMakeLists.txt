project(libstreambrowser)


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

SET(libstreambrowser_SRCS
    streambrowser.cpp
    streambrowserfactory.cpp
    streamwindow.cpp
)

SET(libstreambrowser_RCCS 
    translations/translations.qrc)

QT4_ADD_RESOURCES(libstreambrowser_RCC_SRCS ${libstreambrowser_RCCS})

# user interface

SET(libstreambrowser_UIS
    streamwindow.ui
)

QT4_WRAP_UI(libstreambrowser_UIS_H ${libstreambrowser_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(streambrowser MODULE ${libstreambrowser_SRCS} ${libstreambrowser_UIS_H} ${libstreambrowser_RCC_SRCS})
add_dependencies(streambrowser qmmpui)
target_link_libraries(streambrowser ${QT_LIBRARIES} qmmpui libqmmp)
install(TARGETS streambrowser DESTINATION ${LIB_DIR}/qmmp/General)
