# stb-resize
Utility for resize images, based on stb_image lib. Developed for work on satellite's Raspberry Pi


## Setup

### Clone repo
```shell
$ git clone git@github.com:zdrvzbl/stb-resize.git
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

The first and second parameters specify the paths to the image and the result. The `-H`, `-W` and `-r` options set the resulting dimensions. If `-H` and `-W` is given, then `-r` is ignored. `-q` option sets quality of output image in percents

```shell
./stb-resize -r 4 -q 90 image_in.jpg image_out.jpg
```


## Structure

- `stb_{name}` - [stb_image](https://github.com/nothings/stb.git) headers
- `main.c` - CLI program
- `scripts/preview.sh` - Script for making photo previews on satellite's RPi
