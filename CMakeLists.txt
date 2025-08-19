cmake_minimum_required(VERSION 3.10)
project(PixelRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PixelRenderer main.cpp
        engine.cpp)

# Link against GDI32 for StretchDIBits and related Win32 drawing
target_link_libraries(PixelRenderer PRIVATE gdi32)
