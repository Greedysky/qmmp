include(../../plugins.pri)

CONFIG += warn_on \
plugin  \
 lib

TARGET = $$PLUGINS_PREFIX/CommandLineOptions/uninstalloption

TEMPLATE = lib
QMAKE_LIBDIR += ../../../../bin

TRANSLATIONS = translations/uninstall_plugin_cs.ts \
               translations/uninstall_plugin_de.ts \
               translations/uninstall_plugin_pl.ts \
               translations/uninstall_plugin_ru.ts \
               translations/uninstall_plugin_uk_UA.ts \
               translations/uninstall_plugin_zh_TW.ts \
               translations/uninstall_plugin_zh_CN.ts \
               translations/uninstall_plugin_it.ts \
               translations/uninstall_plugin_tr.ts \
               translations/uninstall_plugin_lt.ts \
               translations/uninstall_plugin_nl.ts \
               translations/uninstall_plugin_ja.ts \
               translations/uninstall_plugin_es.ts \
               translations/uninstall_plugin_sr_BA.ts \
               translations/uninstall_plugin_sr_RS.ts
RESOURCES = translations/translations.qrc


INCLUDEPATH += ../../../../src

LIBS += -lqmmpui0 -lqmmp0

HEADERS += uninstalloption.h

SOURCES += uninstalloption.cpp
