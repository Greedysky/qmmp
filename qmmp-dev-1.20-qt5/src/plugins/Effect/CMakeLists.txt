SET(USE_SOXR TRUE CACHE BOOL "enable/disable SoX resampler plugin")
SET(USE_BS2B TRUE CACHE BOOL "enable/disable bs2b plugin")
SET(USE_LADSPA TRUE CACHE BOOL "enable/disable ladspa plugin")
SET(USE_CROSSFADE TRUE CACHE BOOL "enable/disable crossfade plugin")
SET(USE_STEREO TRUE CACHE BOOL "enable/disable extra stereo plugin")

IF(USE_SOXR)
add_subdirectory(soxr)
ENDIF(USE_SOXR)

IF(USE_BS2B)
add_subdirectory(bs2b)
ENDIF(USE_BS2B)

IF(USE_LADSPA)
add_subdirectory(ladspa)
ENDIF(USE_LADSPA)

IF(USE_CROSSFADE)
add_subdirectory(crossfade)
ENDIF(USE_CROSSFADE)

IF(USE_STEREO)
add_subdirectory(stereo)
ENDIF(USE_STEREO)
