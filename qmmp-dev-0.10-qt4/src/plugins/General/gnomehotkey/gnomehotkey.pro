include(../../plugins.pri)

CONFIG += warn_on \
plugin  \
 lib \
 qdbus

TARGET = $$PLUGINS_PREFIX/General/gnomehotkey
QMAKE_CLEAN = $$PLUGINS_PREFIX/General/libgnomehotkey.so

TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib

RESOURCES = translations/translations.qrc

isEmpty(LIB_DIR){
    LIB_DIR = /lib
}
target.path = $$LIB_DIR/qmmp/General
INSTALLS += target


HEADERS += gnomehotkeyfactory.h \
    mediakeys.h

SOURCES += gnomehotkeyfactory.cpp \
    mediakeys.cpp

INCLUDEPATH += ../../../../src

LIBS += -lqmmpui -lqmmp
