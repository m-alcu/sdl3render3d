# Makefile for building SDL3-based project

# Compiler and flags
CXX := C:/msys64/ucrt64/bin/g++.exe
CXXFLAGS := -g -std=c++20 -fopenmp -IC:/tools/SDL3-3.2.14/x86_64-w64-mingw32/include
LDFLAGS := -LC:/tools/SDL3-3.2.14/x86_64-w64-mingw32/lib -lmingw32 -lSDL3 -mwindows

# Output binary
TARGET := build/demo.exe

# Source files
SRC := \
    src/main.cpp \
    src/backgrounds/backgroundFactory.cpp \
    src/backgrounds/desert.cpp \
    src/backgrounds/imagepng.cpp \
    src/backgrounds/twister.cpp \
    src/objects/solid.cpp \
    src/objects/cube.cpp \
    src/objects/test.cpp \
    src/objects/tetrakis.cpp \
    src/objects/torus.cpp \
    src/objects/amiga.cpp \
    src/objects/world.cpp \
    src/objects/ascLoader.cpp \
    src/objects/objLoader.cpp \
    src/slib.cpp \
    src/smath.cpp \
    src/scene.cpp \
    src/vendor/lodepng/lodepng.cpp \
    src/vendor/imgui/imgui_impl_sdlrenderer3.cpp \
    src/vendor/imgui/imgui_tables.cpp \
    src/vendor/imgui/imgui_widgets.cpp \
    src/vendor/imgui/imgui.cpp \
    src/vendor/imgui/imgui_draw.cpp \
    src/vendor/imgui/imgui_impl_sdl3.cpp

# Default rule
all: $(TARGET)

# Build rule
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Clean rule
clean:
	del /Q build\*.exe
