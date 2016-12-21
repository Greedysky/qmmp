include(../../plugins.pri)

CONFIG += warn_on \
plugin  \
 lib \
 qdbus

TARGET = $$PLUGINS_PREFIX/General/udisks
QMAKE_CLEAN = $$PLUGINS_PREFIX/General/libudisks.so

TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib

RESOURCES = translations/translations.qrc

isEmpty(LIB_DIR){
    LIB_DIR = /lib
}
target.path = $$LIB_DIR/qmmp/General
INSTALLS += target


HEADERS += udisksfactory.h \
           udisksplugin.h \
           udisksmanager.h \
           udisksdevice.h \
           settingsdialog.h

SOURCES += udisksfactory.cpp \
           udisksplugin.cpp \
           udisksmanager.cpp \
           udisksdevice.cpp \
           settingsdialog.cpp

INCLUDEPATH += ../../../../src

LIBS += -lqmmpui -lqmmp

FORMS += settingsdialog.ui
