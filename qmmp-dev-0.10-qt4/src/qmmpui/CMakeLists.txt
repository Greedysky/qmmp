project(libqmmpui)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 OLD)
cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

SET(QT_USE_QTNETWORK TRUE)
INCLUDE(FindQt4)
include(${QT_USE_FILE})

# qt lib
ADD_DEFINITIONS( -Wall )
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)
ADD_DEFINITIONS(-DQT_THREAD)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../)

link_directories(${CMAKE_INSTALL_PREFIX}/${LIB_DIR})
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../qmmp)

SET(libqmmpui_SRCS
    general.cpp
    uihelper.cpp
    playlistparser.cpp
    commandlinemanager.cpp
    filedialog.cpp
    qtfiledialog.cpp
    fileloader.cpp
    playstate.cpp
    playlistmodel.cpp
    playlistitem.cpp
    playlistgroup.cpp
    playlisttrack.cpp
    playlistcontainer.cpp
    normalcontainer.cpp
    groupedcontainer.cpp
    mediaplayer.cpp
    detailsdialog.cpp
    tageditor.cpp
    playlistmanager.cpp
    metadataformatter.cpp
    templateeditor.cpp
    uiloader.cpp
    jumptotrackdialog.cpp
    configdialog.cpp
    pluginitem.cpp
    aboutdialog.cpp
    qmmpuisettings.cpp
    radioitemdelegate.cpp
    playlistdownloader.cpp
    addurldialog.cpp
    qmmpuiplugincache.cpp
    tagupdater.cpp
    playlisttask.cpp
    playlistheadermodel.cpp
    columneditor.cpp
    metadatahelper.cpp
)

SET(libqmmpui_HDRS
    generalfactory.h
    playlistformat.h
    commandlinemanager.h
    commandlineoption.h
    filedialogfactory.h
    playstate_p.h
    playlistitem.h
    playlistgroup.h
    playlisttrack.h
    playlistcontainer_p.h
    normalcontainer_p.h
    groupedcontainer_p.h
    metadataformatter.h
    uiloader.h
    uifactory.h
    pluginitem_p.h
    general.h
    playlistparser.h
    qmmpuiplugincache_p.h
    metadatahelper_p.h
)

SET(libqmmpui_DEVEL_HDRS
    commandlinemanager.h
    commandlineoption.h
    configdialog.h
    detailsdialog.h
    filedialogfactory.h
    filedialog.h
    generalfactory.h
    general.h
    mediaplayer.h
    metadataformatter.h
    playlistdownloader.h
    playlistformat.h
    playlistitem.h
    playlistgroup.h
    playlisttrack.h
    playlistmanager.h
    playlistmodel.h
    playlistparser.h
    qmmpuisettings.h
    templateeditor.h
    uifactory.h
    uihelper.h
    uiloader.h
    playlistheadermodel.h
)


SET(libqmmpui_UIS
    forms/detailsdialog.ui
    forms/tageditor.ui
    forms/templateeditor.ui
    forms/jumptotrackdialog.ui
    forms/configdialog.ui
    forms/aboutdialog.ui
    forms/addurldialog.ui
    forms/columneditor.ui
)

QT4_WRAP_UI(libqmmpui_UIS_H ${libqmmpui_UIS})

QT4_ADD_RESOURCES(libqmmpui_RCC_SRCS translations/libqmmpui_locales.qrc txt/txt.qrc images/qmmpui_images.qrc)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(qmmpui SHARED ${libqmmpui_SRCS} ${libqmmpui_RCC_SRCS} ${libqmmpui_UIS_H} ${libqmmpui_HDRS})
target_link_libraries(qmmpui ${QT_LIBRARIES} libqmmp)
add_dependencies(qmmpui libqmmp)
SET_TARGET_PROPERTIES(qmmpui PROPERTIES VERSION ${QMMP_VERSION} SOVERSION ${QMMP_SOVERSION})
install(TARGETS qmmpui LIBRARY DESTINATION ${LIB_DIR}
                       RUNTIME DESTINATION bin
                       ARCHIVE DESTINATION ${LIB_DIR})
IF(NOT ${CMAKE_SYSTEM_NAME} MATCHES Darwin)
	install(FILES ${libqmmpui_DEVEL_HDRS} DESTINATION include/qmmpui)

	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/qmmpui.pc.in ${CMAKE_CURRENT_BINARY_DIR}/qmmpui.pc @ONLY)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qmmpui.pc DESTINATION ${LIB_DIR}/pkgconfig)
ENDIF()
