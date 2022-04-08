echo "------- Setup -------"

mkdir lib
@REM Need to add ProcessorCount here
cd submodules

echo "------- Building GLFW LIBRARY -------"

mkdir glfw/build
cmake -B glfw/build/ -S glfw/ -G "MinGW Makefiles" -DBUILD_SHARED_LIBS=ON -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF
cmake --build glfw/build/ --config Release -j7
cp glfw/build/src/*.dll lib/

echo "------- Done Building GLFW LIBRARY -------"

cd ../

echo "------- Building Executable -------"

mkdir build
cmake -B build/ -S ./ -G "MinGW Makefiles"
cmake --build build/ --config Release -j7

echo "Finished Building..."