project(libtrackchange)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmpui & libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libtrackchange_SRCS
    trackchange.cpp
    trackchangefactory.cpp
    settingsdialog.cpp
)

SET(libtrackchange_RCCS 
    translations/translations.qrc)

QT5_ADD_RESOURCES(libtrackchange_RCC_SRCS ${libtrackchange_RCCS})

# user interface
SET(libtrackchange_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libtrackchange_UIS_H ${libtrackchange_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(trackchange MODULE ${libtrackchange_SRCS} ${libtrackchange_UIS_H} ${libtrackchange_RCC_SRCS})
add_dependencies(trackchange qmmpui)
target_link_libraries(trackchange Qt5::Widgets -lqmmpui -lqmmp)
install(TARGETS trackchange DESTINATION ${LIB_DIR}/qmmp/General)
