project(seekoption)


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

SET(seekoption_SRCS
    seekoption.cpp
)

SET(seekoption_RCCS translations/translations.qrc)

QT4_ADD_RESOURCES(seekoption_RCC_SRCS ${seekoption_RCCS})


# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(seekoption MODULE ${seekoption_SRCS} ${seekoption_RCC_SRCS})
add_dependencies(seekoption qmmpui)
target_link_libraries(seekoption ${QT_LIBRARIES} qmmpui libqmmp)
install(TARGETS seekoption DESTINATION ${LIB_DIR}/qmmp/CommandLineOptions)
