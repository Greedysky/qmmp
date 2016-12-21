project(librgscan)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmpui & libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# taglib
include_directories(${TAGLIB_INCLUDE_DIRS})
link_directories(${TAGLIB_LIBRARY_DIRS})
ADD_DEFINITIONS(${TAGLIB_CFLAGS})


SET(librgscan_SRCS
    rgscanfactory.cpp
    rgscanhelper.cpp
    rgscandialog.cpp
    rgscanner.cpp
    gain_analysis.c
)

SET(librgscan_HDRS
   gain_analysis.h
)

SET(librgscan_RCCS
    translations/translations.qrc
)

QT5_ADD_RESOURCES(librgscan_RCC_SRCS ${librgscan_RCCS})

# user interface

SET(librgscan_UIS
    rgscandialog.ui
)

QT5_WRAP_UI(librgscan_UIS_H ${librgscan_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(rgscan MODULE ${librgscan_SRCS} ${librgscan_UIS_H} ${librgscan_HDRS} ${librgscan_RCC_SRCS})
add_dependencies(rgscan qmmpui)
target_link_libraries(rgscan Qt5::Widgets -lqmmpui -lqmmp ${TAGLIB_LDFLAGS})
install(TARGETS rgscan DESTINATION ${LIB_DIR}/qmmp/General)
