mkdir build
CC=C:\msys64\mingw64\bin\gcc.exe
CXX=C:\msys64\mingw64\bin\g++.exe 
cmake -G "MinGW Makefiles" -B build/ -S ./
cmake --build build/ --config Release
.\build\CGLTFModelLoader.exe