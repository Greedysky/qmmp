project(libtrackchange)


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

SET(libtrackchange_SRCS
    trackchange.cpp
    trackchangefactory.cpp
    settingsdialog.cpp
)

SET(libtrackchange_RCCS 
    translations/translations.qrc)

QT4_ADD_RESOURCES(libtrackchange_RCC_SRCS ${libtrackchange_RCCS})

# user interface
SET(libtrackchange_UIS
    settingsdialog.ui
)

QT4_WRAP_UI(libtrackchange_UIS_H ${libtrackchange_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(trackchange MODULE ${libtrackchange_SRCS} ${libtrackchange_UIS_H} ${libtrackchange_RCC_SRCS})
add_dependencies(trackchange qmmpui)
target_link_libraries(trackchange ${QT_LIBRARIES} qmmpui libqmmp)
install(TARGETS trackchange DESTINATION ${LIB_DIR}/qmmp/General)
