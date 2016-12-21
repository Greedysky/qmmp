SET(USE_SKINNED TRUE CACHE BOOL "enable/disable skinned user interface")
SET(USE_QSUI TRUE CACHE BOOL "enable/disable simple user interface (qsui)")

IF(USE_SKINNED AND Qt5X11Extras_FOUND)
add_subdirectory(skinned)
ENDIF(USE_SKINNED AND Qt5X11Extras_FOUND)

IF(USE_QSUI)
add_subdirectory(qsui)
ENDIF(USE_QSUI)
