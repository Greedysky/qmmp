#Hack for MacOS X to run fix_mac_libs.sh script after installation
install(CODE "execute_process(COMMAND ${CMAKE_SOURCE_DIR}/utils/fix_mac_libs.sh ${CMAKE_INSTALL_PREFIX}/qmmp.app)")
