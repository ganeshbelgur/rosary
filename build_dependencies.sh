#!/usr/bin/env bash

# Reliable way to get current script path
# Taken from https://stackoverflow.com/questions/4774054/reliable-way-for-a-bash-script-to-get-the-full-path-to-itself
SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd $SCRIPTPATH

# Setup the build folders
mkdir -p bin
mkdir -p bin/INSTALL
mkdir -p bin/INSTALL/include
mkdir -p bin/INSTALL/lib
mkdir -p bin/glfw
mkdir -p bin/glad
mkdir -p bin/assimp

# Set up header only GLM library
cp -R -v $SCRIPTPATH/thirdparty/glm/glm $SCRIPTPATH/bin/INSTALL/include

# Set up STB image loader library
cp -R -v $SCRIPTPATH/thirdparty/stb $SCRIPTPATH/bin/INSTALL/include 

# Compile glfw
cmake -S $SCRIPTPATH/thirdparty/glfw -B $SCRIPTPATH/bin/glfw -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=$SCRIPTPATH/bin/INSTALL

cd $SCRIPTPATH/bin/glfw
make
make install
cd $SCRIPTPATH

# Compile glad
cmake -S $SCRIPTPATH/thirdparty/glad -B $SCRIPTPATH/bin/glad -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=$SCRIPTPATH/bin/INSTALL

cd $SCRIPTPATH/bin/glad
make
cp -v libglad.so $SCRIPTPATH/bin/INSTALL/lib
cp -R -v include $SCRIPTPATH/bin/INSTALL
cd $SCRIPTPATH

# Compile assimp
cmake -S $SCRIPTPATH/thirdparty/assimp -B $SCRIPTPATH/bin/assimp -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=$SCRIPTPATH/bin/INSTALL

cd $SCRIPTPATH/bin/assimp
make
make install
cd $SCRIPTPATH

