project(libcopypaste)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmpui & libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libcopypaste_SRCS
    copypaste.cpp
    copypastefactory.cpp
)

SET(libcopypaste_RCCS 
    translations/translations.qrc)

QT5_ADD_RESOURCES(libcopypaste_RCC_SRCS ${libcopypaste_RCCS})

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(copypaste MODULE ${libcopypaste_SRCS} ${libcopypaste_UIS_H} ${libcopypaste_RCC_SRCS})
add_dependencies(copypaste qmmpui)
target_link_libraries(copypaste Qt5::Widgets -lqmmpui -lqmmp)
install(TARGETS copypaste DESTINATION ${LIB_DIR}/qmmp/General)
