include(../../plugins.pri)

INCLUDEPATH += ../../../../src
CONFIG += warn_on \
plugin

TARGET =$$PLUGINS_PREFIX/General/copypaste
unix : QMAKE_CLEAN = $$PLUGINS_PREFIX/General/libcopypaste.so


TEMPLATE = lib
unix : QMAKE_LIBDIR += ../../../../lib
unix : LIBS += -lqmmpui -lqmmp

win32 : QMAKE_LIBDIR += ../../../../bin
win32 : LIBS += -lqmmpui0 -lqmmp0

RESOURCES = translations/translations.qrc
unix {
    isEmpty(LIB_DIR){
        LIB_DIR = /lib
    }
    target.path = $$LIB_DIR/qmmp/General
    INSTALLS += target
}
HEADERS += copypastefactory.h \
           copypaste.h

win32 : HEADERS += ../../../../src/qmmpui/general.h
SOURCES += copypastefactory.cpp \
           copypaste.cpp

