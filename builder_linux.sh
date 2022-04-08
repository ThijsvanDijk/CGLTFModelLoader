echo "------- Setup -------"

mkdir lib
NOFPROC=`expr $(nproc) - 1`

echo "------- Downloading Libs -------"

mkdir submodules && cd submodules
git clone https://github.com/glfw/glfw.git
git clone https://github.com/nigels-com/glew.git

echo "------- Building GLFW LIBRARY -------"

mkdir glfw/build
cmake -B ./glfw/build/ -S glfw/ -DBUILD_SHARED_LIBS=ON
cmake --build glfw/build/ --config Release -j$NOFPROC

cp glfw/build/src/libglfw.so* ../lib/
cp -r glfw/include/* ../include/

echo "------- Done Building GLFW LIBRARY -------"
echo "------- Building GLEW LIBRARY -------"

mkdir glew/build
cmake -B ./glew/build/ -S glew/
cmake --build glew/build/ -j$NOFPROC

cp glew/build/lib/libglut.so* ../lib/
cp -r glew/include/* ../include/

echo "------- Done Building GLEW LIBRARY -------"
cd ../
# rm -r submodules
echo "------- Building Executable -------"

mkdir build
cmake -B build/ -S ./
cmake --build build/ --config Release -j$NOFPROC

echo "Finished Building..."