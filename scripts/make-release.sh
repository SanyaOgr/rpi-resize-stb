#! /bin/bash
# commpress stb-resize and preview.sh into release/preview.tar.gz
# copy install-rpi.sh -> release/

mkdir -p release
cp ./build/stb-resize ./release/
cp ./scripts/preview.sh ./release/
cp ./scripts/install-rpi.sh ./release/
cd release
tar -zcvf preview.tar.gz stb-resize preview.sh && rm stb-resize preview.sh