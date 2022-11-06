cmake_minimum_required(VERSION 3.15)
project(SkiaDemo)

set(CMAKE_CXX_STANDARD 14)

include_directories( 3rdPart/include
        3rdPart/include/android
        3rdPart/include/atlastext
        3rdPart/include/c
        3rdPart/include/codec
        3rdPart/include/config
        3rdPart/include/core
        3rdPart/include/effects
        3rdPart/include/encode
        3rdPart/include/gpu
        3rdPart/include/gpu/gl
        3rdPart/include/gpu/mock
        3rdPart/include/gpu/mtl
        3rdPart/include/gpu/vk
        3rdPart/include/pathops
        3rdPart/include/ports
        3rdPart/include/private
        3rdPart/include/svg
        3rdPart/include/utils
        3rdPart/include/utils/mac
        3rdPart/include/views)
link_directories( 3rdPart/lib/linux/x64/Shared)

add_executable(SkiaDemo main.cpp)

target_link_libraries(SkiaDemo skia)
target_link_libraries(SkiaDemo SDL2)
