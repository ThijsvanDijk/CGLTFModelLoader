echo "------- Setup -------"

mkdir lib
NOFPROC=`expr $(nproc) - 1`

cd submodules

echo "------- Building GLFW LIBRARY -------"

mkdir glfw/build
cmake -B glfw/build/ -S glfw/ -DBUILD_SHARED_LIBS=ON -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF
cmake --build glfw/build/ --config Release -j$NOFPROC
cp glfw/build/src/libglfw.so* ../lib/

echo "------- Done Building GLFW LIBRARY -------"

cd ../

echo "------- Building Executable -------"

mkdir build
cmake -B build/ -S ./
cmake --build build/ --config Release -j$NOFPROC

echo "------- Finished Building -------"