project(libqmmpfiledialog)


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

SET(libqmmpfiledialog_SRCS
    qmmpfiledialogimpl.cpp
    qmmpfiledialog.cpp
)

SET(libqmmpfiledialog_RCCS
    translations/translations.qrc)

QT4_ADD_RESOURCES(libqmmpfiledialog_RCC_SRCS ${libqmmpfiledialog_RCCS})

# user interface


SET(libqmmpfiledialog_UIS
    qmmpfiledialog.ui
)

QT4_WRAP_UI(libqmmpfiledialog_UIS_H ${libqmmpfiledialog_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(qmmpfiledialog MODULE ${libqmmpfiledialog_SRCS} ${libqmmpfiledialog_UIS_H} ${libqmmpfiledialog_RCC_SRCS})
add_dependencies(qmmpfiledialog qmmpui)
target_link_libraries(qmmpfiledialog ${QT_LIBRARIES} qmmpui libqmmp)
install(TARGETS qmmpfiledialog DESTINATION ${LIB_DIR}/qmmp/FileDialogs)
