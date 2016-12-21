project(libsoxr)

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

pkg_check_modules(SOXR soxr)

include_directories(${SOXR_INCLUDE_DIRS})
link_directories(${SOXR_LIBRARY_DIRS})
ADD_DEFINITIONS(${SOXR_CFLAGS})

SET(libsoxr_SRCS
    soxresampler.cpp
    settingsdialog.cpp
    effectsoxrfactory.cpp
)

SET(libsoxr_HDRS
    soxresampler.h
)

SET(libsoxr_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libsoxr_RCC_SRCS ${libsoxr_RCCS})

# user interface

SET(libsoxr_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libsoxr_UIS_H ${libsoxr_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(SOXR_FOUND)
ADD_LIBRARY(soxr MODULE ${libsoxr_SRCS} ${libsoxr_UIS_H} ${libsoxr_RCC_SRCS} ${libsoxr_HDRS})
add_dependencies(soxr qmmp)
target_link_libraries(soxr Qt5::Widgets -lqmmp ${SOXR_LDFLAGS})
install(TARGETS soxr DESTINATION ${LIB_DIR}/qmmp/Effect)
ENDIF(SOXR_FOUND)
