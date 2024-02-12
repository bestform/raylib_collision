cmake_minimum_required(VERSION 3.27)
project(collision C)

set(CMAKE_C_STANDARD 23)

include_directories(c:/raylib/raylib/cmake-build-debug/raylib/include)

link_directories(c:/raylib/raylib/cmake-build-debug/raylib)

link_libraries(
        raylib.a
        OpenGL32
        GDI32
        WinMM
)

add_executable(collision main.c
        lib/poly.c
        lib/poly.h
        external/raygui.h
        lib/raycast.c
        lib/raycast.h
        lib/character.c
        lib/character.h)
