include(../../plugins.pri)

INCLUDEPATH += ../../../../src
CONFIG += warn_on \
plugin \
link_pkgconfig

TARGET =$$PLUGINS_PREFIX/General/rgscan
unix:QMAKE_CLEAN = $$PLUGINS_PREFIX/General/librgscan.so


TEMPLATE = lib
unix:QMAKE_LIBDIR += ../../../../lib
unix:LIBS += -lqmmpui -lqmmp

win32:QMAKE_LIBDIR += ../../../../bin
win32:LIBS += -lqmmpui0 -lqmmp0 -ltag.dll

RESOURCES = translations/translations.qrc
unix{
isEmpty(LIB_DIR){
    LIB_DIR = /lib
}
PKGCONFIG += taglib
target.path = $$LIB_DIR/qmmp/General
INSTALLS += target
}
HEADERS += rgscanfactory.h \
    rgscanhelper.h \
    rgscandialog.h \
    gain_analysis.h \
    rgscanner.h

win32:HEADERS += ../../../../src/qmmpui/general.h
SOURCES += rgscanfactory.cpp \
    rgscanhelper.cpp \
    rgscandialog.cpp \
    gain_analysis.c \
    rgscanner.cpp

FORMS += rgscandialog.ui
