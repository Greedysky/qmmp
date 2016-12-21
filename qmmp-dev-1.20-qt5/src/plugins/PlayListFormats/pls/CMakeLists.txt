project(libplsplaylistformat)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmpui
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)

SET(libplsplaylistformat_SRCS
    plsplaylistformat.cpp
)

QT5_ADD_RESOURCES(libplsplaylistformat_RCC_SRCS ${libplsplaylistformat_RCCS})

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(plsplaylistformat MODULE ${libplsplaylistformat_SRCS})
add_dependencies(plsplaylistformat qmmpui)
target_link_libraries(plsplaylistformat Qt5::Widgets -lqmmpui)
install(TARGETS plsplaylistformat DESTINATION ${LIB_DIR}/qmmp/PlayListFormats)
