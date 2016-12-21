SET(USE_MAD            TRUE CACHE BOOL "enable/disable mad plugin")
SET(USE_FLAC           TRUE CACHE BOOL "enable/disable flac plugin")
SET(USE_VORBIS         TRUE CACHE BOOL "enable/disable ogg vorbis plugin")
SET(USE_FFMPEG         TRUE CACHE BOOL "enable/disable ffmpeg plugin")
SET(USE_MPC            TRUE CACHE BOOL "enable/disable mpc plugin")
SET(USE_SNDFILE        TRUE CACHE BOOL "enable/disable sndfile plugin")
SET(USE_WAVPACK        TRUE CACHE BOOL "enable/disable wavpack plugin")
SET(USE_MODPLUG        TRUE CACHE BOOL "enable/disable modplug plugin")
SET(USE_AAC            TRUE CACHE BOOL "enable/disable aac plugin")
SET(USE_CUE            TRUE CACHE BOOL "enable/disable cue plugin")
SET(USE_CDA            TRUE CACHE BOOL "enable/disable cd audio support")
SET(USE_MIDI           TRUE CACHE BOOL "enable/disable midi support")
SET(USE_GME            TRUE CACHE BOOL "enable/disable game music plugin")
SET(USE_OPUS           TRUE CACHE BOOL "enable/disable opus plugin")
SET(USE_SID            TRUE CACHE BOOL "enable/disable sid plugin")
SET(USE_ARCHIVE        TRUE CACHE BOOL "enable/disable archive plugin")


IF(USE_MAD AND TAGLIB_FOUND)
add_subdirectory(mad)
ENDIF(USE_MAD AND TAGLIB_FOUND)

IF(USE_FLAC AND TAGLIB_FOUND)
add_subdirectory(flac)
ENDIF(USE_FLAC AND TAGLIB_FOUND)

IF(USE_VORBIS AND TAGLIB_FOUND)
add_subdirectory(vorbis)
ENDIF(USE_VORBIS AND TAGLIB_FOUND)

IF(USE_FFMPEG)
add_subdirectory(ffmpeg)
ENDIF(USE_FFMPEG)

IF(USE_MPC AND TAGLIB_FOUND)
add_subdirectory(mpc)
ENDIF(USE_MPC AND TAGLIB_FOUND)

IF(USE_SNDFILE)
add_subdirectory(sndfile)
ENDIF(USE_SNDFILE)

IF(USE_WAVPACK)
add_subdirectory(wavpack)
ENDIF(USE_WAVPACK)

IF(USE_MODPLUG)
add_subdirectory(modplug)
ENDIF(USE_MODPLUG)

IF(USE_AAC AND TAGLIB_FOUND)
add_subdirectory(aac)
ENDIF(USE_AAC AND TAGLIB_FOUND)

IF(USE_CUE)
add_subdirectory(cue)
ENDIF(USE_CUE)

IF(USE_CDA)
add_subdirectory(cdaudio)
ENDIF(USE_CDA)

IF(USE_MIDI)
add_subdirectory(wildmidi)
ENDIF(USE_MIDI)

IF(USE_GME)
add_subdirectory(gme)
ENDIF(USE_GME)

IF(USE_OPUS AND TAGLIB_FOUND)
add_subdirectory(opus)
ENDIF(USE_OPUS AND TAGLIB_FOUND)

IF(USE_SID)
add_subdirectory(sid)
ENDIF(USE_SID)

IF(USE_ARCHIVE AND TAGLIB_FOUND AND (NOT (TAGLIB_VERSION VERSION_LESS 1.11)))
add_subdirectory(archive)
ENDIF(USE_ARCHIVE AND TAGLIB_FOUND AND (NOT (TAGLIB_VERSION VERSION_LESS 1.11)))
