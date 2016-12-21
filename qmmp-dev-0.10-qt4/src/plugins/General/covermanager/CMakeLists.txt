project(libcovermanager)



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
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# libqmmpui
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
#libqmmp
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libcovermanager_SRCS
    coverwidget.cpp
    covermanager.cpp
    covermanagerfactory.cpp
)

SET(libcovermanager_RCCS
    translations/translations.qrc
    images/cm_images.qrc
)

QT4_ADD_RESOURCES(libcovermanager_RCC_SRCS ${libcovermanager_RCCS})

# user interface


SET(libcovermanager_UIS
)

QT4_WRAP_UI(libcovermanager_UIS_H ${libcovermanager_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(covermanager MODULE ${libcovermanager_SRCS} ${libcovermanager_UIS_H}
  ${libcovermanager_RCC_SRCS})
add_dependencies(covermanager qmmpui)
target_link_libraries(covermanager ${QT_LIBRARIES} qmmpui libqmmp)
install(TARGETS covermanager DESTINATION ${LIB_DIR}/qmmp/General)
