include(../../plugins.pri)
FORMS += settingsdialog.ui

HEADERS += outputoss4factory.h \
           outputoss4.h \
           settingsdialog.h


SOURCES += outputoss4factory.cpp \
           outputoss4.cpp \
           settingsdialog.cpp

TARGET=$$PLUGINS_PREFIX/Output/oss4
QMAKE_CLEAN =$$PLUGINS_PREFIX/Output/liboss4.so


INCLUDEPATH += ../../../
QMAKE_LIBDIR += ../../../../lib
CONFIG += warn_on \
thread \
plugin

DEFINES += HAVE_SYS_SOUNDCARD_H
INCLUDEPATH += /usr/lib/oss/include

TEMPLATE = lib
LIBS += -lqmmp

TRANSLATIONS = translations/oss4_plugin_cs.ts \
               translations/oss4_plugin_de.ts \
               translations/oss4_plugin_zh_CN.ts \
               translations/oss4_plugin_zh_TW.ts \
               translations/oss4_plugin_ru.ts \
               translations/oss4_plugin_pl.ts \
               translations/oss4_plugin_uk_UA.ts \
               translations/oss4_plugin_it.ts \
               translations/oss4_plugin_tr.ts \
               translations/oss4_plugin_lt.ts \
               translations/oss4_plugin_nl.ts \
               translations/oss4_plugin_ja.ts \
               translations/oss4_plugin_es.ts \
               translations/oss4_plugin_sr_BA.ts \
               translations/oss4_plugin_sr_RS.ts

RESOURCES = translations/translations.qrc
isEmpty (LIB_DIR){
LIB_DIR = /lib
}

target.path = $$LIB_DIR/qmmp/Output
INSTALLS += target
