include(../../plugins.pri)

QT += multimedia

FORMS += settingsdialog.ui

HEADERS += outputqtmultimediafactory.h \
           outputqtmultimedia.h \
           settingsdialog.h

SOURCES += outputqtmultimediafactory.cpp \
           outputqtmultimedia.cpp \
           settingsdialog.cpp

TARGET=$$PLUGINS_PREFIX/Output/qtmultimedia

INCLUDEPATH += ../../../
QMAKE_LIBDIR += ../../../../lib

CONFIG += warn_on \
thread \
plugin

TEMPLATE = lib

RESOURCES = translations/translations.qrc

unix {
    isEmpty (LIB_DIR){
    LIB_DIR = /lib
    }

    target.path = $$LIB_DIR/qmmp/Output
    INSTALLS += target
    LIBS += -lqmmp
    QMAKE_CLEAN =$$PLUGINS_PREFIX/Output/libqtmultimedia.so
}

win32 {
    LIBS += -lqmmp0
    QMAKE_LIBDIR += ../../../../bin
}
