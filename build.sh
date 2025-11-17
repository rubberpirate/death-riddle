#!/bin/bash

# Build script for Death Riddle

echo "========================================="
echo "  Death Riddle - Build Script"
echo "========================================="
echo ""

# Check if build directory exists
if [ -d "build" ]; then
    echo "Cleaning existing build directory..."
    rm -rf build
fi

# Create build directory
echo "Creating build directory..."
mkdir -p build
cd build

# Run CMake
echo ""
echo "Running CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo ""
    echo "❌ CMake configuration failed!"
    echo "Please make sure Qt6 is installed."
    echo ""
    echo "On Ubuntu/Debian:"
    echo "  sudo apt-get install qt6-base-dev qt6-multimedia-dev"
    echo ""
    echo "On Fedora:"
    echo "  sudo dnf install qt6-qtbase-devel qt6-qtmultimedia-devel"
    echo ""
    echo "On Arch:"
    echo "  sudo pacman -S qt6-base qt6-multimedia"
    echo ""
    exit 1
fi

# Build the project
echo ""
echo "Building project..."
cmake --build . -j$(nproc)

if [ $? -ne 0 ]; then
    echo ""
    echo "❌ Build failed!"
    exit 1
fi

echo ""
echo "========================================="
echo "  ✅ Build successful!"
echo "========================================="
echo ""
echo "To run the game:"
echo "  cd build/bin"
echo "  ./DeathRiddle"
echo ""
echo "Or simply run:"
echo "  ./build/bin/DeathRiddle"
echo ""
