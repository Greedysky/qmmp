TEMPLATE = subdirs
unix:SUBDIRS = IncDecVolumeOption \
               SeekOption \
               StatusOption \
               PlayListOption

win32:SUBDIRS += UninstallOption
