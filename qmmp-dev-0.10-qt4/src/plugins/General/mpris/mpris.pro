include(../../plugins.pri)

CONFIG += warn_on \
plugin  \
 lib \
 qdbus

TARGET =$$PLUGINS_PREFIX/General/mpris
QMAKE_CLEAN =$$PLUGINS_PREFIX/General/libmpris.so

TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib

RESOURCES = translations/translations.qrc

isEmpty(LIB_DIR){
    LIB_DIR = /lib
}
target.path = $$LIB_DIR/qmmp/General
INSTALLS += target
#FORMS += settingsdialog.ui

#RESOURCES += images/images.qrc


HEADERS += mprisfactory.h \
           mpris.h  \
           mpris1/playerobject.h \
           mpris1/rootobject.h \
           mpris1/tracklistobject.h \
           mpris2/root2object.h \
           mpris2/player2object.h

SOURCES += mprisfactory.cpp \
           mpris.cpp \
           mpris1/playerobject.cpp \
           mpris1/rootobject.cpp \
           mpris1/tracklistobject.cpp \
           mpris2/root2object.cpp \
           mpris2/player2object.cpp

INCLUDEPATH += ../../../../src

LIBS += -lqmmpui -lqmmp
