project(libincdecvolumeoption)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmpui
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libincdecvolumeoption_SRCS
    incdecvolumeoption.cpp
)

SET(libincdecvolumeoption_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libincdecvolumeoption_RCC_SRCS ${libincdecvolumeoption_RCCS})


# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(incdecvolumeoption MODULE ${libincdecvolumeoption_SRCS}
           ${libincdecvolumeoption_RCC_SRCS})
add_dependencies(incdecvolumeoption qmmpui)
target_link_libraries(incdecvolumeoption Qt5::Widgets -lqmmpui -lqmmp)
install(TARGETS incdecvolumeoption DESTINATION ${LIB_DIR}/qmmp/CommandLineOptions)
