project(libcue)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

IF(USE_ENCA AND ENCA_FOUND)
ADD_DEFINITIONS(-DWITH_ENCA)
include_directories(${ENCA_INCLUDE_DIRS})
link_directories(${ENCA_LIBRARY_DIRS})
ADD_DEFINITIONS(${ENCA_CFLAGS})
ENDIF(USE_ENCA AND ENCA_FOUND)

SET(libcue_SRCS
  decoder_cue.cpp
  decodercuefactory.cpp
  settingsdialog.cpp
  cueparser.cpp
  cuemetadatamodel.cpp
)

SET(libcue_HDRS
  decoder_cue.h
  cueparser.h
)

SET(libcue_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libcue_RCC_SRCS ${libcue_RCCS})

# user interface


SET(libcue_UIS
    settingsdialog.ui
)

QT5_WRAP_UI(libcue_UIS_H ${libcue_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(cue MODULE ${libcue_SRCS} ${libcue_RCC_SRCS} ${libcue_UIS_H} ${libcue_HDRS})
add_dependencies(cue qmmp)
IF(USE_ENCA AND ENCA_FOUND)
target_link_libraries(cue Qt5::Widgets -lqmmp ${ENCA_LDFLAGS})
ELSE(USE_ENCA AND ENCA_FOUND)
target_link_libraries(cue Qt5::Widgets -lqmmp)
ENDIF(USE_ENCA AND ENCA_FOUND)
install(TARGETS cue DESTINATION ${LIB_DIR}/qmmp/Input)
