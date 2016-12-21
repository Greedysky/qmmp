project(skinned)


if(COMMAND cmake_policy)
   cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)


# qt plugin
ADD_DEFINITIONS( -Wall )
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)
ADD_DEFINITIONS(-DQT_THREAD)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(QT_INCLUDES
  ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../
)


# libqmmpui & libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# X11
include_directories(${X11_INCLUDE_DIRS})
link_directories(${X11_LIBRARY_DIRS})
ADD_DEFINITIONS(${X11_CFLAGS})

SET(skinned_SRCS
    fft.c
    balancebar.cpp
    button.cpp
    skinnedsettings.cpp
    display.cpp
    dock.cpp
    eqgraph.cpp
    eqpreset.cpp
    eqslider.cpp
    eqtitlebar.cpp
    eqwidget.cpp
    keyboardmanager.cpp
    listwidget.cpp
    listwidgetdrawer.cpp
    mainvisual.cpp
    mainwindow.cpp
    monostereo.cpp
    number.cpp
    pixmapwidget.cpp
    playlistcontrol.cpp
    playlist.cpp
    playlistslider.cpp
    playlisttitlebar.cpp
    playstatus.cpp
    positionbar.cpp
    preseteditor.cpp
    shadedbar.cpp
    shadedvisual.cpp
    skin.cpp
    skinreader.cpp
    symboldisplay.cpp
    textscroller.cpp
    timeindicator.cpp
    titlebarcontrol.cpp
    titlebar.cpp
    togglebutton.cpp
    visualmenu.cpp
    volumebar.cpp
    cursorimage.cpp
    playlistbrowser.cpp
    playlistselector.cpp
    popupwidget.cpp
    popupsettings.cpp
    windowsystem.cpp
    actionmanager.cpp
    shortcutitem.cpp
    shortcutdialog.cpp
    skinnedfactory.cpp
    hotkeyeditor.cpp
    playlistheader.cpp
    horizontalslider.cpp
)

SET(skinned_HDRS
    eqpreset.h
    fft.h
    inlines.h
    keyboardmanager.h
    listwidgetdrawer.h
    cursorimage.h
    windowsystem.h
    shortcutitem.h
)

SET(skinned_RCCS glare/glare.qrc resources/resources.qrc translations/translations.qrc)

QT4_ADD_RESOURCES(skinned_RCC_SRCS ${skinned_RCCS})

# user interface

SET(skinned_UIS
    forms/skinnedsettings.ui
    forms/preseteditor.ui
    forms/playlistbrowser.ui
    forms/popupsettings.ui
    forms/shortcutdialog.ui
    forms/hotkeyeditor.ui
)

QT4_WRAP_UI(skinned_UIS_H ${skinned_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(X11_FOUND)
ADD_LIBRARY(skinned MODULE ${skinned_SRCS} ${skinned_UIS_H} ${skinned_RCC_SRCS} ${skinned_HDRS})
target_link_libraries(skinned ${QT_LIBRARIES} libqmmp qmmpui ${X11_LDFLAGS})
add_dependencies(skinned qmmpui libqmmp)
install(TARGETS skinned DESTINATION ${LIB_DIR}/qmmp/Ui)
ENDIF(X11_FOUND)
