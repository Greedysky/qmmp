project(libladspa)


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

QT4_ADD_RESOURCES(libladspa_RCC_SRCS ${libladspa_RCCS})

# user interface

SET(libladspa_UIS
    settingsdialog.ui
)

QT4_WRAP_UI(libladspa_UIS_H ${libladspa_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(ladspa MODULE ${libladspa_SRCS} ${libladspa_UIS_H}
  ${libladspa_RCC_SRCS} ${libladspa_HDRS})
add_dependencies(ladspa qmmp)
target_link_libraries(ladspa ${QT_LIBRARIES} libqmmp ${CMAKE_DL_LIBS})
install(TARGETS ladspa DESTINATION ${LIB_DIR}/qmmp/Effect)

