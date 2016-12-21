include(../../plugins.pri)

HEADERS += \
    archiveinputdevice.h \
    decoderarchivefactory.h \
    decoder_archive.h \
    archivetagreader.h \
    archivemetadatamodel.h

SOURCES += \
    archiveinputdevice.cpp \
    decoderarchivefactory.cpp \
    decoder_archive.cpp \
    archivetagreader.cpp \
    archivemetadatamodel.cpp

TARGET=$$PLUGINS_PREFIX/Input/archive


INCLUDEPATH += ../../../
CONFIG += warn_on \
plugin \
link_pkgconfig

TEMPLATE = lib

RESOURCES = translations/translations.qrc

unix {
    isEmpty(LIB_DIR):LIB_DIR = /lib
    target.path = $$LIB_DIR/qmmp/Input
    INSTALLS += target

    QMAKE_LIBDIR += ../../../../lib
    LIBS += -lqmmp
    PKGCONFIG += libarchive taglib
    QMAKE_CLEAN =$$PLUGINS_PREFIX/Input/libarchive.so
}

win32 {
#    HEADERS += ../../../../src/qmmp/metadatamodel.h \
#               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin
    LIBS += -lqmmp0 -larchive -ltag
}
