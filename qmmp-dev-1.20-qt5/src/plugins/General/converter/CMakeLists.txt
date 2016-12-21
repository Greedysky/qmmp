project(libconverter)

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


SET(libconverter_SRCS
    converterfactory.cpp
    converter.cpp
    converterhelper.cpp
    converterdialog.cpp
    preseteditor.cpp
)

SET(libconverter_RCCS 
    translations/translations.qrc presets.qrc)

QT5_ADD_RESOURCES(libconverter_RCC_SRCS ${libconverter_RCCS})

# user interface

SET(libconverter_UIS
    converterdialog.ui
    preseteditor.ui
)

QT5_WRAP_UI(libconverter_UIS_H ${libconverter_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(converter MODULE ${libconverter_SRCS} ${libconverter_UIS_H} ${libconverter_RCC_SRCS})
add_dependencies(converter qmmpui)
target_link_libraries(converter Qt5::Widgets -lqmmpui -lqmmp ${TAGLIB_LDFLAGS})
install(TARGETS converter DESTINATION ${LIB_DIR}/qmmp/General)
