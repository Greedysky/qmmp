project(libplsplaylistformat)


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

SET(libplsplaylistformat_SRCS
    plsplaylistformat.cpp
)

QT4_ADD_RESOURCES(libplsplaylistformat_RCC_SRCS ${libplsplaylistformat_RCCS})

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(plsplaylistformat MODULE ${libplsplaylistformat_SRCS})
add_dependencies(plsplaylistformat qmmpui)
target_link_libraries(plsplaylistformat ${QT_LIBRARIES} qmmpui)
install(TARGETS plsplaylistformat DESTINATION ${LIB_DIR}/qmmp/PlayListFormats)
