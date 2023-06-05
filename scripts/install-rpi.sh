#! /bin/bash
# extract preview program and move it to /usr/local/bin

tar -xf preview.tar.gz
mkdir -p ~/bin
chmod +x stb-resize
chmod +x preview.sh
mv ./stb-resize ~/bin/ -v
mv ./preview.sh ~/bin/ -v