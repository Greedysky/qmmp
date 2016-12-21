project(libscrobbler)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmpui
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
#libqmmp
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libscrobbler_SRCS
    settingsdialog.cpp
    scrobblerhandler.cpp
    scrobblerfactory.cpp
    scrobblercache.cpp
    scrobbler.cpp
)

SET(libscrobbler_HDRS
    scrobblercache.h
    defines.h
)

SET(libscrobbler_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libscrobbler_RCC_SRCS ${libscrobbler_RCCS})

# user interface


SET(libscrobbler_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libscrobbler_UIS_H ${libscrobbler_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(scrobbler MODULE ${libscrobbler_SRCS} ${libscrobbler_UIS_H}
  ${libscrobbler_RCC_SRCS} ${libscrobbler_HDRS})
add_dependencies(scrobbler qmmpui)
target_link_libraries(scrobbler Qt5::Widgets Qt5::Network -lqmmpui -lqmmp)
install(TARGETS scrobbler DESTINATION ${LIB_DIR}/qmmp/General)
