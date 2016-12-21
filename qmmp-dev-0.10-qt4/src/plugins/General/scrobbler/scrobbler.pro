include(../../plugins.pri)
CONFIG += warn_on \
    plugin
TEMPLATE = lib
unix:QMAKE_LIBDIR += ../../../../lib
win32:QMAKE_LIBDIR += ../../../../bin
TARGET = $$PLUGINS_PREFIX/General/scrobbler
unix:QMAKE_CLEAN = $$PLUGINS_PREFIX/General/libscrobbler.so

RESOURCES = translations/translations.qrc
unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib
    target.path = $$LIB_DIR/qmmp/General
    INSTALLS += target
}
HEADERS += scrobblerfactory.h \
    settingsdialog.h \
    scrobblerhandler.h \
    scrobblercache.h \
    scrobbler.h \
    defines.h
win32:HEADERS += ../../../../src/qmmpui/general.h
SOURCES += scrobblerfactory.cpp \
    settingsdialog.cpp \
    scrobblerhandler.cpp \
    scrobblercache.cpp \
    scrobbler.cpp
QT += network
INCLUDEPATH += ../../../
unix:LIBS += -lqmmpui \
    -lqmmp
win32:LIBS += -lqmmpui0 \
    -lqmmp0
FORMS += settingsdialog.ui












