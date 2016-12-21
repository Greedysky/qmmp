project(libqmmp)


if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 OLD)
    cmake_policy(SET CMP0005 OLD)
endif(COMMAND cmake_policy)

# qt lib
ADD_DEFINITIONS( -Wall )
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)
ADD_DEFINITIONS(-DQT_THREAD)
ADD_DEFINITIONS(-DLIB_DIR=\\\"/${LIB_DIR}\\\")
ADD_DEFINITIONS(-DQMMP_INSTALL_PREFIX=\\\"${CMAKE_INSTALL_PREFIX}\\\")

IF(NOT QMMP_VERSION_STABLE) #add svn revision
execute_process(COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/svn_revision.sh
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                OUTPUT_VARIABLE SVN_REVISION
                OUTPUT_STRIP_TRAILING_WHITESPACE)
IF(SVN_REVISION)
ADD_DEFINITIONS(-DSVN_REVISION=\\\"${SVN_REVISION}\\\")
ENDIF(SVN_REVISION)
ENDIF(NOT QMMP_VERSION_STABLE)

include_directories(${CMAKE_CURRENT_BINARY_DIR})


SET(QMMP_DEFAULT_OUTPUT "" CACHE STRING "default output plugin")
IF(QMMP_DEFAULT_OUTPUT)
    ADD_DEFINITIONS(-DQMMP_DEFAULT_OUTPUT=\\\"${QMMP_DEFAULT_OUTPUT}\\\")
    MESSAGE(STATUS "Default output: ${QMMP_DEFAULT_OUTPUT}")
ENDIF(QMMP_DEFAULT_OUTPUT)


SET(libqmmp_SRCS
    visual.cpp
    recycler.cpp
    decoder.cpp
    output.cpp
    outputwriter.cpp
    fileinfo.cpp
    equ/iir.c
    equ/iir_cfs.c
    equ/iir_fpu.c
    soundcore.cpp
    effect.cpp
    qmmp.cpp
    statehandler.cpp
    volumecontrol.cpp
    metadatamodel.cpp
    tagmodel.cpp
    qmmpaudioengine.cpp
    abstractengine.cpp
    audioparameters.cpp
    inputsource.cpp
    fileinputsource.cpp
    emptyinputsource.cpp
    metadatamanager.cpp
    replaygain.cpp
    qmmpsettings.cpp
    audioconverter.cpp
    eqsettings.cpp
    qmmpevents.cpp
    qmmpplugincache.cpp
    channelmap.cpp
    channelconverter.cpp
    volume.cpp
    dithering.cpp
)

SET(libqmmp_HDRS
    qmmpevents_p.h
    outputfactory.h
    decoderfactory.h
    effectfactory.h
    equ/iir_cfs.h
    equ/iir_fpu.h
    equ/iir.h
    visualfactory.h
    recycler_p.h
    buffer.h
    effect.h
    qmmp.h
    replaygain_p.h
    fileinfo.h
    audioconverter.h
    qmmpplugincache_p.h
    eqsettings.h
    decoder.h
    tagmodel.h
    audioparameters.h
    inputsourcefactory.h
    enginefactory.h
    metadatamanager.h
    output.h
    channelmap.h
    channelconverter_p.h
    dithering_p.h
)

SET(libqmmp_DEVEL_HDRS
    abstractengine.h
    audioconverter.h
    audioparameters.h
    buffer.h
    channelmap.h
    decoderfactory.h
    decoder.h
    effectfactory.h
    effect.h
    enginefactory.h
    eqsettings.h
    fileinfo.h
    inputsourcefactory.h
    inputsource.h
    metadatamanager.h
    metadatamodel.h
    outputfactory.h
    output.h
    qmmp.h
    qmmpsettings.h
    soundcore.h
    statehandler.h
    tagmodel.h
    visualfactory.h
    visual.h
    volume.h
)

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(libqmmp SHARED ${libqmmp_SRCS} ${libqmmp_HDRS})
target_link_libraries(libqmmp ${QT_LIBRARIES})
SET_TARGET_PROPERTIES(libqmmp PROPERTIES VERSION ${QMMP_VERSION} SOVERSION ${QMMP_SOVERSION} OUTPUT_NAME qmmp)
install(TARGETS libqmmp LIBRARY DESTINATION ${LIB_DIR}
                        RUNTIME DESTINATION bin
                        ARCHIVE DESTINATION ${LIB_DIR})

IF(NOT ${CMAKE_SYSTEM_NAME} MATCHES Darwin)
	install(FILES ${libqmmp_DEVEL_HDRS} DESTINATION include/qmmp)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/qmmp.pc.in ${CMAKE_CURRENT_BINARY_DIR}/qmmp.pc @ONLY)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qmmp.pc DESTINATION ${LIB_DIR}/pkgconfig)
ENDIF()
