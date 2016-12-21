include(../../plugins.pri)
FORMS += settingsdialog.ui
HEADERS += decodermadfactory.h \
    decoder_mad.h \
    settingsdialog.h \
    tagextractor.h \
    mpegmetadatamodel.h \
    replaygainreader.h
SOURCES += decoder_mad.cpp \
    decodermadfactory.cpp \
    settingsdialog.cpp \
    tagextractor.cpp \
    mpegmetadatamodel.cpp \
    replaygainreader.cpp
TARGET = $$PLUGINS_PREFIX/Input/mad

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
    LIBS += -lqmmp -lmad
    PKGCONFIG += taglib mad
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libmad.so
}

win32 {
    HEADERS += ../../../../src/qmmp/metadatamodel.h \
               ../../../../src/qmmp/decoderfactory.h
    QMAKE_LIBDIR += ../../../../bin
    LIBS += -lqmmp0 -lmad -ltag.dll
}
