cmake_minimum_required(VERSION 2.8.6 FATAL_ERROR)

#freebsd support
include_directories(SYSTEM /usr/local/include)
SET(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} /usr/local/include)

#macports support
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	SET(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} /opt/local/include)
ENDIF()

#extract version from qmmp.h
FILE(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/src/qmmp/qmmp.h"
     QMMP_VERSION_DATA REGEX "^#define[ \t]+QMMP_VERSION_[A-Z]+[ \t]+[0-9]+.*$")

IF(QMMP_VERSION_DATA)
   FOREACH(item IN ITEMS MAJOR MINOR PATCH STABLE)
      STRING(REGEX REPLACE ".*#define[ \t]+QMMP_VERSION_${item}[ \t]+([0-9]+).*"
       "\\1" QMMP_VERSION_${item} ${QMMP_VERSION_DATA})
   ENDFOREACH(item IN ITEMS MAJOR MINOR PATCH)
   SET(QMMP_VERSION "${QMMP_VERSION_MAJOR}.${QMMP_VERSION_MINOR}.${QMMP_VERSION_PATCH}")
   SET(QMMP_SOVERSION "${QMMP_VERSION_MAJOR}")
   IF(QMMP_VERSION_STABLE)
      MESSAGE(STATUS "Qmmp API version: ${QMMP_VERSION} (stable)")
   ELSE(QMMP_VERSION_STABLE)
      MESSAGE(STATUS "Qmmp API version: ${QMMP_VERSION} (development)")
   ENDIF(QMMP_VERSION_STABLE)
ELSE(QMMP_VERSION_DATA)
   MESSAGE(FATAL_ERROR "invalid header")
ENDIF(QMMP_VERSION_DATA)

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

SET(QT_MIN_VERSION 4.6.0)
INCLUDE(FindQt4)
INCLUDE(${QT_USE_FILE})
INCLUDE(FindPkgConfig)
SET(CMAKE_AUTOMOC ON)

IF(NOT QT_LRELEASE_EXECUTABLE)
FIND_PROGRAM(QT_LRELEASE_EXECUTABLE
    NAMES lrelease
    PATHS ${QT_BINARY_DIR}
    NO_DEFAULT_PATH
)
ENDIF(NOT QT_LRELEASE_EXECUTABLE)

#debian hack
IF(EXISTS ${QT_BINARY_DIR}/lrelease-qt4)
SET(QT_LRELEASE_EXECUTABLE ${QT_BINARY_DIR}/lrelease-qt4)
ENDIF(EXISTS ${QT_BINARY_DIR}/lrelease-qt4)

IF(QT_LRELEASE_EXECUTABLE)
MESSAGE(STATUS "Found lrelease executable: " ${QT_LRELEASE_EXECUTABLE})
ELSE(QT_LRELEASE_EXECUTABLE)
MESSAGE(FATAL_ERROR "Could NOT find lrelease executable")
ENDIF(QT_LRELEASE_EXECUTABLE)

MESSAGE(STATUS "Generating translations ...")
IF(SVN_VERSION)
execute_process(COMMAND find ${CMAKE_CURRENT_SOURCE_DIR} -name *.ts COMMAND xargs ${QT_LRELEASE_EXECUTABLE} -silent -nounfinished)
ELSE(SVN_VERSION)
execute_process(COMMAND find ${CMAKE_CURRENT_SOURCE_DIR} -name *.ts COMMAND xargs ${QT_LRELEASE_EXECUTABLE} -silent)
ENDIF(SVN_VERSION)

IF(NOT LIB_SUFFIX)
IF("${QT_LIBRARY_DIR}" MATCHES lib64)
set(LIB_SUFFIX 64)
ENDIF("${QT_LIBRARY_DIR}" MATCHES lib64)
ENDIF(NOT LIB_SUFFIX)

IF(${CMAKE_SYSTEM_NAME} MATCHES Darwin)
	SET(LIB_DIR "qmmp.app/Contents/Frameworks" CACHE STRING "library path")
ELSE()
	SET(LIB_DIR "lib${LIB_SUFFIX}" CACHE STRING "library path")
ENDIF()

SET(USE_SKINNED TRUE CACHE BOOL "enable/disable standard skinned user interface")
SET(USE_DIR_ASSOC TRUE CACHE BOOL "enable/disable inode/directory mime type association")

pkg_search_module(X11 x11)

