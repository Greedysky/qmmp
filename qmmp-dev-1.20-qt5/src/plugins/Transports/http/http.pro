include(../../plugins.pri)
HEADERS += \
    httpinputfactory.h \
    httpinputsource.h \
    settingsdialog.h \
    httpstreamreader.h
SOURCES += \
    httpinputfactory.cpp \
    httpinputsource.cpp \
    settingsdialog.cpp \
    httpstreamreader.cpp
win32:HEADERS += ../../../../src/qmmp/inputsource.h \
    ../../../../src/qmmp/inputsourcefactory.h
TARGET = $$PLUGINS_PREFIX/Transports/http
unix:QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libhttp.so
INCLUDEPATH += ../../../
CONFIG += warn_on \
    plugin \
    link_pkgconfig
TEMPLATE = lib
unix {
    QMAKE_LIBDIR += ../../../../lib
    LIBS += -lqmmp \
        -L/usr/lib
    PKGCONFIG += libcurl
    isEmpty(LIB_DIR):LIB_DIR = /lib
    target.path = $$LIB_DIR/qmmp/Transports
    INSTALLS += target
}
win32 {
    QMAKE_LIBDIR += ../../../../bin
    LIBS += -lqmmp0 \
        -lcurldll
}

contains(CONFIG, WITH_ENCA){
   CONFIG += link_pkgconfig
   DEFINES += WITH_ENCA
   unix:PKGCONFIG += enca
   win32:LIBS += -lenca.dll

}

FORMS += settingsdialog.ui

RESOURCES = translations/translations.qrc
