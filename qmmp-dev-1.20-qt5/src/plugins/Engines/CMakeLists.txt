SET(USE_MPLAYER TRUE CACHE BOOL "enable/disable mplayer support")

IF(USE_MPLAYER)
add_subdirectory(mplayer)
ENDIF(USE_MPLAYER)
