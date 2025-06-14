Antes de compilar

Crear directorio build y copiar SDL3.dll en el directorio (procede de SDL3)

Para compilar:

mingw32-make

cmake
=====

cd build

cmake .. -DCMAKE_TOOLCHAIN_FILE=../mingw-toolchain.cmake -G "MinGW Makefiles"


cmake .. -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=../mingw-toolchain.cmake
cmake --build .