add_subdirectory(src/qmmp)
add_subdirectory(src/qmmpui)
add_subdirectory(src/plugins)
add_subdirectory(src/app)
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	#Hack for MacOS X to run fix_mac_libs.sh script after installation
	add_subdirectory(src/mac)
ENDIF()

MACRO(PRINT_SUMMARY str USE FOUND)
IF(${USE} AND ${FOUND})
MESSAGE("${str}enabled")
ELSE(${USE} AND ${FOUND})
MESSAGE("${str}disabled")
ENDIF(${USE} AND ${FOUND})
ENDMACRO(PRINT_SUMMARY)

IF (UNIX)
ADD_CUSTOM_TARGET (distclean @echo cleaning for source distribution)

ADD_CUSTOM_COMMAND(
    COMMENT "distribution clean"
    COMMAND make
    ARGS -C ${CMAKE_CURRENT_BINARY_DIR} clean
    COMMAND find
    ARGS ${CMAKE_CURRENT_BINARY_DIR} -name "CMakeCache.txt" | xargs rm -rf
    COMMAND find
    ARGS ${CMAKE_CURRENT_BINARY_DIR} -name "CMakeFiles" | xargs rm -rf
    COMMAND find
    ARGS ${CMAKE_CURRENT_BINARY_DIR} -name "Makefile" | xargs rm -rf
    COMMAND find
    ARGS ${CMAKE_CURRENT_BINARY_DIR} -name \"*.cmake\" | xargs rm -rf
    COMMAND find
    ARGS ${CMAKE_CURRENT_SOURCE_DIR} -name "*.qm" | xargs rm -rf
    COMMAND find
    ARGS ${CMAKE_CURRENT_BINARY_DIR} -name "*.pc" | xargs rm -rf
    COMMAND find
    ARGS ${CMAKE_CURRENT_BINARY_DIR} -name "*.depends" | xargs rm -rf
    COMMAND find
    ARGS ${CMAKE_CURRENT_BINARY_DIR} -name "*.cxx_parameters" | xargs rm -rf
    COMMAND find
    ARGS ${CMAKE_CURRENT_BINARY_DIR} -name "moc_*.cpp" | xargs rm -rf
    COMMAND rm
    ARGS -rf ${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt
    TARGET  distclean
)
ENDIF(UNIX)


MESSAGE("")
MESSAGE("Transports:")
PRINT_SUMMARY ("HTTP support ......................" USE_CURL CURL_FOUND)
PRINT_SUMMARY ("MMS support ......................." USE_MMS MMS_FOUND)

MESSAGE("")
MESSAGE("Input Plugins:")
PRINT_SUMMARY ("mp3 support ......................." USE_MAD  MAD_FOUND)
PRINT_SUMMARY ("FLAC support ......................" USE_FLAC FLAC_FOUND)
PRINT_SUMMARY ("Ogg Vorbis support ................" USE_VORBIS VORBIS_FOUND)
PRINT_SUMMARY ("Musepack support .................." USE_MPC MPC_FOUND)
PRINT_SUMMARY ("MOD support ......................." USE_MODPLUG MODPLUG_FOUND)
PRINT_SUMMARY ("Wave support ......................" USE_SNDFILE SNDFILE_FOUND)
PRINT_SUMMARY ("WavPack support ..................." USE_WAVPACK WAVPACK_FOUND)
PRINT_SUMMARY ("FFmpeg/Libav support .............." USE_FFMPEG FFMPEG_FOUND)
PRINT_SUMMARY ("FFmpeg 0.6-0.8 support ............" USE_FFMPEG_LEGACY FFMPEG_LEGACY_FOUND)
PRINT_SUMMARY ("AAC support ......................." USE_AAC FAAD_FOUND)
PRINT_SUMMARY ("CUE sheet support ................." USE_CUE 1)
PRINT_SUMMARY ("Mplayer support ..................." USE_MPLAYER 1)
PRINT_SUMMARY ("CD Audio support .................." USE_CDA CDIO_FOUND)
PRINT_SUMMARY ("Midi support ......................" USE_MIDI WILDMIDI_FOUND)
PRINT_SUMMARY ("Game music support ................" USE_GME GME_FOUND)
PRINT_SUMMARY ("Ogg Opus support .................." USE_OPUS OPUS_FOUND)
PRINT_SUMMARY ("SID support ......................." USE_SID SIDPLAYFP_FOUND)

MESSAGE("")
MESSAGE("Output Plugins:")
PRINT_SUMMARY ("ALSA support ......................" USE_ALSA ALSA_FOUND)
PRINT_SUMMARY ("OSS support ......................." USE_OSS 1)
PRINT_SUMMARY ("OSS4 support ......................" USE_OSS4 OSS4_FOUND)
PRINT_SUMMARY ("JACK support ......................" USE_JACK JACK_FOUND)
PRINT_SUMMARY ("PulseAudio support ................" USE_PULSE PULSE_FOUND)
PRINT_SUMMARY ("Null output support ..............." USE_NULL 1)
PRINT_SUMMARY ("Win32 Waveout support ............." USE_WAVEOUT WINDOWS_H_FOUND)
PRINT_SUMMARY ("Win32 DirectSound support ........." USE_DSOUND DSOUND_H_FOUND)
PRINT_SUMMARY ("Win32 WASAPI support .............." USE_WASAPI WASAPI_FOUND)
PRINT_SUMMARY ("Qt Multimedia support ............." USE_QTMULTIMEDIA QT_QTMULTIMEDIA_FOUND)

MESSAGE("")
MESSAGE("Effect Plugins:")
PRINT_SUMMARY ("SoX Resampler ....................." USE_SOXR SOXR_FOUND)
PRINT_SUMMARY ("BS2B Plugin ......................." USE_BS2B BS2B_FOUND)
PRINT_SUMMARY ("LADSPA Host ......................." USE_LADSPA 1)
PRINT_SUMMARY ("Crossfade ........................." USE_CROSSFADE 1)
PRINT_SUMMARY ("Extra Stereo ......................" USE_STEREO 1)

MESSAGE("")
MESSAGE("Visual Plugins:")
PRINT_SUMMARY ("Analyzer .........................." USE_ANALYZER 1)
PRINT_SUMMARY ("ProjectM .........................." USE_PROJECTM PROJECTM_FOUND)

MESSAGE("")
MESSAGE("General Plugins:")
PRINT_SUMMARY ("MPRIS support ....................." USE_MPRIS 1)
PRINT_SUMMARY ("Scrobbler ........................." USE_SCROBBLER 1)
PRINT_SUMMARY ("Tray support ......................" USE_STATICON 1)
PRINT_SUMMARY ("Notifier support .................." USE_NOTIFIER 1)
PRINT_SUMMARY ("Lyrics support ...................." USE_LYRICS 1)
PRINT_SUMMARY ("HAL support ......................." USE_HAL 1)
PRINT_SUMMARY ("UDisks2 support ..................." USE_UDISKS2 1)
PRINT_SUMMARY ("UDisks support ...................." USE_UDISKS 1)
PRINT_SUMMARY ("Global hotkey support ............." USE_HOTKEY X11_FOUND)
PRINT_SUMMARY ("GNOME/Cinnamon hotkey support ....." USE_GNOMEHOTKEY 1)
PRINT_SUMMARY ("File operations ..................." USE_FILEOPS 1)
PRINT_SUMMARY ("Cover manager ....................." USE_COVER 1)
PRINT_SUMMARY ("KDE4 notifications support ........" USE_KDENOTIFY 1)
PRINT_SUMMARY ("Audio converter ..................." USE_CONVERTER TAGLIB_FOUND)
PRINT_SUMMARY ("ReplayGain Scanner ................" USE_RGSCAN TAGLIB_FOUND)
PRINT_SUMMARY ("Stream browser ...................." USE_SB 1)
PRINT_SUMMARY ("Track change ......................" USE_TRACKCHANGE 1)
PRINT_SUMMARY ("Copy/Paste ........................" USE_COPYPASTE 1)

MESSAGE("")
MESSAGE("File Dialogs:")
PRINT_SUMMARY ("QMMP File Dialog .................." USE_QMMP_DIALOG 1)
PRINT_SUMMARY ("Two-panel File File Dialog ........" USE_TWO_PANEL_DIALOG 1)

MESSAGE("")
MESSAGE("Advanced:")
PRINT_SUMMARY ("Skinned user interface ............" USE_SKINNED X11_FOUND)
PRINT_SUMMARY ("Simple user interface (QSUI) ......" USE_QSUI 1)
PRINT_SUMMARY ("Automatic charset detection ......." USE_ENCA ENCA_FOUND)
PRINT_SUMMARY ("Directory association ............." USE_DIR_ASSOC 1)
PRINT_SUMMARY ("Set RPATH in installed binaries ..." CMAKE_INSTALL_RPATH_USE_LINK_PATH 1)

MESSAGE("")
