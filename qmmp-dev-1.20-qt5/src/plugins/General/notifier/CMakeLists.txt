project(libnotifier)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmpui & libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libnotifier_SRCS
    settingsdialog.cpp
    notifier.cpp
    notifierfactory.cpp
    popupwidget.cpp
)

SET(libnotifier_RCCS 
    translations/translations.qrc
    notifier_images.qrc)

QT5_ADD_RESOURCES(libnotifier_RCC_SRCS ${libnotifier_RCCS})

# user interface


SET(libnotifier_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libnotifier_UIS_H ${libnotifier_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(notifier MODULE ${libnotifier_SRCS} ${libnotifier_UIS_H} ${libnotifier_RCC_SRCS})
add_dependencies(notifier qmmpui)
IF(X11_FOUND AND Qt5X11Extras_FOUND)
  include_directories(${X11_INCLUDE_DIRS})
  link_directories(${X11_LIBRARY_DIRS})
  ADD_DEFINITIONS(${X11_CFLAGS})
  ADD_DEFINITIONS(-DQ_WS_X11)
  target_link_libraries(notifier Qt5::X11Extras Qt5::Widgets -lqmmpui -lqmmp)
ELSE(X11_FOUND AND Qt5X11Extras_FOUND)
  target_link_libraries(notifier Qt5::Widgets -lqmmpui -lqmmp)
ENDIF(X11_FOUND AND Qt5X11Extras_FOUND)
install(TARGETS notifier DESTINATION ${LIB_DIR}/qmmp/General)
