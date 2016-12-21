CONFIG += ordered

include (qmmp.pri)

TEMPLATE = subdirs
SUBDIRS = src/qmmp src/qmmpui src/plugins src/app


unix:exists($$[QT_INSTALL_BINS]/lrelease){
LRELEASE_EXECUTABLE = $$[QT_INSTALL_BINS]/lrelease
}

unix:exists($$[QT_INSTALL_BINS]/lrelease-qt5){
LRELEASE_EXECUTABLE = $$[QT_INSTALL_BINS]/lrelease-qt5
}


win32:exists($$[QT_INSTALL_BINS]/lrelease.exe){
LRELEASE_EXECUTABLE = $$[QT_INSTALL_BINS]/lrelease.exe
}


isEmpty(LRELEASE_EXECUTABLE){
error(Could not find lrelease executable)
}
else {
message(Found lrelease executable: $$LRELEASE_EXECUTABLE)
}

message(generating translations)
unix:system(find . -name *.ts | xargs $$LRELEASE_EXECUTABLE -compress)
win32:system(for /r %B in (*.ts) do $$LRELEASE_EXECUTABLE -compress %B)

message("******* BUILD CONFIGURATION *******")
for(out, CONFIG):message($$out)
message("*** END OF BUILD CONFIGURATION ****")
