include(../../qmmp.pri)

HEADERS += qmmpstarter.h \
           builtincommandlineoption.h

SOURCES += qmmpstarter.cpp \
           builtincommandlineoption.cpp \
    main.cpp



# Some conf to redirect intermediate stuff in separate dirs
UI_DIR = ./.build/ui/
MOC_DIR = ./.build/moc/
OBJECTS_DIR = ./.build/obj
QT += network
unix:TARGET = ../../bin/qmmp
win32:TARGET = ../../../bin/qmmp
CONFIG += thread \
    warn_on
QMAKE_LIBDIR += ../../lib \
    qmmpui

unix:LIBS += -L../../lib \
    -lqmmp \
    -lqmmpui
win32:LIBS += -L../../bin \
    -lqmmp0 \
    -lqmmpui0

win32:RC_FILE = qmmp.rc

INCLUDEPATH += ../
RESOURCES = images/images.qrc
TEMPLATE = app
unix {
    target.path = /bin
    desktop.files = qmmp.desktop \
        qmmp_enqueue.desktop \
        qmmp_dir.desktop
    desktop.path = /share/applications
    icon16.files = images/16x16/qmmp.png
    icon32.files = images/32x32/qmmp.png
    icon48.files = images/48x48/qmmp.png
    icon_svg.files = images/scalable/qmmp.svgz \
        images/scalable/qmmp-simple.svgz
    icon16.path = /share/icons/hicolor/16x16/apps
    icon32.path = /share/icons/hicolor/32x32/apps
    icon48.path = /share/icons/hicolor/48x48/apps
    icon_svg.path = /share/icons/hicolor/scalable/apps
    INSTALLS += desktop \
        target \
        icon16 \
        icon32 \
        icon48 \
        icon_svg
}
RESOURCES += translations/qmmp_locales.qrc
