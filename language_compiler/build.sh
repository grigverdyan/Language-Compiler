#!/bin/bash

BUILD_DIR="build"

# Function to clean the build 
clean() {
    echo "Cleaning build directory..."
    rm -rf $BUILD_DIR
    echo "Build directory cleaned."
}

# Function to build the project
build() {
    echo "Building the project..."
    mkdir -p $BUILD_DIR
    cd $BUILD_DIR
    cmake ..
    echo "Build completed."
}

# Function to rebuild the project
rebuild() {
    clean
    build
}

# Function to make
mymake() {
    pwd
    cd $BUILD_DIR
    make
    cd ..
    echo "Target made."
}

case "$1" in
    clean)
        clean
        ;;
    build)
        build
        ;;
    rebuild)
        rebuild
        ;;
    mymake)
        make
        ;;
    *)
        echo "Usage: $0 {clean|build|rebuild}"
        exit 1
        ;;
esac