#!/bin/bash
# Usage: ./build.sh [gcc|clang] [Debug|Release]

# Set the installation prefix (this remains the same across build and install)
INSTALL_PREFIX="${PWD}/built"

# Select compiler and assign a friendly name for the build folder
if [ "$1" == "gcc" ]; then
  COMPILER_C="gcc"
  COMPILER_CXX="g++"
  COMPILER_NAME="GCC"
elif [ "$1" == "clang" ]; then
  COMPILER_C="clang"
  COMPILER_CXX="clang++"
  COMPILER_NAME="Clang"
else
  echo "Usage: $0 [gcc|clang] [Debug|Release]"
  exit 1
fi

# Set build type (default to Debug if not provided)
if [ -z "$2" ]; then
  BUILD_TYPE="Debug"
else
  BUILD_TYPE="$2"
fi

# Define the build directory including both compiler and build type
BUILD_DIR="Bin/${COMPILER_NAME}-${BUILD_TYPE}"

# Create the build and install directories if they don't exist
mkdir -p "$BUILD_DIR"
mkdir -p "$INSTALL_PREFIX"

# Set the Vulkan SDK directory (assumed to be under "VulkanSDK/1.3.275.0" in the project root)
VULKAN_SDK_DIR="${PWD}/VulkanSDK/1.3.275.0"
if [ ! -d "$VULKAN_SDK_DIR" ]; then
  echo "Error: Vulkan SDK directory not found at $VULKAN_SDK_DIR"
  exit 1
fi

# Export VULKAN_SDK for find_package(Vulkan)
export VULKAN_SDK="$VULKAN_SDK_DIR"

# Configure the project with CMake, passing in the necessary variables
cmake -S . -B "$BUILD_DIR" \
  -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX" \
  -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
  -DCMAKE_C_COMPILER="$COMPILER_C" \
  -DCMAKE_CXX_COMPILER="$COMPILER_CXX" \
  -DVULKAN_SDK="$VULKAN_SDK_DIR"

# Build the project (verbose output)
cmake --build "$BUILD_DIR" --verbose
