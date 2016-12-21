project(libprojectm)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
  cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

# projectM
MESSAGE(STATUS "Found OpenGL implementation: ${Qt5Gui_OPENGL_IMPLEMENTATION}")
IF(${Qt5Gui_OPENGL_IMPLEMENTATION} STREQUAL GL)
  pkg_search_module(PROJECTM libprojectM>=2.0.0)
  ADD_DEFINITIONS(${PROJECTM_CFLAGS})
ELSE(${Qt5Gui_OPENGL_IMPLEMENTATION} STREQUAL GL)
  MESSAGE(WARNING "Unsuppoted OpenGL implementation, projectM disabled")
ENDIF(${Qt5Gui_OPENGL_IMPLEMENTATION} STREQUAL GL)

find_file(PROJECTM_CONFIG config.inp
          PATHS "/usr/share/projectM/" "/usr/local/share/projectM/")

IF(PROJECTM_CONFIG)
ADD_DEFINITIONS(-DPROJECTM_CONFIG=\\\"${PROJECTM_CONFIG}\\\")
MESSAGE(STATUS "Found projectM configuration: ${PROJECTM_CONFIG}")
ENDIF(PROJECTM_CONFIG)

SET(libprojectm_SRCS
    projectmwidget.cpp
    projectmplugin.cpp
    visualprojectmfactory.cpp
    projectmwrapper.cpp
)

SET(libprojectm_RCCS translations/translations.qrc)

QT5_ADD_RESOURCES(libprojectm_RCC_SRCS ${libprojectm_RCCS})

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})
IF(PROJECTM_FOUND)
ADD_LIBRARY(projectm MODULE ${libprojectm_SRCS} ${libprojectm_RCC_SRCS})
add_dependencies(projectm qmmp)
target_link_libraries(projectm Qt5::Widgets -lqmmp ${PROJECTM_LDFLAGS} -lGL)
install(TARGETS projectm DESTINATION ${LIB_DIR}/qmmp/Visual)
ENDIF(PROJECTM_FOUND)
