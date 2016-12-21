project(libmpris)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmpui & libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libmpris_SRCS
    mpris.cpp
    mprisfactory.cpp
    mpris1/rootobject.cpp
    mpris1/playerobject.cpp
    mpris1/tracklistobject.cpp
    mpris2/root2object.cpp
    mpris2/player2object.cpp
)

SET(libmpris_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libmpris_RCC_SRCS ${libmpris_RCCS})

# user interface


# QT5_WRAP_UI(libmpris_UIS_H ${libmpris_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(mpris MODULE ${libmpris_SRCS} ${libmpris_RCC_SRCS})
add_dependencies(mpris qmmpui)
target_link_libraries(mpris Qt5::Widgets Qt5::DBus -lqmmpui -lqmmp)
install(TARGETS mpris DESTINATION ${LIB_DIR}/qmmp/General)
