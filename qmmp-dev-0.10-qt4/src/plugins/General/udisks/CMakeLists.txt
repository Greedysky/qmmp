project(libudisks)


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

SET(libudisks_SRCS
    udisksfactory.cpp
    udisksmanager.cpp
    udisksplugin.cpp
    udisksdevice.cpp
    settingsdialog.cpp
)

SET(libudisks_RCCS translations/translations.qrc)

QT4_ADD_RESOURCES(libudisks_RCC_SRCS ${libudisks_RCCS})

# user interface

SET(libudisks_UIS
    settingsdialog.ui
)

QT4_WRAP_UI(libudisks_UIS_H ${libudisks_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(udisks MODULE ${libudisks_SRCS} ${libudisks_UIS_H} ${libudisks_RCC_SRCS})
add_dependencies(udisks qmmpui)
target_link_libraries(udisks ${QT_LIBRARIES} qmmpui libqmmp)
install(TARGETS udisks DESTINATION ${LIB_DIR}/qmmp/General)
