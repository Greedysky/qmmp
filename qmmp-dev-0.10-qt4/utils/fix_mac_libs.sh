#!/bin/bash

#This script changes paths in installed binaries to relative. The first argument is path to .app package.

echo Fixing paths to libraries inside $1...

FINDDIR=$1/Contents
IFS=$'\n'
for i in `find $FINDDIR -type f -name \*.dylib -or -type f -name \*.so -or -type f -name qmmp` ; do
    install_name_tool -change libqmmp.0.dylib @executable_path/../Frameworks/libqmmp.0.dylib $i
    install_name_tool -change libqmmpui.0.dylib @executable_path/../Frameworks/libqmmpui.0.dylib $i
done

echo Fixing paths to libraries done.
