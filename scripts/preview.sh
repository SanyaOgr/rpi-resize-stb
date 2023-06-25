#! /bin/bash
# make 340x240 preview and 1024x768 photo with preffered file sizes(in Kb)
# parameters: [photo number] [preview file size] [photo file size]

num=$1
src_dir="$HOME/photos"
src_name="$num.jpg"
dst_dir="$HOME/thumbs"
prev_name="$num-prev.jpg"
photo_name="$num-main.jpg"
prev_targz_name="$num-prev.tar.gz"
photo_targz_name="$num-main.tar.gz"

src_path="$src_dir/$src_name"
prev_path="$dst_dir/$prev_name"
photo_path="$dst_dir/$photo_name"

prev_good_size=$(($2 * 1024))
photo_good_size=$(($3 * 1024))

# resize

prev_qual=80
photo_qual=80

# echo "-- Resizing: 340x240, 80%"
stb-resize -W 340 -H 240 -q $prev_qual $src_path $prev_path > /dev/null

# echo "-- Resizing: 1024x768, 80%"
stb-resize -W 1024 -H 768 -q $photo_qual $src_path $photo_path > /dev/null

prev_size=$(stat -c '%s' $prev_path)
photo_size=$(stat -c '%s' $photo_path)

# echo "-- Preview size: $prev_size b"
# echo "-- Photo size: $photo_size b"

# decrease preview quality until size will be good

while [ $prev_size -gt $prev_good_size ] && [ $prev_qual -gt 50 ]
do
    prev_qual=$(( $prev_qual - 10 ))

    # echo "-- Decrease preview quality: $prev_qual %"
    stb-resize -q $prev_qual $prev_path $prev_path > /dev/null

    prev_size=$(stat -c '%s' $prev_path)
    # echo "-- Preview size: $prev_size b"
done

# decrease photo quality until size will be good

while [ $photo_size -gt $photo_good_size ] && [ $photo_qual -gt 50 ]
do
    photo_qual=$(( $photo_qual - 10 ))

    # echo "-- Decrease photo quality: $photo_qual %"
    stb-resize -q $photo_qual $photo_path $photo_path > /dev/null

    photo_size=$(stat -c '%s' $photo_path)
    # echo "-- Photo size: $photo_size b"
done

cd $dst_dir
tar -zcvf $prev_targz_name $prev_name > /dev/null
tar -zcvf $photo_targz_name $photo_name > /dev/null

echo "-- Preview archive: $prev_targz_name (340x240 $prev_qual % $(($prev_size/1024)) Kb)"
echo "-- Photo archive: $photo_targz_name (1024x768 $photo_qual % $(($photo_size/1024)) Kb)"