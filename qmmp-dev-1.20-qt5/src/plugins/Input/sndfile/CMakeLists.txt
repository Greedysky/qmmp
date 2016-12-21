project(libsndfile)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
              
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# libsndfile
pkg_search_module(SNDFILE sndfile >= 1.0.21)

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

QT5_ADD_RESOURCES(libsndfile_RCC_SRCS ${libsndfile_RCCS})

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(SNDFILE_FOUND)
ADD_LIBRARY(sndfile MODULE ${libsndfile_SRCS} ${libsndfile_UIS_H}
  ${libsndfile_RCC_SRCS} ${libsndfile_HDRS})
add_dependencies(sndfile qmmp)
target_link_libraries(sndfile Qt5::Widgets -lqmmp ${SNDFILE_LDFLAGS})
install(TARGETS sndfile DESTINATION ${LIB_DIR}/qmmp/Input)
ENDIF(SNDFILE_FOUND)



