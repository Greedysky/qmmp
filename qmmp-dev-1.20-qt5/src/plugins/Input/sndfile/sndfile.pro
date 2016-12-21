include(../../plugins.pri)

HEADERS += decodersndfilefactory.h \
           decoder_sndfile.h
SOURCES += decoder_sndfile.cpp \
           decodersndfilefactory.cpp

TARGET=$$PLUGINS_PREFIX/Input/sndfile


INCLUDEPATH += ../../../
CONFIG += warn_on \
plugin \
link_pkgconfig

TEMPLATE = lib

QMAKE_LIBDIR += ../../../../lib

RESOURCES = translations/translations.qrc

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target

    QMAKE_LIBDIR += ../../../../lib
    LIBS += -lqmmp
    PKGCONFIG += sndfile
    QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libsndfile.so
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin
    LIBS += -lqmmp0 -lsndfile -lflac -lvorbisenc -lvorbis -logg
}
