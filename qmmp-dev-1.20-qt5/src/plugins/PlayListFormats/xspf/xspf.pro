include(../../plugins.pri)

CONFIG += warn_on \
plugin  \
 lib

TARGET =$$PLUGINS_PREFIX/PlayListFormats/xspfplaylistformat


TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib

INCLUDEPATH += ../../../../src

SOURCES += xspfplaylistformat.cpp
HEADERS += xspfplaylistformat.h

unix {
    isEmpty(LIB_DIR){
        LIB_DIR = /lib
    }
    target.path = $$LIB_DIR/qmmp/PlayListFormats
    INSTALLS += target

    QMAKE_CLEAN =$$PLUGINS_PREFIX/PlayListFormats/libxsplaylistformat.so
    LIBS += -lqmmpui -lqmmp
}

win32 {
    LIBS += -lqmmpui0 -lqmmp0
    QMAKE_LIBDIR += ../../../../bin
}

