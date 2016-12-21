project(libanalyzer)


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

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(QT_INCLUDES
  ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../
)

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libanalyzer_SRCS
    analyzer.cpp
    colorwidget.cpp
    settingsdialog.cpp
    visualanalyzerfactory.cpp
    fft.c
)

SET(libanalyzer_HDRS
   inlines.h
   fft.h
)

SET(libanalyzer_RCCS translations/translations.qrc)

QT4_ADD_RESOURCES(libanalyzer_RCC_SRCS ${libanalyzer_RCCS})

# user interface


SET(libanalyzer_UIS
    settingsdialog.ui
)

QT4_WRAP_UI(libanalyzer_UIS_H ${libanalyzer_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(analyzer MODULE ${libanalyzer_SRCS} ${libanalyzer_UIS_H}
  ${libanalyzer_RCC_SRCS} ${libanalyzer_HDRS})
add_dependencies(analyzer qmmp)
target_link_libraries(analyzer ${QT_LIBRARIES} libqmmp)
install(TARGETS analyzer DESTINATION ${LIB_DIR}/qmmp/Visual)
