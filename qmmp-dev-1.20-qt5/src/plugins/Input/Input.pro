include(../../../qmmp.pri)

TEMPLATE = subdirs
SUBDIRS += mad vorbis cue sndfile wavpack

contains(CONFIG, FLAC_PLUGIN){
    SUBDIRS += flac
}

contains(CONFIG, MUSEPACK_PLUGIN){
    SUBDIRS += mpc
}

contains(CONFIG, MODPLUG_PLUGIN){
    SUBDIRS += modplug
}

contains(CONFIG, FFMPEG_PLUGIN){
    SUBDIRS += ffmpeg
}

contains(CONFIG, GME_PLUGIN){
    SUBDIRS += gme
}

contains(CONFIG, OPUS_PLUGIN){
    SUBDIRS += opus
}

contains(CONFIG, CDAUDIO_PLUGIN){
    SUBDIRS += cdaudio
}

contains(CONFIG, SID_PLUGIN){
    SUBDIRS += sid
}

unix{

contains(CONFIG, AAC_PLUGIN){
    SUBDIRS += aac
}


contains(CONFIG, WILDMIDI_PLUGIN){
    SUBDIRS += wildmidi
}

contains(CONFIG, ARCHIVE_PLUGIN){
  TAGLIB_VERSION = $$system("pkg-config --modversion taglib")
  TAGLIB_VERSION = $$split(TAGLIB_VERSION, ".")
  TAGLIB_VER_MAJ = $$member(TAGLIB_VERSION, 0)
  TAGLIB_VER_MIN = $$member(TAGLIB_VERSION, 1)

  greaterThan(TAGLIB_VER_MAJ, 1) | equals(TAGLIB_VER_MAJ, 1) {
    greaterThan(TAGLIB_VER_MIN, 10):SUBDIRS += archive
  } else {
    message("Archive plugin requires at least TagLib 1.11")
  }
}

}
