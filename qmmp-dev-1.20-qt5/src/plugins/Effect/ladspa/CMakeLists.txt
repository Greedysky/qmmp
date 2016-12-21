project(libladspa)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libladspa_SRCS
    ladspahost.cpp
    ladspahelper.cpp
    ladspaslider.cpp
    ladspabutton.cpp
    settingsdialog.cpp
    effectladspafactory.cpp
)

SET(libladspa_HDRS
    ladspahelper.h
)

SET(libladspa_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libladspa_RCC_SRCS ${libladspa_RCCS})

# user interface

SET(libladspa_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libladspa_UIS_H ${libladspa_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(ladspa MODULE ${libladspa_SRCS} ${libladspa_UIS_H}
  ${libladspa_RCC_SRCS} ${libladspa_HDRS})
add_dependencies(ladspa qmmp)
target_link_libraries(ladspa Qt5::Widgets -lqmmp ${CMAKE_DL_LIBS})
install(TARGETS ladspa DESTINATION ${LIB_DIR}/qmmp/Effect)

