cmake_minimum_required(VERSION 3.16)
project(SDL3Demo CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-g -fopenmp)

include_directories(
    C:/tools/SDL3-3.2.14/x86_64-w64-mingw32/include
)
link_directories(
    C:/tools/SDL3-3.2.14/x86_64-w64-mingw32/lib
)

set(SRC_DIRS
    src
    src/vendor/imgui
    src/vendor/lodepng
    src/objects
    src/effects
    src/backgrounds
)
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${SRC_DIRS}/*.cpp
)

add_executable(demo ${SRC_FILES})
target_link_libraries(demo PRIVATE mingw32 SDL3)

