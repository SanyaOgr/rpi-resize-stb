# stb-resize
Utility for resize images, based on stb_image lib. Developed for work on satellite's Raspberry Pi


## Setup

### Clone repo
```shell
$ git clone git@github.com:SanyaOgr/stb-resize-rpi.git
```

### Install dependencies

Build dependencies:
```shell
$ sudo apt install build-essential cmake
```
Cross-compiling toolchain if you need: https://github.com/zdrvzbl/rpi-crosscompile-template#setup

### Configure

```shell
$ cmake --preset <linux-default | linux-arm> .

```
### Build

```shell
$ cmake --build <build/default | build/rpi>
```

### Install

```shell
$ cmake --install <build/default | build/rpi>
```

CMake install command copies binary and script to `~/bin` or `{projectDir}/rpi-install/bin` in depend of chosen preset

## Usage

The first and second parameters specify the paths to the image and the result. The `-H`, `-W` and `-r` options set the resulting dimensions. If `-H` or `-W` is given, then `-r` is ignored. The `-q` option can be used with saving to `.jpg` or `.jpeg`, it sets quality of the output image in percents.

```shell
./stbresize -r 4 -q 90 image_in.jpg image_out.jpg
```


## Structure

- `stb_{name}` - [stb_image headers](https://github.com/nothings/stb.git)
- `main.c` - CLI program
- `scripts/preview.sh` - Script for making photo previews on satellite's RPi
