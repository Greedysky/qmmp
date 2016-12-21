include(../../plugins.pri)
INCLUDEPATH += ../../../../src
CONFIG += warn_on \
    plugin
unix:TARGET = $$PLUGINS_PREFIX/General/statusicon
unix:QMAKE_CLEAN = $$PLUGINS_PREFIX/General/libstatusicon.so
win32:TARGET = $$PLUGINS_PREFIX/General/statusicon
TEMPLATE = lib
unix {
    QMAKE_LIBDIR += ../../../../lib
    LIBS += -lqmmpui \
        -lqmmp
}
win32 {
    QMAKE_LIBDIR += ../../../../bin
    LIBS += -lqmmpui0 \
        -lqmmp0
}

RESOURCES = translations/translations.qrc
unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib
    target.path = $$LIB_DIR/qmmp/General
    INSTALLS += target
}
RESOURCES += images/tray_images.qrc
HEADERS += statusiconfactory.h \
    statusicon.h \
    settingsdialog.h \
    qmmptrayicon.h \
    statusiconpopupwidget.h \
    coverwidget.h
SOURCES += statusiconfactory.cpp \
    statusicon.cpp \
    settingsdialog.cpp \
    qmmptrayicon.cpp \
    statusiconpopupwidget.cpp \
    coverwidget.cpp
FORMS += settingsdialog.ui
win32:HEADERS += ../../../../src/qmmpui/general.h
