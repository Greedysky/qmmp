SET(USE_MPRIS TRUE CACHE BOOL "enable/disable mpris plugin")
SET(USE_SCROBBLER TRUE CACHE BOOL "enable/disable scrobbler plugin")
SET(USE_STATICON TRUE CACHE BOOL "enable/disable status icon plugin")
SET(USE_NOTIFIER TRUE CACHE BOOL "enable/disable notifier plugin")
SET(USE_LYRICS TRUE CACHE BOOL "enable/disable lyrics version")
SET(USE_HAL (${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD") CACHE BOOL "enable/disable hal plugin")
SET(USE_UDISKS2 TRUE CACHE BOOL "enable/disable udisks2 plugin")
SET(USE_HOTKEY TRUE CACHE BOOL "enable/disable global hotkey plugin")
SET(USE_GNOMEHOTKEY TRUE CACHE BOOL "enable/disable gnome hotkey plugin")
SET(USE_FILEOPS TRUE CACHE BOOL "enable/disable fileops plugin")
SET(USE_COVER TRUE CACHE BOOL "enable/disable cover manager plugin")
SET(USE_KDENOTIFY TRUE CACHE BOOL "enable/disable kde notification plugin")
SET(USE_CONVERTER TRUE CACHE BOOL "enable/disable audio converter plugin")
SET(USE_RGSCAN TRUE CACHE BOOL "enable/disable replaygain scanner plugin")
SET(USE_SB TRUE CACHE BOOL "enable/disable stream browser plugin")
SET(USE_TRACKCHANGE TRUE CACHE BOOL "enable/disable track change plugin")
SET(USE_COPYPASTE TRUE CACHE BOOL "enable/disable copy/paste change plugin")

IF(USE_MPRIS AND Qt5DBus_FOUND)
add_subdirectory(mpris)
ENDIF(USE_MPRIS AND Qt5DBus_FOUND)

IF(USE_SCROBBLER)
add_subdirectory(scrobbler)
ENDIF(USE_SCROBBLER)

IF(USE_STATICON)
add_subdirectory(statusicon)
ENDIF(USE_STATICON)

IF(USE_NOTIFIER)
add_subdirectory(notifier)
ENDIF(USE_NOTIFIER)

IF(USE_LYRICS)
add_subdirectory(lyrics)
ENDIF(USE_LYRICS)

IF(USE_HAL AND Qt5DBus_FOUND)
add_subdirectory(hal)
ENDIF(USE_HAL AND Qt5DBus_FOUND)

IF(USE_UDISKS2 AND Qt5DBus_FOUND)
add_subdirectory(udisks2)
ENDIF(USE_UDISKS2 AND Qt5DBus_FOUND)

IF(USE_HOTKEY AND Qt5X11Extras_FOUND)
add_subdirectory(hotkey)
ENDIF(USE_HOTKEY AND Qt5X11Extras_FOUND)

IF(USE_FILEOPS)
add_subdirectory(fileops)
ENDIF(USE_FILEOPS)

IF(USE_COVER)
add_subdirectory(covermanager)
ENDIF(USE_COVER)

IF(USE_KDENOTIFY AND Qt5DBus_FOUND)
add_subdirectory(kdenotify)
ENDIF(USE_KDENOTIFY AND Qt5DBus_FOUND)

IF(USE_CONVERTER AND TAGLIB_FOUND)
add_subdirectory(converter)
ENDIF(USE_CONVERTER AND TAGLIB_FOUND)

IF(USE_RGSCAN AND TAGLIB_FOUND)
add_subdirectory(rgscan)
ENDIF(USE_RGSCAN AND TAGLIB_FOUND)

IF(USE_SB)
add_subdirectory(streambrowser)
ENDIF(USE_SB)

IF(USE_TRACKCHANGE)
add_subdirectory(trackchange)
ENDIF(USE_TRACKCHANGE)

IF(USE_COPYPASTE)
add_subdirectory(copypaste)
ENDIF(USE_COPYPASTE)

IF(USE_GNOMEHOTKEY AND Qt5DBus_FOUND)
add_subdirectory(gnomehotkey)
ENDIF(USE_GNOMEHOTKEY AND Qt5DBus_FOUND)
