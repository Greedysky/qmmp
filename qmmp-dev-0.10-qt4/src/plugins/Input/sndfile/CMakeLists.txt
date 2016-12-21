project(libsndfile)


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

# libsndfile
pkg_search_module(SNDFILE sndfile)

include_directories(${SNDFILE_INCLUDE_DIRS})
link_directories(${SNDFILE_LIBRARY_DIRS})
ADD_DEFINITIONS(${SNDFILE_CFLAGS})

SET(libsndfile_SRCS
  decoder_sndfile.cpp
  decodersndfilefactory.cpp
)

SET(libsndfile_HDRS
  decoder_sndfile.h
)

SET(libsndfile_RCCS translations/translations.qrc)

QT4_ADD_RESOURCES(libsndfile_RCC_SRCS ${libsndfile_RCCS})

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(SNDFILE_FOUND)
ADD_LIBRARY(sndfile MODULE ${libsndfile_SRCS} ${libsndfile_UIS_H}
  ${libsndfile_RCC_SRCS} ${libsndfile_HDRS})
add_dependencies(sndfile qmmp)
target_link_libraries(sndfile ${QT_LIBRARIES} libqmmp ${SNDFILE_LDFLAGS})
install(TARGETS sndfile DESTINATION ${LIB_DIR}/qmmp/Input)
ENDIF(SNDFILE_FOUND)



