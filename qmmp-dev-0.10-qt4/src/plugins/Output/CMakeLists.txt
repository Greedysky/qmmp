SET(USE_ALSA TRUE CACHE BOOL "enable/disable alsa plugin")
SET(USE_JACK TRUE CACHE BOOL "enable/disable jack plugin")
SET(USE_OSS FALSE CACHE BOOL "enable/disable oss plugin")
SET(USE_OSS4 TRUE CACHE BOOL "enable/disable oss4 plugin")
SET(USE_PULSE TRUE CACHE BOOL "enable/disable pulse audio plugin")
SET(USE_NULL TRUE CACHE BOOL "enable/disable null output plugin")
SET(USE_WAVEOUT TRUE CACHE BOOL "enable/disable Win32 waveout plugin")
SET(USE_DSOUND TRUE CACHE BOOL "enable/disable DirectSound plugin")
SET(USE_QTMULTIMEDIA TRUE CACHE BOOL "enable/disable Qt Multimedia output plugin")
SET(USE_WASAPI TRUE CACHE BOOL "enable/disable WASAPI plugin")

IF(USE_ALSA)
add_subdirectory(alsa)
ENDIF(USE_ALSA)

IF(USE_JACK)
add_subdirectory(jack)
ENDIF(USE_JACK)

IF(USE_OSS)
add_subdirectory(oss)
ENDIF(USE_OSS)

IF(USE_OSS4)
add_subdirectory(oss4)
ENDIF(USE_OSS4)

IF(USE_PULSE)
add_subdirectory(pulseaudio)
ENDIF(USE_PULSE)

IF(USE_NULL)
add_subdirectory(null)
ENDIF(USE_NULL)

IF(USE_WAVEOUT)
add_subdirectory(waveout)
ENDIF(USE_WAVEOUT)

IF(USE_DSOUND)
add_subdirectory(directsound)
ENDIF(USE_DSOUND)

IF(USE_QTMULTIMEDIA)
add_subdirectory(qtmultimedia)
ENDIF(USE_QTMULTIMEDIA)

IF(USE_WASAPI)
add_subdirectory(wasapi)
ENDIF(USE_WASAPI)
