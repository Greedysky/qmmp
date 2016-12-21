include(../../../qmmp.pri)
TEMPLATE = subdirs


contains(CONFIG, WITH_SKINNED){
SUBDIRS += skinned
}

contains(CONFIG, WITH_QSUI){
SUBDIRS += qsui
}
