project(qsui)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)


# libqmmpui & libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)


SET(libqsui_SRCS
    actionmanager.cpp
    listwidget.cpp
    mainwindow.cpp
    popupsettings.cpp
    popupwidget.cpp
    positionslider.cpp
    shortcutdialog.cpp
    shortcutitem.cpp
    qsuifactory.cpp
    qsuisettings.cpp
    visualmenu.cpp
    equalizer.cpp
    logo.cpp
    aboutqsuidialog.cpp
    keyboardmanager.cpp
    qsuianalyzer.cpp
    fft.c
    colorwidget.cpp
    qsuitabwidget.cpp
    qsuitabbar.cpp
    eqpreset.cpp
    filesystembrowser.cpp
    elidinglabel.cpp
    coverwidget.cpp
    playlistbrowser.cpp
    toolbareditor.cpp
    listwidgetdrawer.cpp
    playlistheader.cpp
    hotkeyeditor.cpp
    volumeslider.cpp
)

SET(libqsui_HDRS
    shortcutitem.h
    inlines.h
    fft.h
    eqpreset.h
    listwidgetdrawer.h
)

SET(libqsui_RCCS translations/translations.qrc txt/qsui_txt.qrc resources/qsui_resources.qrc)

QT5_ADD_RESOURCES(libqsui_RCC_SRCS ${libqsui_RCCS})

# user interface

SET(libqsui_UIS
    forms/mainwindow.ui
    forms/popupsettings.ui
    forms/shortcutdialog.ui
    forms/qsuisettings.ui
    forms/aboutqsuidialog.ui
    forms/toolbareditor.ui
    forms/hotkeyeditor.ui
)

QT5_WRAP_UI(libqsui_UIS_H ${libqsui_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(qsui MODULE ${libqsui_SRCS} ${libqsui_UIS_H} ${libqsui_RCC_SRCS} ${libqsui_HDRS})
add_dependencies(qsui qmmpui libqmmp)
target_link_libraries(qsui Qt5::Widgets -lqmmpui -lqmmp)
install(TARGETS qsui DESTINATION ${LIB_DIR}/qmmp/Ui)
