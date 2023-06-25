cmake_minimum_required(VERSION 3.0)

project(stb-resize)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(stb-resize src/main.c src/stb_image.h src/stb_image_resize.h src/stb_image_write.h)

target_link_libraries(stb-resize m)

target_compile_options(stb-resize PRIVATE -Wall -Wextra -Werror -pedantic)

install(TARGETS stb-resize RUNTIME DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/scripts/preview.sh DESTINATION bin)