project(libudisks2)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmpui & libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libudisks2_SRCS
    udisks2factory.cpp
    udisks2manager.cpp
    udisks2plugin.cpp
    udisks2device.cpp
    settingsdialog.cpp
)

SET(libudisks2_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libudisks2_RCC_SRCS ${libudisks2_RCCS})

# user interface

SET(libudisks2_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libudisks2_UIS_H ${libudisks2_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(udisks2 MODULE ${libudisks2_SRCS} ${libudisks2_UIS_H} ${libudisks2_RCC_SRCS})
add_dependencies(udisks2 qmmpui)
target_link_libraries(udisks2 Qt5::Widgets Qt5::DBus -lqmmpui -lqmmp)
install(TARGETS udisks2 DESTINATION ${LIB_DIR}/qmmp/General)
