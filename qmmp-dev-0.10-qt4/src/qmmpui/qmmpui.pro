include(../../qmmp.pri)

VERSION = $$QMMP_VERSION
INCLUDEPATH += ../
TEMPLATE = lib
QT += network

CONFIG += warn_on \
    shared \
    qt \
    thread

QMAKE_LIBDIR += ../../lib \
    qmmpui

unix {
    TARGET = ../../lib/qmmpui
    LIBS += -L../../lib -lqmmp
    isEmpty(LIB_DIR):LIB_DIR = /lib
    target.path = $$LIB_DIR
}

win32 {
    TARGET = ../../../bin/qmmpui
    LIBS += -L../../bin -lqmmp0 -lole32
    HEADERS += winfileassoc.h \
               winfileassocpage_p.h
    SOURCES += winfileassoc.cpp \
               winfileassocpage.cpp
    FORMS += forms/winfileassocpage.ui
}

HEADERS += general.h \
    generalfactory.h \
    playlistformat.h \
    playlistparser.h \
    commandlinemanager.h \
    commandlineoption.h \
    filedialog.h \
    filedialogfactory.h \
    qtfiledialog_p.h \
    playlistitem.h \
    playlistmodel.h \
    playstate_p.h \
    fileloader_p.h \
    mediaplayer.h \
    detailsdialog.h \
    tageditor_p.h \
    playlistmanager.h \
    templateeditor.h \
    uifactory.h \
    uiloader.h \
    uihelper.h \
    jumptotrackdialog_p.h \
    configdialog.h \
    pluginitem_p.h \
    aboutdialog_p.h \
    qmmpuisettings.h \
    radioitemdelegate_p.h \
    playlistdownloader.h \
    addurldialog_p.h \
    qmmpuiplugincache_p.h \
    tagupdater_p.h \
    playlistgroup.h \
    playlisttrack.h \
    playlistcontainer_p.h \
    groupedcontainer_p.h \
    normalcontainer_p.h \
    playlisttask_p.h \
    metadataformatter.h \
    columneditor_p.h \
    playlistheadermodel.h \
    metadatahelper_p.h

SOURCES += general.cpp \
    playlistparser.cpp \
    commandlinemanager.cpp \
    filedialog.cpp \
    qtfiledialog.cpp \
    playlistmodel.cpp \
    playstate.cpp \
    playlistitem.cpp \
    fileloader.cpp \
    mediaplayer.cpp \
    detailsdialog.cpp \
    tageditor.cpp \
    playlistmanager.cpp \
    templateeditor.cpp \
    uiloader.cpp \
    uihelper.cpp \
    jumptotrackdialog.cpp \
    configdialog.cpp \
    pluginitem.cpp \
    aboutdialog.cpp \
    qmmpuisettings.cpp \
    radioitemdelegate.cpp \
    playlistdownloader.cpp \
    addurldialog.cpp \
    qmmpuiplugincache.cpp \
    tagupdater.cpp \
    playlistgroup.cpp \
    playlisttrack.cpp \
    groupedcontainer.cpp \
    normalcontainer.cpp \
    playlistcontainer.cpp \
    playlisttask.cpp \
    metadataformatter.cpp \
    columneditor.cpp \
    playlistheadermodel.cpp \
    metadatahelper.cpp

FORMS += forms/detailsdialog.ui \
    forms/tageditor.ui \
    forms/templateeditor.ui \
    forms/jumptotrackdialog.ui \
    forms/configdialog.ui \
    forms/aboutdialog.ui \
    forms/addurldialog.ui \
    forms/columneditor.ui

unix:DESTDIR = .
RESOURCES += translations/libqmmpui_locales.qrc \
    images/qmmpui_images.qrc \
    txt/txt.qrc
TRANSLATIONS = translations/libqmmpui_ru.ts \
    translations/libqmmpui_tr.ts \
    translations/libqmmpui_zh_CN.ts \
    translations/libqmmpui_cs.ts \
    translations/libqmmpui_pt_BR.ts \
    translations/libqmmpui_uk_UA.ts \
    translations/libqmmpui_zh_TW.ts \
    translations/libqmmpui_de.ts \
    translations/libqmmpui_it.ts \
    translations/libqmmpui_lt.ts \
    translations/libqmmpui_pl_PL.ts \
    translations/libqmmpui_nl.ts \
    translations/libqmmpui_ja.ts \
    translations/libqmmpui_es.ts \
    translations/libqmmpui_sk.ts \
    translations/libqmmpui_sr_BA.ts \
    translations/libqmmpui_sr_RS.ts
unix {
    devel.files += general.h \
    generalfactory.h \
    playlistformat.h \
    playlistparser.h \
    commandlinemanager.h \
    commandlineoption.h \
    filedialog.h \
    filedialogfactory.h \
    playlistitem.h \
    playlistmodel.h \
    mediaplayer.h \
    detailsdialog.h \
    playlistmanager.h \
    templateeditor.h \
    uifactory.h \
    uiloader.h \
    uihelper.h \
    configdialog.h \
    qmmpuisettings.h \
    playlistdownloader.h \
    playlistgroup.h \
    playlisttrack.h \
    metadataformatter.h \
    playlistheadermodel.h

    devel.path = /include/qmmpui
    INSTALLS += target \
        devel
}

unix {
    CONFIG += create_pc create_prl no_install_prl
    QMAKE_PKGCONFIG_NAME = qmmpui
    QMAKE_PKGCONFIG_DESCRIPTION = qmmp user interface library
    QMAKE_PKGCONFIG_REQUIRES = QtCore QtGui QtNetwork qmmp
    QMAKE_PKGCONFIG_DESTDIR = pkgconfig
}
