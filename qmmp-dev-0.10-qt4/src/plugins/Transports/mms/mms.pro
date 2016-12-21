include(../../plugins.pri)
HEADERS += \
    mmsinputfactory.h \
    mmsinputsource.h \
    settingsdialog.h \
    mmsstreamreader.h
SOURCES += \
    mmsinputfactory.cpp \
    mmsinputsource.cpp \
    settingsdialog.cpp \
    mmsstreamreader.cpp
win32:HEADERS += ../../../../src/qmmp/inputsource.h \
    ../../../../src/qmmp/inputsourcefactory.h
TARGET = $$PLUGINS_PREFIX/Transports/mms
unix:QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libmms.so
INCLUDEPATH += ../../../
CONFIG += warn_on \
    plugin \
    link_pkgconfig
TEMPLATE = lib

QMAKE_LIBDIR += ../../../../lib
LIBS += -lqmmp -L/usr/lib
PKGCONFIG += libmms
isEmpty(LIB_DIR):LIB_DIR = /lib
target.path = $$LIB_DIR/qmmp/Transports
INSTALLS += target

FORMS += settingsdialog.ui

RESOURCES = translations/translations.qrc
