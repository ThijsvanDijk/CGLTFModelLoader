echo "------- Building GLFW LIBRARY -------"
mkdir glfw/build
cmake -B glfw/build/ -S glfw/ -DBUILD_SHARED_LIBS=ON
cmake --build glfw/build/
mkdir lib
cp glfw/build/src/libglfw.* lib/
cp -r glfw/include/* include/
echo "------- Done Building GLFW LIBRARY -------"
echo "------- Building Executable -------"
mkdir build
cmake -B build/ -S ./
cmake --build build/
echo "Finished Building..."