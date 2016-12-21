project(libhal)


SET (QT_USE_QTDBUS TRUE)

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

# libqmmpui & libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libhal_SRCS
    halfactory.cpp
    halmanager.cpp
    halplugin.cpp
    haldevice.cpp
    settingsdialog.cpp
)

SET(libhal_RCCS translations/translations.qrc)

QT4_ADD_RESOURCES(libhal_RCC_SRCS ${libhal_RCCS})

# user interface

SET(libhal_UIS
    settingsdialog.ui
)

QT4_WRAP_UI(libhal_UIS_H ${libhal_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(hal MODULE ${libhal_SRCS} ${libhal_UIS_H} ${libhal_RCC_SRCS})
add_dependencies(hal qmmpui)
target_link_libraries(hal ${QT_LIBRARIES} qmmpui libqmmp)
install(TARGETS hal DESTINATION ${LIB_DIR}/qmmp/General)
