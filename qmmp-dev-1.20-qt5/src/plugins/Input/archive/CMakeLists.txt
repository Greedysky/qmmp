project(libarchive)


if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
    cmake_policy(SET CMP0005 NEW)
endif(COMMAND cmake_policy) 

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# libarchive
pkg_search_module(LIBARCHIVE libarchive>=3.2.0)

include_directories(${archive_INCLUDE_DIRS} ${TAGLIB_INCLUDE_DIRS})
link_directories(${archive_LIBRARY_DIRS} ${TAGLIB_LIBRARY_DIRS})
ADD_DEFINITIONS(${archive_CFLAGS} ${TAGLIB_CFLAGS})

SET(libarchive_SRCS
  archiveinputdevice.cpp
  archivemetadatamodel.cpp
  decoder_archive.cpp
  archivetagreader.cpp
  decoderarchivefactory.cpp
)

SET(libarchive_HDRS
  archivetagreader.h
  decoder_archive.h
)

SET(libarchive_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libarchive_RCC_SRCS ${libarchive_RCCS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(LIBARCHIVE_FOUND)
ADD_LIBRARY(archive MODULE ${libarchive_SRCS} ${libarchive_RCC_SRCS} ${libarchive_HDRS})
add_dependencies(archive qmmp)
target_link_libraries(archive Qt5::Widgets libqmmp ${LIBARCHIVE_LDFLAGS} ${TAGLIB_LDFLAGS})
install(TARGETS archive DESTINATION ${LIB_DIR}/qmmp/Input)
ENDIF(LIBARCHIVE_FOUND)
