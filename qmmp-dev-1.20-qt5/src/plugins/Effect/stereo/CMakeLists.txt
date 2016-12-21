project(libstereo)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libstereo_SRCS
    stereoplugin.cpp
    settingsdialog.cpp
    effectstereofactory.cpp
)

SET(libstereo_HDRS
    stereoplugin.h
)

SET(libstereo_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libstereo_RCC_SRCS ${libstereo_RCCS})

# user interface

SET(libstereo_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libstereo_UIS_H ${libstereo_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})


ADD_LIBRARY(stereo MODULE ${libstereo_SRCS} ${libstereo_UIS_H} ${libstereo_RCC_SRCS} ${libstereo_HDRS})
add_dependencies(stereo qmmp)
target_link_libraries(stereo Qt5::Widgets -lqmmp)
install(TARGETS stereo DESTINATION ${LIB_DIR}/qmmp/Effect)
