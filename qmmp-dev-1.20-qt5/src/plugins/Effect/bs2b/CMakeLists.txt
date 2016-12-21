project(libbs2b)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libbs2b_SRCS
    bs2bplugin.cpp
    settingsdialog.cpp
    effectbs2bfactory.cpp
)

SET(libbs2b_HDRS
    bs2bplugin.h
)

SET(libbs2b_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libbs2b_RCC_SRCS ${libbs2b_RCCS})

pkg_search_module(BS2B libbs2b>=3.0.0)

include_directories(${BS2B_INCLUDE_DIRS})
link_directories(${BS2B_LIBRARY_DIRS})
ADD_DEFINITIONS(${BS2B_CFLAGS})

# user interface

SET(libbs2b_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libbs2b_UIS_H ${libbs2b_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(BS2B_FOUND)
ADD_LIBRARY(bs2b MODULE ${libbs2b_SRCS} ${libbs2b_UIS_H}
  ${libbs2b_RCC_SRCS} ${libbs2b_HDRS})
add_dependencies(bs2b qmmp)
target_link_libraries(bs2b Qt5::Widgets -lqmmp ${BS2B_LDFLAGS})
install(TARGETS bs2b DESTINATION ${LIB_DIR}/qmmp/Effect)
ENDIF(BS2B_FOUND)
