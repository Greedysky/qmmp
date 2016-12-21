SET(USE_QMMP_DIALOG TRUE CACHE BOOL "enable/disable Qmmp file dialog")
SET(USE_TWO_PANEL_DIALOG TRUE CACHE BOOL "enable/disable Two-panel file dialog")

IF(USE_QMMP_DIALOG)
add_subdirectory(QmmpFileDialog)
ENDIF(USE_QMMP_DIALOG)

IF(USE_TWO_PANEL_DIALOG)
add_subdirectory(TwoPanelFileDialog)
ENDIF(USE_TWO_PANEL_DIALOG)
