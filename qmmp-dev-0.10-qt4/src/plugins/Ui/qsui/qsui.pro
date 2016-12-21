include(../../plugins.pri)

TARGET = $$PLUGINS_PREFIX/Ui/qsui

CONFIG += warn_on \
    plugin

TEMPLATE = lib

SOURCES += \
    mainwindow.cpp \
    listwidget.cpp \
    visualmenu.cpp \
    positionslider.cpp \
    actionmanager.cpp \
    shortcutdialog.cpp \
    shortcutitem.cpp \
    popupsettings.cpp \
    popupwidget.cpp \
    equalizer.cpp \
    logo.cpp \
    keyboardmanager.cpp \
    aboutqsuidialog.cpp \
    qsuifactory.cpp \
    qsuisettings.cpp \
    qsuianalyzer.cpp \
    fft.c \
    colorwidget.cpp \
    qsuitabwidget.cpp \
    qsuitabbar.cpp \
    eqpreset.cpp \
    filesystembrowser.cpp \
    elidinglabel.cpp \
    coverwidget.cpp \
    playlistbrowser.cpp \
    toolbareditor.cpp \
    listwidgetdrawer.cpp \
    playlistheader.cpp \
    hotkeyeditor.cpp \
    volumeslider.cpp
HEADERS += mainwindow.h \
    listwidget.h \
    visualmenu.h \
    positionslider.h \
    actionmanager.h \
    shortcutdialog.h \
    shortcutitem.h \
    popupsettings.h \
    popupwidget.h \
    equalizer.h \
    logo.h \
    keyboardmanager.h \
    aboutqsuidialog.h \
    qsuifactory.h \
    qsuisettings.h \
    qsuianalyzer.h \
    fft.h \
    inlines.h \
    colorwidget.h \
    qsuitabwidget.h \
    qsuitabbar.h \
    eqpreset.h \
    filesystembrowser.h \
    elidinglabel.h \
    coverwidget.h \
    playlistbrowser.h \
    toolbareditor.h \
    listwidgetdrawer.h \
    playlistheader.h \
    hotkeyeditor.h \
    volumeslider.h

FORMS += forms/mainwindow.ui \
    forms/shortcutdialog.ui \
    forms/popupsettings.ui \
    forms/aboutqsuidialog.ui \
    forms/qsuisettings.ui \
    forms/toolbareditor.ui \
    forms/hotkeyeditor.ui

RESOURCES += translations/translations.qrc resources/qsui_resources.qrc txt/qsui_txt.qrc

TEMPLATE = lib

CONFIG += warn_on \
          plugin

TARGET = $$PLUGINS_PREFIX/Ui/qsui

unix{
  QMAKE_LIBDIR += ../../../../lib
  LIBS += -lqmmpui -lqmmp
  isEmpty(LIB_DIR){
    LIB_DIR = /lib
  }  

  target.path = $$LIB_DIR/qmmp/Ui
  INSTALLS += target
}

win32{
   QMAKE_LIBDIR += ../../../../bin
   LIBS += -lqmmpui0 -lqmmp0
   INCLUDEPATH += ../../ ./
}

INCLUDEPATH += ../../../
