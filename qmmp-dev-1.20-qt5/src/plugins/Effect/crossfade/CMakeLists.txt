project(libcrossfade)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libcrossfade_SRCS
    crossfadeplugin.cpp
    settingsdialog.cpp
    effectcrossfadefactory.cpp
)

SET(libcrossfade_HDRS
    crossfadeplugin.h
)

SET(libcrossfade_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libcrossfade_RCC_SRCS ${libcrossfade_RCCS})

# user interface

SET(libcrossfade_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libcrossfade_UIS_H ${libcrossfade_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})


ADD_LIBRARY(crossfade MODULE ${libcrossfade_SRCS} ${libcrossfade_UIS_H} ${libcrossfade_RCC_SRCS}
${libcrossfade_HDRS})
add_dependencies(crossfade qmmp)
target_link_libraries(crossfade Qt5::Widgets -lqmmp)
install(TARGETS crossfade DESTINATION ${LIB_DIR}/qmmp/Effect)
