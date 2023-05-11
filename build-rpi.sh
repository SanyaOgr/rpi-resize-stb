mkdir build 
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=rpi-toolchain.cmake -DCMAKE_BUILD_TYPE=Release ..
make