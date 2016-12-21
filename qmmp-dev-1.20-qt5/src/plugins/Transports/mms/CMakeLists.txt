project(libmms)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
    cmake_policy(SET CMP0005 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# libcurl
pkg_search_module(MMS libmms>=0.4)
include_directories(${MMS_INCLUDE_DIRS})
link_directories(${MMS_LIBRARY_DIRS})
ADD_DEFINITIONS(${MMS_CFLAGS})


SET(libmms_SRCS
    mmsstreamreader.cpp
    mmsinputfactory.cpp
    mmsinputsource.cpp
    settingsdialog.cpp
)

# resources
SET(libmms_RCCS translations/translations.qrc)
QT5_ADD_RESOURCES(libmms_RCC_SRCS ${libmms_RCCS})

# user interface
SET(libmms_UIS
    settingsdialog.ui
)
QT5_WRAP_UI(libmms_UIS_H ${libmms_UIS})


# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})
IF(MMS_FOUND)
ADD_LIBRARY(mms MODULE ${libmms_SRCS} ${libmms_UIS_H} ${libmms_RCC_SRCS})
target_link_libraries(mms Qt5::Widgets -lqmmp ${MMS_LDFLAGS})
add_dependencies(mms qmmp)
install(TARGETS mms DESTINATION ${LIB_DIR}/qmmp/Transports)
ENDIF(MMS_FOUND)
