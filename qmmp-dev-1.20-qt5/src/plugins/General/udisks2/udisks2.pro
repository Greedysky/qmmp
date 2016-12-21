include(../../plugins.pri)

CONFIG += warn_on \
plugin  \
 lib

QT += dbus

TARGET = $$PLUGINS_PREFIX/General/udisks2
QMAKE_CLEAN = $$PLUGINS_PREFIX/General/libudisks2.so

TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib

RESOURCES = translations/translations.qrc

isEmpty(LIB_DIR){
    LIB_DIR = /lib
}
target.path = $$LIB_DIR/qmmp/General
INSTALLS += target


HEADERS += udisks2factory.h \
           udisks2plugin.h \
           udisks2manager.h \
           udisks2device.h \
           settingsdialog.h

SOURCES += udisks2factory.cpp \
           udisks2plugin.cpp \
           udisks2manager.cpp \
           udisks2device.cpp \
           settingsdialog.cpp

INCLUDEPATH += ../../../../src

LIBS += -lqmmpui -lqmmp

FORMS += settingsdialog.ui
