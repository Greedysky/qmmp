#!/bin/sh

###################################################
# Removes remaining qmake files from qmmp package #
###################################################

make distclean
rm -rf lib
find . -type f -name *.qm -delete
find . -type d -name ".build" | xargs rm -rf
rm -rf src/qmmpui/pkgconfig
rm -rf src/qmmp/pkgconfig
