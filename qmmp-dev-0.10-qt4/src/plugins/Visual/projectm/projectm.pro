include(../../plugins.pri)

contains(CONFIG, WITH_PROJECTM20){
DEFINES += PROJECTM_20
}

TARGET =$$PLUGINS_PREFIX/Visual/projectm
QMAKE_CLEAN =$$PLUGINS_PREFIX/Visual/libprojectm.so

HEADERS += projectmwidget.h \
           visualprojectmfactory.h \
           projectmplugin.h \
    projectmwrapper.h
SOURCES += projectmplugin.cpp \
           visualprojectmfactory.cpp \
           projectmwidget.cpp \
    projectmwrapper.cpp
INCLUDEPATH += ../../../
CONFIG += warn_on \
plugin \
link_pkgconfig

TEMPLATE = lib
QMAKE_LIBDIR += ../../../../lib
QT += opengl

TRANSLATIONS = translations/projectm_plugin_cs.ts \
               translations/projectm_plugin_de.ts \
               translations/projectm_plugin_zh_CN.ts \
               translations/projectm_plugin_zh_TW.ts \
               translations/projectm_plugin_pl.ts \
               translations/projectm_plugin_ru.ts \
               translations/projectm_plugin_uk_UA.ts \
               translations/projectm_plugin_it.ts \
               translations/projectm_plugin_tr.ts \
               translations/projectm_plugin_lt.ts \
               translations/projectm_plugin_nl.ts \
               translations/projectm_plugin_ja.ts \
               translations/projectm_plugin_es.ts \
               translations/projectm_plugin_sr_BA.ts \
               translations/projectm_plugin_sr_RS.ts
RESOURCES = translations/translations.qrc

unix {
  PKGCONFIG += libprojectM
  isEmpty(LIB_DIR):LIB_DIR = /lib
  target.path = $$LIB_DIR/qmmp/Visual
  INSTALLS += target
  LIBS += -lqmmp -L/usr/lib -I/usr/include
  #projectM config path
  PROJECTM_CONFIG_FILES = /usr/share/projectM/config.inp \
                          /usr/local/share/projectM/config.inp
  for(path, PROJECTM_CONFIG_FILES) {
    exists($$path) {
      message ("found projectm configuration: "$$path)
      DEFINES += PROJECTM_CONFIG=\\\"$$path\\\"
    }
  }
}

win32 {
    QMAKE_LIBDIR += ../../../../bin
    LIBS += -lqmmp0 -lprojectM.dll
}
