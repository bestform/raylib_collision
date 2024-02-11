# Raylib Collision

This is a toy project that implements collision detection between arbitrary convex polygons.
It also supports ray casting.

In its current state it is a windows build of a raylib project with an example application using the library.
In the future I intend to release this as a standalone library that can be used in any raylib project.

## Build

To build this project you need to modify `CMakeLists.txt` to fit your environment. The main parts to modify are
`include_directories` and `link_directories`. 

This is currently a windows build, but it should work on linux and OSX if you adapt the `CMakeLists.txt` further.

## external code

I've included a copy of `raylibui.h` in this project. It is not needed for the library, but it is used for the
demo application. The code in `raylibui.h` is not mine. It is meant to be copied to projects using it, so I did just that.