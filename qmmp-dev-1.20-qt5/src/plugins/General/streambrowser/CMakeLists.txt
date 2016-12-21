project(libstreambrowser)

if(COMMAND cmake_policy)
cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# libqmmpui & libqmmp
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmpui)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/../../../qmmp)

SET(libstreambrowser_SRCS
    streambrowser.cpp
    streambrowserfactory.cpp
    streamwindow.cpp
)

SET(libstreambrowser_RCCS 
    translations/translations.qrc)

QT5_ADD_RESOURCES(libstreambrowser_RCC_SRCS ${libstreambrowser_RCCS})

# user interface

SET(libstreambrowser_UIS
    streamwindow.ui
)

QT5_WRAP_UI(libstreambrowser_UIS_H ${libstreambrowser_UIS})
# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(streambrowser MODULE ${libstreambrowser_SRCS} ${libstreambrowser_UIS_H} ${libstreambrowser_RCC_SRCS})
add_dependencies(streambrowser qmmpui)
target_link_libraries(streambrowser Qt5::Widgets Qt5::Network -lqmmpui -lqmmp)
install(TARGETS streambrowser DESTINATION ${LIB_DIR}/qmmp/General)
