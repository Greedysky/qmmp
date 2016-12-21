project(libmplayer)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)


SET(libmplayer_SRCS
    mplayerenginefactory.cpp
    mplayerengine.cpp
    mplayermetadatamodel.cpp
    settingsdialog.cpp
)

SET(libmplayer_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libmplayer_RCC_SRCS ${libmplayer_RCCS})

# user interface


SET(libmplayer_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libmplayer_UIS_H ${libmplayer_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(mplayer MODULE ${libmplayer_SRCS} ${libmplayer_RCC_SRCS} ${libmplayer_UIS_H})
add_dependencies(mplayer qmmp)
target_link_libraries(mplayer Qt5::Widgets -lqmmp)
install(TARGETS mplayer DESTINATION ${LIB_DIR}/qmmp/Engines)

