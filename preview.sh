#! /bin/bash
# make from photo 340x240 preview with preffered file-size(in Kb)
# parameter: photo file-size(Kb)

./stb-resize -W 340 -H 240 -q 60 $1 prev-$1