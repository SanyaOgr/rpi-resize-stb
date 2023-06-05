# rpi-resize-stb
Raspberry PI utility for image resizing


## Build

### Clone repo
```shell
$ git clone git@github.com:SanyaOgr/stb-resize-rpi.git --recursive
```

### Install dependencies

Build dependencies:
```shell
$ sudo apt-get install build-essential cmake
```
Cross-compiling toolchain: https://github.com/SanyaOgr/rpi-crosscompile-template#setup

### Compilation

```shell
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=rpi-toolchain.cmake ..
$ cmake --build .
```


## Usage

The first and second parameters specify the paths to the image and the result. The `-H`, `-W` and `-r` options set the resulting dimensions. If `-H` or `-W` is given, then `-r` is ignored. The `-q` option can be used with saving to `.jpg` or `.jpeg`, it sets quality of the output image in percents.

```shell
./stbresize -r 4 -q 90 image_in.jpg image_out.jpg
```


## Structure

- `dependencies.c` - API [stb](https://github.com/nothings/stb.git)
- `stb/` - [stb](https://github.com/nothings/stb.git)
- `stbresize.c` - CLI program.
- `scripts/preview.sh` - Script for making photo previews on RPi which works on satellite.
