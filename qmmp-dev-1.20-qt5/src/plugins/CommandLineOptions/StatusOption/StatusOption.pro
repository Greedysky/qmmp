include(../../plugins.pri)

CONFIG += warn_on \
plugin  \
 lib

TARGET = $$PLUGINS_PREFIX/CommandLineOptions/statusoption
QMAKE_CLEAN = $$PLUGINS_PREFIX/CommandLineOptions/libstatusoption.so

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

HEADERS += statusoption.h

SOURCES += statusoption.cpp
