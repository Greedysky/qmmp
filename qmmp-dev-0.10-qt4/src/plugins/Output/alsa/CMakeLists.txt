project(libalsa)


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

# alsa
pkg_search_module(ALSA alsa)
include_directories(${ALSA_INCLUDE_DIRS})
link_directories(${ALSA_LIBRARY_DIRS})
ADD_DEFINITIONS(${ALSA_CFLAGS})

SET(libalsa_SRCS
  outputalsa.cpp
  outputalsafactory.cpp
  settingsdialog.cpp
)

SET(libalsa_RCCS translations/translations.qrc)

QT4_ADD_RESOURCES(libalsa_RCC_SRCS ${libalsa_RCCS})

# user interface

SET(libalsa_UIS
    settingsdialog.ui
)

QT4_WRAP_UI(libalsa_UIS_H ${libalsa_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(ALSA_FOUND)
ADD_LIBRARY(alsa MODULE ${libalsa_SRCS} ${libalsa_UIS_H} ${libalsa_RCC_SRCS})
add_dependencies(alsa qmmp)
target_link_libraries(alsa ${QT_LIBRARIES} libqmmp ${ALSA_LDFLAGS})
install(TARGETS alsa DESTINATION ${LIB_DIR}/qmmp/Output)
ENDIF(ALSA_FOUND)

