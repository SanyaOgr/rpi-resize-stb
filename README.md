# rpi-resize-stb
Raspberry PI utility for image resizing
- Source: https://github.com/ImageProcessing-ElectronicPublications/stb-image-resize.git


## Functionality

The image is scaled using interpolation:
- bicubic (4x4)

Include prefilter:
- Gauss

Added features:
- Saving into `.jpg` and `.jpeg` format


## Build

### Load submodules

Submodules:
- [stb](https://github.com/nothings/stb.git) -> [src/stb](src/stb)

```shell
$ git submodule init
$ git submodule update
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

- `bicubic.h` - bicubic image scaling
- `dependencies.c` - API [stb](https://github.com/nothings/stb.git)
- `gauss.h` - prefilter: Gauss
- `stb/` - [stb](https://github.com/nothings/stb.git)
- `stbresize.c` - CLI program.
