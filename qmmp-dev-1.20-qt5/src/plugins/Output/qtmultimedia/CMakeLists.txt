project(libqtmultimedia)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libqtmultimedia_SRCS
  outputqtmultimedia.cpp
  outputqtmultimediafactory.cpp
  settingsdialog.cpp
)

SET(libqtmultimedia_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libqtmultimedia_RCC_SRCS ${libqtmultimedia_RCCS})

SET(libqtmultimedia_UIS
    settingsdialog.ui
)
QT5_WRAP_UI(libqtmultimedia_UIS_H ${libqtmultimedia_UIS})

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(qtmultimedia MODULE ${libqtmultimedia_SRCS} ${libqtmultimedia_UIS_H} ${libqtmultimedia_RCC_SRCS})
add_dependencies(qtmultimedia qmmp)
target_link_libraries(qtmultimedia Qt5::Widgets Qt5::Multimedia -lqmmp)
install(TARGETS qtmultimedia DESTINATION ${LIB_DIR}/qmmp/Output)
