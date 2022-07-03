mkdir build 
cmake -G "MinGW Makefiles" -B build/ -S ./
cmake --build build/ --config Release
.\build\CGLTFModelLoader.exe