include(../../plugins.pri)

INCLUDEPATH += ../../../../src
CONFIG += warn_on \
plugin \
link_pkgconfig

TARGET =$$PLUGINS_PREFIX/General/converter
unix:QMAKE_CLEAN = $$PLUGINS_PREFIX/General/libconverter.so


TEMPLATE = lib
unix:QMAKE_LIBDIR += ../../../../lib
unix:LIBS += -lqmmpui -lqmmp

PKGCONFIG += taglib

win32:QMAKE_LIBDIR += ../../../../bin
win32:LIBS += -lqmmpui0 -lqmmp0

RESOURCES = translations/translations.qrc presets.qrc
unix{
isEmpty(LIB_DIR){
    LIB_DIR = /lib
}
target.path = $$LIB_DIR/qmmp/General
INSTALLS += target
}
HEADERS += converterfactory.h \
    converterhelper.h \
    converterdialog.h \
    converter.h \
    preseteditor.h
win32:HEADERS += ../../../../src/qmmpui/general.h
SOURCES += converterfactory.cpp \
    converterhelper.cpp \
    converterdialog.cpp \
    converter.cpp \
    preseteditor.cpp

FORMS += converterdialog.ui \
    preseteditor.ui
