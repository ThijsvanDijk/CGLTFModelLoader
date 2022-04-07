echo "------- Getting GLFW LIBRARY -------"
git clone https://github.com/glfw/glfw.git
echo "------- Building GLFW LIBRARY -------"

mkdir glfw/build
cmake -B ./glfw/build/ -S glfw/ -DBUILD_SHARED_LIBS=ON
cmake --build glfw/build/ --config Release -j7

mkdir lib
cp glfw/build/src/libglfw.* lib/
cp -r glfw/include/* include/

echo "------- Done Building GLFW LIBRARY -------"
echo "------- Building Executable -------"

mkdir build
cmake -B build/ -S ./
cmake --build build/ --config Release -j7

echo "Finished Building..."