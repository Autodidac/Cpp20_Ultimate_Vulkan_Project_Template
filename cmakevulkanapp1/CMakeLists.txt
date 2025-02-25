
# Set the vcpkg toolchain file
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg_installed/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "Vcpkg toolchain file")
endif()

cmake_minimum_required(VERSION 3.8)
project(cmakevulkanapp1 LANGUAGES CXX)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(cmakevulkanapp1 "cmakevulkanapp1.cpp" "Header.h")

# Check if VULKAN_SDK is set
if(NOT DEFINED ENV{VULKAN_SDK})
    message(FATAL_ERROR "VULKAN_SDK environment variable not set. Please install the Vulkan SDK and set VULKAN_SDK accordingly.")
endif()
set(VULKAN_SDK_ENV $ENV{VULKAN_SDK})

# Include Vulkan headers and link to Vulkan library from SDK
target_include_directories(cmakevulkanapp1 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${VULKAN_SDK_ENV}/Include
    ${VCPKG_INSTALLATION_ROOT}/x64-linux/include
    ${VCPKG_INSTALLATION_ROOT}/x64-windows/include
)

target_link_directories(cmakevulkanapp1 PRIVATE
    ${VULKAN_SDK_ENV}/Lib
    ${VCPKG_INSTALLATION_ROOT}/x64-linux/lib
    ${VCPKG_INSTALLATION_ROOT}/x64-windows/lib
)

# Find Vulkan, glfw3, and glm
find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG QUIET)
find_package(glm CONFIG QUIET)

if(glfw3_FOUND AND glm_FOUND)
    message(STATUS "Using glfw3 and glm.")
    target_link_libraries(cmakevulkanapp1 PRIVATE Vulkan::Vulkan glfw glm::glm)
else()
    message(FATAL_ERROR "Required packages (glfw3 and glm) not found. Please install them via your package manager or vcpkg.")
endif()

# Install rule
install(TARGETS cmakevulkanapp1 RUNTIME DESTINATION bin)
