#!/bin/bash
set -e

# Navigate to the source directory
cd src/BT_example/pick_place

# Create build directory
echo "===================="
echo "Creating Build Directory"
echo "===================="
mkdir build
cd build

# Compile the project
echo "===================="
echo "Compiling the Project"
echo "===================="
cmake ..
make

# Run the compiled executable
echo "===================="
echo "Running the Executable"
echo "===================="
./pickPlace