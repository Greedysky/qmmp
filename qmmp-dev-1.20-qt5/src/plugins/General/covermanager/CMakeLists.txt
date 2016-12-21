project(libcovermanager)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# libqmmpui
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
#libqmmp
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libcovermanager_SRCS
    coverwidget.cpp
    covermanager.cpp
    covermanagerfactory.cpp
)

SET(libcovermanager_RCCS
    translations/translations.qrc
    images/cm_images.qrc
)

QT5_ADD_RESOURCES(libcovermanager_RCC_SRCS ${libcovermanager_RCCS})

# user interface


SET(libcovermanager_UIS
)

QT5_WRAP_UI(libcovermanager_UIS_H ${libcovermanager_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(covermanager MODULE ${libcovermanager_SRCS} ${libcovermanager_UIS_H}
  ${libcovermanager_RCC_SRCS})
add_dependencies(covermanager qmmpui)
target_link_libraries(covermanager Qt5::Widgets -lqmmpui -lqmmp)
install(TARGETS covermanager DESTINATION ${LIB_DIR}/qmmp/General)
