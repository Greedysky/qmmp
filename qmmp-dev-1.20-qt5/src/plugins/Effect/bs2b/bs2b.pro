include(../../plugins.pri)

HEADERS += bs2bplugin.h \
           effectbs2bfactory.h \
           settingsdialog.h

SOURCES += bs2bplugin.cpp \
           effectbs2bfactory.cpp \
           settingsdialog.cpp

TARGET =$$PLUGINS_PREFIX/Effect/bs2b

INCLUDEPATH += ../../../
CONFIG += warn_on \
plugin \
link_pkgconfig

TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib

RESOURCES = translations/translations.qrc

FORMS += settingsdialog.ui

unix {
    isEmpty(LIB_DIR){
        LIB_DIR = /lib
    }
    target.path = $$LIB_DIR/qmmp/Effect
    INSTALLS += target

    PKGCONFIG += libbs2b
    LIBS += -lqmmp -L/usr/lib -I/usr/include
    QMAKE_CLEAN =$$PLUGINS_PREFIX/Effect/libbs2b.so
}

win32 {
    QMAKE_LIBDIR += ../../../../bin
    LIBS += -lqmmp0 -lbs2b
}
