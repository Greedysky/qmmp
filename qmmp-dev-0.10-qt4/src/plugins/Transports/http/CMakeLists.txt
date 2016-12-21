project(libhttp)


if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
    cmake_policy(SET CMP0005 NEW)
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
  ${CMAKE_CURRENT_BINARY_DIR}/../../../
)

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# libcurl
pkg_search_module(CURL libcurl)
include_directories(${CURL_INCLUDE_DIRS})
link_directories(${CURL_LIBRARY_DIRS})
ADD_DEFINITIONS(${CURL_CFLAGS})

# libenca
IF(USE_ENCA AND ENCA_FOUND)
ADD_DEFINITIONS(-DWITH_ENCA)
include_directories(${ENCA_INCLUDE_DIRS})
link_directories(${ENCA_LIBRARY_DIRS})
ENDIF(USE_ENCA AND ENCA_FOUND)

SET(libhttp_SRCS
    httpstreamreader.cpp
    httpinputfactory.cpp
    httpinputsource.cpp
    settingsdialog.cpp
)

# resources
SET(libhttp_RCCS translations/translations.qrc)
QT4_ADD_RESOURCES(libhttp_RCC_SRCS ${libhttp_RCCS})

# user interface
SET(libhttp_UIS
    settingsdialog.ui
)
QT4_WRAP_UI(libhttp_UIS_H ${libhttp_UIS})


# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(CURL_FOUND)
ADD_LIBRARY(http MODULE ${libhttp_SRCS} ${libhttp_UIS_H} ${libhttp_RCC_SRCS})

IF(USE_ENCA AND ENCA_FOUND)
target_link_libraries(http ${QT_LIBRARIES} libqmmp ${CURL_LDFLAGS} ${CURL_CFLAGS} ${ENCA_LDFLAGS} ${ENCA_CFLAGS})
ELSE(USE_ENCA AND ENCA_FOUND)
target_link_libraries(http ${QT_LIBRARIES} libqmmp ${CURL_LDFLAGS})
ENDIF(USE_ENCA AND ENCA_FOUND)

add_dependencies(http qmmp)
install(TARGETS http DESTINATION ${LIB_DIR}/qmmp/Transports)
ENDIF(CURL_FOUND)
