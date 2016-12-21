#!/bin/sh

cd ../../../trunk/qmmp
find -name '*.ts' -not -path "*/udisks/*" -not -path "*/ffmpeg_legacy/*"  -print0 | xargs --null cp --parents -t ../../branches/qmmp-1.1 -v
find -name '*.txt' -not -path "*/CMakeLists.txt" -print0 | xargs --null cp --parents -t ../../branches/qmmp-1.1 -v
#find -name '*.desktop' -print0 | xargs --null cp --parents -t ../../branches/qmmp-1.1 -v
