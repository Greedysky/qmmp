TEMPLATE = subdirs

SUBDIRS += Input \
           Output \
           General \
           Visual \
           Transports \
           Effect \
           PlayListFormats \
           FileDialogs \
           Ui \
           CommandLineOptions

unix:SUBDIRS += Engines

