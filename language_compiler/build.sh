#!/bin/bash

BUILD_DIR=build

case "$1" in
    "build")
        mkdir -p "$BUILD_DIR"
        cd "$BUILD_DIR"
        cmake ..
        make
        cd ..
        ;;
    "clean")
        rm -rf "$BUILD_DIR"
        ;;
    "rebuild")
        rm -rf "$BUILD_DIR"
        mkdir -p "$BUILD_DIR"
        cd "$BUILD_DIR"
        cmake ..
        make
        cd ..
        ;;
    *)
        echo "Usage: $0 {build|clean|rebuild}"
        exit 1
        ;;
esac