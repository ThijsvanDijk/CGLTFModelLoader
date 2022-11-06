# Get number of processing cores
NOFPROC=`expr $(nproc) - 1`
mkdir build
cmake -B build/ -S ./
cmake --build build/ --config Release -j$NOFPROC
./build/cgltf