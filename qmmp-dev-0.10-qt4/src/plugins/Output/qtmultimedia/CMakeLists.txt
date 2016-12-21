project(libqtmultimedia)


SET(QT_USE_QTMULTIMEDIA TRUE)
INCLUDE(FindQt4)
INCLUDE(${QT_USE_FILE})

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

# qt plugin
ADD_DEFINITIONS( -Wall )
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)
ADD_DEFINITIONS(-DQT_THREAD)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(QT_INCLUDES
  ${QT_INCLUDES}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../../
)

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libqtmultimedia_SRCS
  outputqtmultimedia.cpp
  outputqtmultimediafactory.cpp
  settingsdialog.cpp
)

SET(libqtmultimedia_HDRS
  outputqtmultimedia.h
)

SET(libqtmultimedia_RCCS translations/translations.qrc)

QT4_ADD_RESOURCES(libqtmultimedia_RCC_SRCS ${libqtmultimedia_RCCS})

SET(libqtmultimedia_UIS
    settingsdialog.ui
)
QT4_WRAP_UI(libqtmultimedia_UIS_H ${libqtmultimedia_UIS})

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(${QT_QTMULTIMEDIA_FOUND})
	ADD_LIBRARY(qtmultimedia MODULE ${libqtmultimedia_SRCS} ${libqtmultimedia_UIS_H} ${libqtmultimedia_RCC_SRCS} ${libqtmultimedia_HDRS})
	add_dependencies(qtmultimedia qmmp)
	target_link_libraries(qtmultimedia ${QT_LIBRARIES} libqmmp)
	install(TARGETS qtmultimedia DESTINATION ${LIB_DIR}/qmmp/Output)
ENDIF(${QT_QTMULTIMEDIA_FOUND})
