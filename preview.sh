#! /bin/bash
# make from photo 340x240 preview with preffered file-size(in Kb)
# parameter: photo file-size(Kb)

src=$1
dst=prev-$src
pref_size=$(($2 * 1024))

# resize

echo "-- Resizing: 340x240, 80%"

qual=80
./stb-resize -W 340 -H 240 -q $qual $src $dst
size=$(stat -c '%s' $dst)

echo "-- File size: $size b"

# decrease quality until size will be good

while [ $size -gt $pref_size ]
do
    qual=$(( $qual - 10 ))

    if [ $qual -lt 50 ]
    then
        break
    fi

    echo "-- Decrease quality: $qual %"
    ./stb-resize -q $qual $dst $dst

    size=$(stat -c '%s' $dst)
    echo "-- File size: $size b"
done