include(../../plugins.pri)

CONFIG += warn_on \
plugin  \
 lib  \
 link_pkgconfig


TARGET = $$PLUGINS_PREFIX/General/hotkey


TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib

RESOURCES = translations/translations.qrc

target.path = $$LIB_DIR/qmmp/General

HEADERS += hotkeyfactory.h \
           hotkeymanager.h \
           settingsdialog.h \
           hotkeydialog.h

SOURCES += hotkeyfactory.cpp \
           settingsdialog.cpp \
           hotkeydialog.cpp \
           hotkeymanager_x11.cpp \
           hotkeymanager_win.cpp

FORMS += settingsdialog.ui \
         hotkeydialog.ui


INCLUDEPATH += ../../../../src

unix {
    isEmpty(LIB_DIR){
        LIB_DIR = /lib
    }
    target.path = $$LIB_DIR/qmmp/General
    INSTALLS += target
    QMAKE_LIBDIR += ../../../../lib
    QMAKE_CLEAN = $$PLUGINS_PREFIX/General/libhotkey.so
    PKGCONFIG += x11
    DEFINES += HAVE_XKBLIB_H
    LIBS += -lqmmpui -lqmmp
    QT += x11extras
}

win32 {
    QMAKE_LIBDIR += ../../../../bin
    LIBS += -lqmmpui0 -lqmmp0
}

