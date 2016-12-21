include(../../plugins.pri)

CONFIG += warn_on \
plugin  \
 lib

TARGET =$$PLUGINS_PREFIX/CommandLineOptions/incdecvolumeoption
QMAKE_CLEAN =$$PLUGINS_PREFIX/CommandLineOptions/libincdecvolumeoption.so

TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib

RESOURCES = translations/translations.qrc

isEmpty(LIB_DIR){
    LIB_DIR = /lib
}
target.path = $$LIB_DIR/qmmp/CommandLineOptions
INSTALLS += target

INCLUDEPATH += ../../../../src

LIBS += -lqmmpui -lqmmp

HEADERS += incdecvolumeoption.h

SOURCES += incdecvolumeoption.cpp
