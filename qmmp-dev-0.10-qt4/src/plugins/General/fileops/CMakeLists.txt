project(libfileops)


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

SET(libfileops_SRCS
    fileops.cpp
    fileopsfactory.cpp
    settingsdialog.cpp
    hotkeydialog.cpp
)

SET(libfileops_RCCS 
    translations/translations.qrc)

QT4_ADD_RESOURCES(libfileops_RCC_SRCS ${libfileops_RCCS})

# user interface

SET(libfileops_UIS
    settingsdialog.ui
    hotkeydialog.ui
)

QT4_WRAP_UI(libfileops_UIS_H ${libfileops_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(fileops MODULE ${libfileops_SRCS} ${libfileops_UIS_H} ${libfileops_RCC_SRCS})
add_dependencies(fileops qmmpui)
target_link_libraries(fileops ${QT_LIBRARIES} qmmpui libqmmp)
install(TARGETS fileops DESTINATION ${LIB_DIR}/qmmp/General)
