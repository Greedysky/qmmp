include(../../plugins.pri)
CONFIG += warn_on \
    plugin
TEMPLATE = lib
unix:QMAKE_LIBDIR += ../../../../lib
win32:QMAKE_LIBDIR += ../../../../bin
TARGET = $$PLUGINS_PREFIX/General/covermanager
unix:QMAKE_CLEAN = $$PLUGINS_PREFIX/General/libcovermanager.so
TRANSLATIONS = translations/covermanager_plugin_cs.ts \
    translations/covermanager_plugin_de.ts \
    translations/covermanager_plugin_zh_CN.ts \
    translations/covermanager_plugin_zh_TW.ts \
    translations/covermanager_plugin_ru.ts \
    translations/covermanager_plugin_pl.ts \
    translations/covermanager_plugin_uk_UA.ts \
    translations/covermanager_plugin_it.ts \
    translations/covermanager_plugin_tr.ts \
    translations/covermanager_plugin_lt.ts \
    translations/covermanager_plugin_nl.ts \
    translations/covermanager_plugin_ja.ts \
    translations/covermanager_plugin_es.ts \
    translations/covermanager_plugin_sr_BA.ts \
    translations/covermanager_plugin_sr_RS.ts

RESOURCES = translations/translations.qrc \
            images/cm_images.qrc
unix { 
    isEmpty(LIB_DIR):LIB_DIR = /lib
    target.path = $$LIB_DIR/qmmp/General
    INSTALLS += target
}
HEADERS += covermanagerfactory.h \
    coverwidget.h \
    covermanager.h
win32:HEADERS += ../../../../src/qmmpui/general.h
SOURCES += covermanagerfactory.cpp \
    coverwidget.cpp \
    covermanager.cpp
QT += network
INCLUDEPATH += ../../../
unix:LIBS += -lqmmpui \
    -lqmmp
win32:LIBS += -lqmmpui0 \
    -lqmmp0
FORMS += 
