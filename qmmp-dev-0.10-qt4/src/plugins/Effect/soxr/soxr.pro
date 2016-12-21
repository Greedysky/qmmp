include(../../plugins.pri)

HEADERS += soxresampler.h \
 effectsoxrfactory.h \
 settingsdialog.h

SOURCES += soxresampler.cpp \
 effectsoxrfactory.cpp \
 settingsdialog.cpp

TARGET=$$PLUGINS_PREFIX/Effect/soxr
QMAKE_CLEAN =$$PLUGINS_PREFIX/Effect/libsoxr.so
INCLUDEPATH += ../../../
CONFIG += warn_on \
plugin

TEMPLATE = lib

RESOURCES = translations/translations.qrc

unix {
    isEmpty(LIB_DIR){
        LIB_DIR = /lib
    }
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target
    CONFIG += link_pkgconfig
    PKGCONFIG += soxr
    QMAKE_LIBDIR += ../../../../lib
    LIBS += -lqmmp -L/usr/lib -I/usr/include
}

win32 {
    QMAKE_LIBDIR += ../../../../bin
    LIBS += -lqmmp0 -lsoxr
}

FORMS += settingsdialog.ui

