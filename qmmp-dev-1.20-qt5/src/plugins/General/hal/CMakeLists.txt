project(libhal)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmpui & libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libhal_SRCS
    halfactory.cpp
    halmanager.cpp
    halplugin.cpp
    haldevice.cpp
    settingsdialog.cpp
)

SET(libhal_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libhal_RCC_SRCS ${libhal_RCCS})

# user interface

SET(libhal_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libhal_UIS_H ${libhal_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(hal MODULE ${libhal_SRCS} ${libhal_UIS_H} ${libhal_RCC_SRCS})
add_dependencies(hal qmmpui)
target_link_libraries(hal Qt5::Widgets Qt5::DBus -lqmmpui -lqmmp)
install(TARGETS hal DESTINATION ${LIB_DIR}/qmmp/General)
