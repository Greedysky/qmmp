include(../../plugins.pri)

CONFIG += warn_on \
plugin  \
 lib

TARGET =$$PLUGINS_PREFIX/PlayListFormats/plsplaylistformat


TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib

INCLUDEPATH += ../../../../src

SOURCES += plsplaylistformat.cpp

HEADERS += plsplaylistformat.h

unix {
    isEmpty(LIB_DIR){
        LIB_DIR = /lib
    }
    target.path = $$LIB_DIR/qmmp/PlayListFormats
    INSTALLS += target

    QMAKE_CLEAN =$$PLUGINS_PREFIX/PlayListFormats/plsplaylistformat.so
    LIBS += -lqmmpui
}

win32 {
    LIBS += -lqmmpui0
    QMAKE_LIBDIR += ../../../../bin
}

