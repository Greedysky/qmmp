project(libnull)


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

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libnull_SRCS
  outputnull.cpp
  outputnullfactory.cpp
)

SET(libnull_HDRS
  outputnull.h
)

SET(libnull_RCCS translations/translations.qrc)

QT4_ADD_RESOURCES(libnull_RCC_SRCS ${libnull_RCCS})

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})


ADD_LIBRARY(null MODULE ${libnull_SRCS} ${libnull_UIS_H} ${libnull_RCC_SRCS} ${libnull_HDRS})
add_dependencies(null qmmp)
target_link_libraries(null ${QT_LIBRARIES} libqmmp)
install(TARGETS null DESTINATION ${LIB_DIR}/qmmp/Output)
