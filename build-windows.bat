mkdir build 
cmake -G "MinGW Makefiles" -B build/ -S ./
cmake --build build/ --config Release -j8
.\build\cgltf.exe