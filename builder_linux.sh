#!/bin/bash

echo "------- Setup -------"

mkdir lib
NOFPROC=`expr $(nproc) - 1`
cd submodules

echo "------- Building ASSIMP LIBRARY -------"

mkdir assimp/build
cmake -B assimp/build/ -S assimp/ -DBUILD_ASSIMP_TOOLS=OFF -DASSIMP_BUILD_TESTS=OFF -DINJECT_DEBUG_POSTFIX=OFF -DASSIMP_INSTALL_PDB=OFF -DIGNORE_GIT_HASH=ON
cmake --build assimp/build/ --config Release -j$NOFPROC
cp assimp/build/bin/libassimp.so* ../lib/
cp assimp/build/include/assimp/config.h ../include/assimp/

echo "------- Done Building ASSIMP LIBRARY -------"

echo "------- Building GLFW LIBRARY -------"

mkdir glfw/build
cmake -B glfw/build/ -S glfw/ -DBUILD_SHARED_LIBS=ON -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF
cmake --build glfw/build/ --config Release -j$NOFPROC
cp glfw/build/src/libglfw.so* ../lib/

echo "------- Done Building GLFW LIBRARY -------"

echo "------- Building CGLM LIBRARY -------"

mkdir cglm/build
cmake -B cglm/build/ -S cglm/ -DCGLM_SHARED=ON -DCGLM_USE_TEST=OFF
cmake --build cglm/build/ --config Release -j$NOFPROC
cp cglm/build/libcglm.so* ../lib/

echo "------- Done Building GLFW LIBRARY -------"

cd ../

echo "------- Building Executable -------"

mkdir build
cmake -B build/ -S ./
cmake --build build/ --config Release -j$NOFPROC

echo "------- Finished Building -------"