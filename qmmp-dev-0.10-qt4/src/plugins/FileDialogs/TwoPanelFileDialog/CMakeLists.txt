project(libtwopanelfiledialog)


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

# libqmmpui
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libtwopanelfiledialog_SRCS
    twopanelfiledialogimpl.cpp
    twopanelfiledialog.cpp
)

SET(libtwopanelfiledialog_RCCS
    translations/translations.qrc)

QT4_ADD_RESOURCES(libtwopanelfiledialog_RCC_SRCS ${libtwopanelfiledialog_RCCS})

# user interface


SET(libtwopanelfiledialog_UIS
    twopanelfiledialog.ui
)

QT4_WRAP_UI(libtwopanelfiledialog_UIS_H ${libtwopanelfiledialog_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(twopanelfiledialog MODULE ${libtwopanelfiledialog_SRCS} ${libtwopanelfiledialog_UIS_H} ${libtwopanelfiledialog_RCC_SRCS})
add_dependencies(twopanelfiledialog qmmpui)
target_link_libraries(twopanelfiledialog ${QT_LIBRARIES} qmmpui libqmmp)
install(TARGETS twopanelfiledialog DESTINATION ${LIB_DIR}/qmmp/FileDialogs)
