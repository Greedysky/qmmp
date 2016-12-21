SET(USE_CURL TRUE CACHE BOOL "enable/disable curl-based http plugin")
SET(USE_MMS TRUE CACHE BOOL "enable/disable mms plugin")

IF(USE_CURL)
add_subdirectory(http)
ENDIF(USE_CURL)

IF(USE_MMS)
add_subdirectory(mms)
ENDIF(USE_MMS)
