SET(USE_ANALYZER TRUE CACHE BOOL "enable/disable analyzer plugin")
SET(USE_PROJECTM TRUE CACHE BOOL "enable/disable projectm plugin")

IF(USE_ANALYZER)
add_subdirectory(analyzer)
ENDIF(USE_ANALYZER)

IF(USE_PROJECTM)
add_subdirectory(projectm)
ENDIF(USE_PROJECTM)
