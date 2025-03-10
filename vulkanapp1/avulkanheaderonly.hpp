﻿// vulkan_cube.hpp
#pragma once

// start with GLFW first
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// platform specific includes
#if defined(_WIN32)
    #include "Windows.h"
#endif 

// Inlude VulkanSDK for C++
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#define VULKAN_HPP_NO_EXCEPTIONS
#define ENABLE_VALIDATION_LAYERS
#include <vulkan/vulkan.hpp>
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

// stb
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// glm is included in the camera code, we'll just use that
#include "avulkancamera.hpp"
//#include <glm/glm.hpp>  // For matrices and vectors
//#include <glm/gtc/matrix_transform.hpp> // For transformations like rotate, lookAt, perspective

// these are for glm::to_string
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <array>
#include <chrono>
#include <cstring>      // For memcpy
#include <fstream>  // For std::ifstream
#include <iostream>
#include <optional>
#include <set>
#include <stdexcept>         // For exceptions (if necessary)
#include <string>   // For std::string
#include <vector>   // For std::vector
#include <chrono>

float calculateDeltaTime() {
    static auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;
    return deltaTime;
}

//static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
//    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
//    VkDebugUtilsMessageTypeFlagsEXT messageType,
//    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
//    void* pUserData) {
//    
//    std::cerr << "[Vulkan Validation Layer] ";
//
//    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
//        std::cerr << "[ERROR] ";
//    } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
//        std::cerr << "[WARNING] ";
//    } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
//        std::cerr << "[INFO] ";
//    } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
//        std::cerr << "[VERBOSE] ";
//    }
//
//    std::cerr << "Message: " << pCallbackData->pMessage << std::endl;
//
//    return VK_FALSE;
//}
//
//void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
//    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
//    if (func != nullptr) {
//        func(instance, debugMessenger, pAllocator);
//    }
//}

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

/*
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }



VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}


void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

struct DebugMessengerDeleter {
    VkInstance instance;
    void operator()(VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator) const {
        DestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator);
    }
};
*/
//#define VK_CHECK(result) if(result != vk::Result::eSuccess) { \
//    throw std::runtime_error("Vulkan error at line " + std::to_string(__LINE__)); }

#define VK_CHECK(x)                                                                    \
    do                                                                                \
    {                                                                                 \
        VkResult err = x;                                                             \
        if (err != VK_SUCCESS)                                                        \
        {                                                                             \
            throw std::runtime_error("Vulkan error (" + std::to_string(err) +         \
                                     ") at " + __FILE__ + ":" + std::to_string(__LINE__)); \
        }                                                                             \
    } while (0)


namespace VulkanCube {

    // setup the camera
   // auto cam = Camera::create(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
    inline Camera::State cam = Camera::create(
        glm::vec3(0.0f, 0.0f, 5.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -90.0f, 0.0f
    );



    //enum class Direction { Forward = 0, Backward, Left, Right };

    //auto processKeyboard(Camera::State& cam, Camera::Direction direction, float deltaTime) -> void {
    //    float velocity = cam.speed * deltaTime;
    //    switch (direction) {
    //    case Camera::Direction::Forward:  cam.position += cam.front * velocity; break;
    //    case Camera::Direction::Backward: cam.position -= cam.front * velocity; break;
    //    case Camera::Direction::Left:     cam.position -= cam.right * velocity; break;
    //    case Camera::Direction::Right:    cam.position += cam.right * velocity; break;
    //    }
    //}

    struct Vertex {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 texCoord;

        static vk::VertexInputBindingDescription getBindingDescription() {
            return { 0, sizeof(Vertex), vk::VertexInputRate::eVertex };
        }

        static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions() {
            return { {
                {0, 0, vk::Format::eR32G32B32Sfloat, offsetof(Vertex, pos)},
                {1, 0, vk::Format::eR32G32Sfloat, offsetof(Vertex, texCoord)}
            } };
        }
    };

    const std::vector<Vertex> vertices = {
        // Front face (z = 0.5)
        {{-0.5f, -0.5f,  0.5f}, {0, 0, 1}, {0, 0}},
        {{ 0.5f, -0.5f,  0.5f}, {0, 0, 1}, {1, 0}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 0, 1}, {1, 1}},
        {{-0.5f,  0.5f,  0.5f}, {0, 0, 1}, {0, 1}},
        // Back face (z = -0.5)
        {{ 0.5f, -0.5f, -0.5f}, {0, 0, -1}, {0, 0}},
        {{-0.5f, -0.5f, -0.5f}, {0, 0, -1}, {1, 0}},
        {{-0.5f,  0.5f, -0.5f}, {0, 0, -1}, {1, 1}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 0, -1}, {0, 1}},
        // Left face (x = -0.5)
        {{-0.5f, -0.5f, -0.5f}, {-1, 0, 0}, {0, 0}},
        {{-0.5f, -0.5f,  0.5f}, {-1, 0, 0}, {1, 0}},
        {{-0.5f,  0.5f,  0.5f}, {-1, 0, 0}, {1, 1}},
        {{-0.5f,  0.5f, -0.5f}, {-1, 0, 0}, {0, 1}},
        // Right face (x = 0.5)
        {{ 0.5f, -0.5f,  0.5f}, {1, 0, 0}, {0, 0}},
        {{ 0.5f, -0.5f, -0.5f}, {1, 0, 0}, {1, 0}},
        {{ 0.5f,  0.5f, -0.5f}, {1, 0, 0}, {1, 1}},
        {{ 0.5f,  0.5f,  0.5f}, {1, 0, 0}, {0, 1}},
        // Top face (y = 0.5)
        {{-0.5f,  0.5f,  0.5f}, {0, 1, 0}, {0, 0}},
        {{ 0.5f,  0.5f,  0.5f}, {0, 1, 0}, {1, 0}},
        {{ 0.5f,  0.5f, -0.5f}, {0, 1, 0}, {1, 1}},
        {{-0.5f,  0.5f, -0.5f}, {0, 1, 0}, {0, 1}},
        // Bottom face (y = -0.5)
        {{-0.5f, -0.5f, -0.5f}, {0, -1, 0}, {0, 0}},
        {{ 0.5f, -0.5f, -0.5f}, {0, -1, 0}, {1, 0}},
        {{ 0.5f, -0.5f,  0.5f}, {0, -1, 0}, {1, 1}},
        {{-0.5f, -0.5f,  0.5f}, {0, -1, 0}, {0, 1}},
    };

    const std::vector<uint16_t> indices = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        8, 9,10,10,11, 8,
        // Right face
        12,13,14,14,15,12,
        // Top face
        16,17,18,18,19,16,
        // Bottom face
        20,21,22,22,23,20
    };


    //const std::vector<uint16_t> indices = {
    //    // Back face (z = -0.5) - Desired normal: (0,0,-1)
    //    0, 3, 2,  2, 1, 0,
    //    // Front face (z = 0.5) - Desired normal: (0,0,1)
    //    4, 5, 6,  6, 7, 4,
    //    // Left face (x = -0.5) - Desired normal: (-1,0,0)
    //    0, 4, 7,  7, 3, 0,
    //    // Right face (x = 0.5) - Desired normal: (1,0,0)
    //    1, 6, 5,  1, 2, 6,
    //    // Bottom face (y = -0.5) - Desired normal: (0,-1,0)
    //    0, 1, 5,  5, 4, 0,
    //    // Top face (y = 0.5) - Desired normal: (0,1,0)
    //    3, 7, 6,  3, 6, 2
    //};

    //const std::vector<uint16_t> indices = {
    //    // Back face (z = -0.5) - Desired normal: (0,0,-1)
    //    0, 1, 2,  2, 3, 0,
    //    // Front face (z = 0.5) - Desired normal: (0,0,1)
    //    4, 5, 6,  6, 7, 4,
    //    // Left face (x = -0.5) - Desired normal: (-1,0,0)
    //    0, 4, 7,  7, 3, 0,
    //    // Right face (x = 0.5) - Desired normal: (1,0,0)
    //    1, 6, 5,  1, 2, 6,
    //    // Bottom face (y = -0.5) - Desired normal: (0,-1,0)
    //    0, 1, 5,  5, 4, 0,
    //    // Top face (y = 0.5) - Desired normal: (0,1,0)
    //    3, 7, 6,  3, 6, 2
    //};

    struct UniformBufferObject {
        alignas(16) glm::mat4 model;
        alignas(16) glm::mat4 view;
        alignas(16) glm::mat4 proj;
    };

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    // Application Class
    class Application {
    public:
        void run() {
            initWindow();
            initVulkan();
            mainLoop();
            cleanup();
        }

    private:
        // Window management
        GLFWwindow* window;

        // Vulkan instance and surface
        vk::UniqueInstance instance;
        vk::UniqueSurfaceKHR surface;

        // Vulkan debugging tools
       // vk::UniqueDebugUtilsMessengerEXT debugMessenger;

        // Vulkan device and queues
        vk::PhysicalDevice physicalDevice;
        vk::UniqueDevice device;
        vk::Queue graphicsQueue;
        vk::Queue presentQueue;
        QueueFamilyIndices queueFamilyIndices;  // Store queue family indices

        // Swapchain and image views
        vk::UniqueSwapchainKHR swapChain;
        std::vector<vk::Image> swapChainImages;  // Raw handles instead of UniqueImage
        vk::Format swapChainImageFormat;
        vk::Extent2D swapChainExtent;
        std::vector<vk::UniqueImageView> swapChainImageViews;

        // Render pass and pipelines
        vk::UniqueRenderPass renderPass;
        vk::UniqueDescriptorSetLayout descriptorSetLayout;
        vk::UniquePipelineLayout pipelineLayout;
        vk::UniquePipeline graphicsPipeline;

        // Buffers (vertex, index, uniform, texture)
        vk::UniqueBuffer vertexBuffer;
        vk::UniqueDeviceMemory vertexBufferMemory;
        vk::UniqueBuffer indexBuffer;
        vk::UniqueDeviceMemory indexBufferMemory;
        vk::UniqueBuffer uniformBuffer;
        vk::UniqueDeviceMemory uniformBufferMemory;
        void* uniformBufferMapped;  // Pointer to mapped memory of uniform buffer

        // In Application class declaration
        std::vector<vk::UniqueDeviceMemory> uniformBuffersMemory; // 🔥 Add this line
        std::vector<void*> uniformBuffersMapped; // Add this to the Application class

        // Textures (image, sampler)
        vk::UniqueImage textureImage;
        vk::UniqueDeviceMemory textureImageMemory;
        vk::UniqueImageView textureImageView;
        vk::UniqueSampler textureSampler;

        // Command pool and buffers
        vk::UniqueCommandPool commandPool;
        std::vector<vk::UniqueCommandBuffer> commandBuffers;

        // Descriptor sets and pool
        vk::UniqueDescriptorPool descriptorPool;
        std::vector<vk::UniqueDescriptorSet> descriptorSets;

        // Framebuffer and depth buffer
        vk::UniqueImage depthImage;
        vk::UniqueDeviceMemory depthImageMemory;
        vk::UniqueImageView depthImageView;
        std::vector<vk::UniqueFramebuffer> framebuffers;
        vk::ClearValue clearValue;  // Clear value for rendering

        std::vector<vk::UniqueSampler> textureSamplers; // Texture samplers for each image
        std::vector<vk::UniqueBuffer> uniformBuffers;   // Uniform buffers for each frame
        std::vector<vk::UniqueImageView> textureImageViews;

        // Semaphores and fences for synchronization
        std::vector<vk::UniqueSemaphore> imageAvailableSemaphores;
        std::vector<vk::UniqueSemaphore> renderFinishedSemaphores;
        std::vector<vk::UniqueFence> inFlightFences;


        std::vector<vk::UniqueImage> textures;                   // This will hold the vk::Image objects for the textures
        vk::Format textureFormat = vk::Format::eR8G8B8A8Srgb;  // This is an example texture format, adjust as needed

        // Frame tracking and resizing
        size_t currentFrame = 0;
        bool framebufferResized = false;

        // Vulkan extensions required
        const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

        float lastX = 400, lastY = 300;
        bool firstMouse = true;
        // External variable to control cube rotation (update via input handling)
        float cubeRotation = 1.0f; // In radians, updated per frame or by user input

        // Define updateCamera to process keyboard input and update the camera state.
        void updateCamera(float deltaTime) {
            // Assuming you have a global GLFWwindow* named 'window'
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                Camera::processKeyboard(VulkanCube::cam, Camera::Direction::Forward, deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                Camera::processKeyboard(VulkanCube::cam, Camera::Direction::Backward, deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                Camera::processKeyboard(VulkanCube::cam, Camera::Direction::Left, deltaTime);
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                Camera::processKeyboard(VulkanCube::cam, Camera::Direction::Right, deltaTime);
            }
        }

        static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
            auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
            if (app) {
                app->processMouseInput(xpos, ypos);
            }
        }

        void initWindow() {
            // Initialize the dispatcher for global (instance-independent) functions.
            VULKAN_HPP_DEFAULT_DISPATCHER.init();

            // Diagnostic: verify that the function pointer is valid.
           // std::cout << "vkEnumerateInstanceLayerProperties pointer: " << (void*)VULKAN_HPP_DEFAULT_DISPATCHER.vkEnumerateInstanceLayerProperties << std::endl;


            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            window = glfwCreateWindow(800, 600, "Vulkan Cube", nullptr, nullptr);
            glfwSetWindowUserPointer(window, this);
            glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

            // Register the mouse callback to handle mouse movement.
            glfwSetCursorPosCallback(window, mouse_callback);
            // Disable the cursor for free-look camera behavior. use Alt+F4 to escape the lack of mouse and close the window 
            //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        void processMouseInput(double xpos, double ypos) {
            if (firstMouse) {
                lastX = static_cast<float>(xpos);
                lastY = static_cast<float>(ypos);
                firstMouse = false;
            }

            float xOffset = static_cast<float>(xpos) - lastX;
            float yOffset = lastY - static_cast<float>(ypos); // Invert Y
            yOffset = static_cast<float>(ypos) - lastY; // Remove inversion - comment this out to flip Y axis in mouse control

            lastX = static_cast<float>(xpos);
            lastY = static_cast<float>(ypos);

            Camera::processMouse(VulkanCube::cam, xOffset, yOffset);
        }

        static void framebufferResizeCallback(GLFWwindow* window, int, int) {
            auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
            if (app) app->framebufferResized = true;
        }

        void initVulkan() {

            createInstanceUnique();
            createSurface(); // Ensure the surface is created before using it

    if (enableValidationLayers) {
        //setupDebugMessenger();
    }
            physicalDevice = pickPhysicalDevice(); // Select a suitable physical device
            if (!physicalDevice) {
                std::cerr << "Failed to pick a physical device!" << std::endl;
                return;
            }

            initializeQueueFamilies(physicalDevice, *surface); // Initialize queue family indices
            createLogicalDevice(); // Create logical device and retrieve queues
            createSwapChain();
            createImageViews();
            createRenderPass();
            createDescriptorSetLayout();
            createGraphicsPipeline();
            createCommandPool(); // Ensure queue family indices are set up
            createDepthResources();
            createFramebuffers();
            createTextureImage();
            createTextureImageView();
            createTextureSampler();
            createVertexBuffer();
            createIndexBuffer();
            createUniformBuffers();
            createDescriptorPool();
            createDescriptorSets();
            createCommandBuffers();
            createSyncObjects();
        }

        void createInstanceUnique() {
            if (!glfwInit()) {
                throw std::runtime_error("Failed to initialize GLFW");
            }

            // Initialize the dispatcher for instance-independent functions.
            VULKAN_HPP_DEFAULT_DISPATCHER.init();

            // Optional: enumerate available instance extensions for diagnostic purposes.
            auto extResult = vk::enumerateInstanceExtensionProperties(nullptr);
            if (extResult.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to enumerate instance extensions!");
            }
            std::vector<vk::ExtensionProperties> availableExtensions = extResult.value;
            std::cout << "Available instance extensions:" << std::endl;
            for (const auto& ext : availableExtensions) {
                std::cout << "\t" << ext.extensionName << std::endl;
            }

            // Get GLFW extensions.
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

            // Check for portability extension support.
            bool portabilityAvailable = false;
            for (const auto& ext : availableExtensions) {
                if (strcmp(ext.extensionName, "VK_KHR_portability_enumeration") == 0) {
                    portabilityAvailable = true;
                    break;
                }
            }
            if (portabilityAvailable) {
                extensions.push_back("VK_KHR_portability_enumeration");
            }

            // Conditional validation layers (enable only in debug builds)
            std::vector<const char*> validationLayers;
#ifdef _DEBUG
            auto layerResult = vk::enumerateInstanceLayerProperties();
            if (layerResult.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to enumerate instance layer properties!");
            }
            std::vector<vk::LayerProperties> availableLayers = layerResult.value;
            bool validationLayerFound = false;
            for (const auto& layer : availableLayers) {
                if (strcmp(layer.layerName, "VK_LAYER_KHRONOS_validation") == 0) {
                    validationLayerFound = true;
                    break;
                }
            }

            if (validationLayerFound) {
                validationLayers.push_back("VK_LAYER_KHRONOS_validation");
                // Only add the debug utils extension if it's available.
                bool debugUtilsAvailable = false;
                for (const auto& ext : availableExtensions) {
                    if (strcmp(ext.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0) {
                        debugUtilsAvailable = true;
                        break;
                    }
                }
                if (debugUtilsAvailable) {
                    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
                }
                else {
                    std::cerr << "VK_EXT_debug_utils extension not available." << std::endl;
                }
            }
            else {
                std::cerr << "Validation layers requested but not available; proceeding without them." << std::endl;
            }
#endif

            vk::ApplicationInfo appInfo{};
            appInfo.pApplicationName = "Vulkan App";
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = "No Engine";
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_3; // Set this to the version of Vulkan you are using.

            vk::InstanceCreateInfo createInfo{};
            createInfo.pApplicationInfo = &appInfo;
            createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
            createInfo.ppEnabledExtensionNames = extensions.data();
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.empty() ? nullptr : validationLayers.data();

            // Create Vulkan instance explicitly passing the dispatcher.
            vk::ResultValue<vk::UniqueInstance> result = vk::createInstanceUnique(createInfo, nullptr, VULKAN_HPP_DEFAULT_DISPATCHER);
            if (result.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to create Vulkan instance: " + vk::to_string(result.result));
            }
            instance = std::move(result.value);

            // Reinitialize the dispatcher with instance-level functions.
            VULKAN_HPP_DEFAULT_DISPATCHER.init(instance.get());
        }

        std::vector<const char*> getRequiredExtensions() {
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            if (!glfwExtensions) {
                throw std::runtime_error("Failed to get required GLFW extensions!");
            }

            std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
            //extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            return extensions;
        }
/*
        void setupDebugMessenger() {
            vk::DebugUtilsMessengerCreateInfoEXT createInfo;
            createInfo.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
            createInfo.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance);
            createInfo.setPfnUserCallback(debugCallback);

            // No need to check vk::Result, an exception will be thrown if this fails
            auto result = instance->createDebugUtilsMessengerEXTUnique(createInfo);
            if (result.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to create debug messenger!");
            }
            debugMessenger = std::move(result.value);

        }
*/
/*
        void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
            createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = debugCallback;
        }

        void setupDebugMessenger() {
            if (!enableValidationLayers) return;

            VkDebugUtilsMessengerCreateInfoEXT createInfo{};
            populateDebugMessengerCreateInfo(createInfo);

            VkDebugUtilsMessengerEXT messenger;
            if (CreateDebugUtilsMessengerEXT(*instance, &createInfo, nullptr, &messenger) != VK_SUCCESS) {
                throw std::runtime_error("failed to set up debug messenger!");
            }

            vk::DispatchLoaderDynamic dld(*instance, vkGetInstanceProcAddr);
            debugMessenger = vk::UniqueDebugUtilsMessengerEXT(messenger, *instance, dld);

        }*/

        vk::PhysicalDevice pickPhysicalDevice() {
            // Enumerate physical devices
            auto result = instance->enumeratePhysicalDevices();

            // Check if the enumeration was successful
            if (result.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to enumerate physical devices!");
            }

            auto devices = result.value;
            if (devices.empty()) {
                throw std::runtime_error("Failed to find GPUs with Vulkan support!");
            }

            // Loop through available devices and find a suitable one
            for (const auto& device : devices) {
                if (isDeviceSuitable(device)) {
                    return device;
                }
            }

            throw std::runtime_error("Failed to find a suitable GPU!");
        }

        bool isDeviceSuitable(vk::PhysicalDevice device) {
            auto indices = findQueueFamilies(device, *surface);

            bool extensionsSupported = checkDeviceExtensionSupport(device);
            bool swapChainAdequate = false;

            if (extensionsSupported) {
                auto swapChainSupport = querySwapChainSupport(device);
                swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
            }

            return indices.isComplete() && extensionsSupported && swapChainAdequate;
        }

        QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface) {
            QueueFamilyIndices indices;

            auto queueFamilies = device.getQueueFamilyProperties();
            for (uint32_t i = 0; i < queueFamilies.size(); i++) {
                // Check if the queue supports graphics
                if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics) {
                    indices.graphicsFamily = i;
                }

                // Check if the queue supports presenting to the surface
                vk::ResultValue<uint32_t> support = device.getSurfaceSupportKHR(i, surface);
                if (support.result == vk::Result::eSuccess) {  // Compare the result field
                    indices.presentFamily = i;
                }

                // If both queues are found, no need to continue looping
                if (indices.isComplete()) {
                    break;
                }
            }

            return indices;
        }

        bool checkDeviceExtensionSupport(vk::PhysicalDevice device) {
            try {
                // Enumerate the available extensions
                auto availableExtensions = device.enumerateDeviceExtensionProperties(); // Returns vk::ResultValue<std::vector<vk::ExtensionProperties>>

                // Check if there was an error during enumeration
                if (availableExtensions.result != vk::Result::eSuccess) {
                    std::cerr << "Failed to enumerate device extensions!" << std::endl;
                    return false;
                }

                // Convert the ResultValue to the vector of extensions
                auto extensions = availableExtensions.value;

                // Ensure the list of required extensions is correctly initialized
                std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

                // Check which required extensions are supported by the device
                for (const auto& extension : extensions) {
                    requiredExtensions.erase(extension.extensionName);
                }

                // If required extensions are still missing, report them
                if (!requiredExtensions.empty()) {
                    std::cerr << "Missing required extensions: " << std::endl;
                    for (const auto& ext : requiredExtensions) {
                        std::cerr << "- " << ext << std::endl;
                    }
                    return false;
                }

                // All required extensions are available
                return true;
            }
            catch (const std::exception& e) {
                std::cerr << "Error checking device extension support: " << e.what() << std::endl;
                return false;
            }
        }

        void createLogicalDevice() {
            QueueFamilyIndices indices = findQueueFamilies(physicalDevice, *surface);

            // Ensure both queue families are valid
            if (!indices.isComplete()) {
                throw std::runtime_error("Queue families are not complete!");
            }

            std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
            std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

            float queuePriority = 1.0f;
            for (uint32_t queueFamily : uniqueQueueFamilies) {
                queueCreateInfos.push_back({
                    {}, // flags
                    queueFamily, // queueFamilyIndex
                    1, // queueCount
                    &queuePriority // pQueuePriorities
                    });
            }

            vk::PhysicalDeviceFeatures deviceFeatures;
            deviceFeatures.samplerAnisotropy = VK_TRUE; // Enable anisotropic filtering

            // Set up the logical device create info
            vk::DeviceCreateInfo createInfo(
                {}, // flags
                static_cast<uint32_t>(queueCreateInfos.size()), queueCreateInfos.data(),
                0, nullptr, // No enabled layers
                static_cast<uint32_t>(deviceExtensions.size()), deviceExtensions.data(),
                &deviceFeatures
            );

            // Attempt to create the logical device
            try {
                auto result = physicalDevice.createDeviceUnique(createInfo);
                device = std::move(result.value);
            }
            catch (const std::exception& e) {
                throw std::runtime_error("Failed to create logical device: " + std::string(e.what()));
            }

            // Retrieve the queues from the device
            graphicsQueue = device->getQueue(indices.graphicsFamily.value(), 0);
            presentQueue = device->getQueue(indices.presentFamily.value(), 0);
        }

        // --- Swap Chain Creation ---
        void createSwapChain() {
            // Query support details
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

            // Choose the surface format, present mode, and swap chain extent
            vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
            vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
            swapChainExtent = chooseSwapExtent(swapChainSupport.capabilities);

            // Set the number of images in the swap chain
            uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
            if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
                imageCount = swapChainSupport.capabilities.maxImageCount;
            }

            // Create swapchain info struct
            vk::SwapchainCreateInfoKHR createInfo(
                {}, surface.get(), imageCount, // surface, image count
                surfaceFormat.format, surfaceFormat.colorSpace, // format and color space
                swapChainExtent, 1, // extent, layers
                vk::ImageUsageFlagBits::eColorAttachment, // image usage
                vk::SharingMode::eExclusive, 0, nullptr, // queue sharing
                swapChainSupport.capabilities.currentTransform, // transform
                vk::CompositeAlphaFlagBitsKHR::eOpaque, // composite alpha
                presentMode, VK_TRUE // present mode and clipped
            );

            // Get the queue families for graphics and present
            QueueFamilyIndices indices = findQueueFamilies(physicalDevice, *surface);
            uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

            // Handle concurrent sharing mode if necessary
            if (indices.graphicsFamily != indices.presentFamily) {
                createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
                createInfo.queueFamilyIndexCount = 2;
                createInfo.pQueueFamilyIndices = queueFamilyIndices;
            }

            // Create the swap chain
            auto result = device->createSwapchainKHRUnique(createInfo);
            if (result.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to create swap chain!");
            }
            swapChain = std::move(result.value);

            // Get swap chain images and store them in a vector of raw vk::Image handles.
            auto swapChainImagesResult = device->getSwapchainImagesKHR(*swapChain);
            if (swapChainImagesResult.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to get swap chain images!");
            }

            // Store the raw image handles directly.
            swapChainImages = swapChainImagesResult.value;

            swapChainImageFormat = surfaceFormat.format;
        }

        struct SwapChainSupportDetails {
            vk::SurfaceCapabilitiesKHR capabilities;
            std::vector<vk::SurfaceFormatKHR> formats;
            std::vector<vk::PresentModeKHR> presentModes;
        };

        SwapChainSupportDetails querySwapChainSupport(vk::PhysicalDevice device) {
            // Query surface capabilities
            auto capabilitiesResult = device.getSurfaceCapabilitiesKHR(*surface);
            if (capabilitiesResult.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to get surface capabilities!");
            }
            vk::SurfaceCapabilitiesKHR capabilities = capabilitiesResult.value;

            // Query surface formats
            auto formatsResult = device.getSurfaceFormatsKHR(*surface);
            if (formatsResult.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to get surface formats!");
            }
            std::vector<vk::SurfaceFormatKHR> formats = formatsResult.value;

            // Query surface present modes
            auto presentModesResult = device.getSurfacePresentModesKHR(*surface);
            if (presentModesResult.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to get surface present modes!");
            }
            std::vector<vk::PresentModeKHR> presentModes = presentModesResult.value;

            // Return the query results wrapped in SwapChainSupportDetails
            return { capabilities, formats, presentModes };
        }

        vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats) {
            for (const auto& format : formats) {
                if (format.format == vk::Format::eB8G8R8A8Srgb &&
                    format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                    return format;
                }
            }
            return formats[0];
        }

        vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& modes) {
            for (const auto& mode : modes) {
                if (mode == vk::PresentModeKHR::eMailbox) return mode;
            }
            return vk::PresentModeKHR::eFifo;
        }

        vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) {
            if (capabilities.currentExtent.width != UINT32_MAX) {
                return capabilities.currentExtent;
            }
            else {
                int width, height;
                glfwGetFramebufferSize(window, &width, &height);
                return vk::Extent2D{
                    std::clamp(static_cast<uint32_t>(width),
                    capabilities.minImageExtent.width,
                    capabilities.maxImageExtent.width),
                    std::clamp(static_cast<uint32_t>(height),
                    capabilities.minImageExtent.height,
                    capabilities.maxImageExtent.height)
                };
            }
        }

        // --- Image Views ---
        void createImageViews() {
            swapChainImageViews.resize(swapChainImages.size());
            for (size_t i = 0; i < swapChainImages.size(); i++) {
                swapChainImageViews[i] = createImageViewUnique(swapChainImages[i], swapChainImageFormat);
            }
        }

        vk::UniqueImageView createImageViewUnique(const vk::Image& image, vk::Format format,
            vk::ImageAspectFlags aspectFlags = vk::ImageAspectFlagBits::eColor) {

            // Set up the image view create info
            vk::ImageViewCreateInfo viewInfo({}, image, vk::ImageViewType::e2D, format,
                vk::ComponentMapping{}, // Default component mapping
                vk::ImageSubresourceRange(aspectFlags, 0, 1, 0, 1)); // Image range

            if (!device) {
                throw std::runtime_error("Device is not initialized!");
            }

            // Create the image view and return it
            auto result = device->createImageViewUnique(viewInfo);
            return std::move(result.value);
        }

        // --- Render Pass ---
        void createRenderPass() {
            // Color Attachment Description
            vk::AttachmentDescription colorAttachment(
                {}, swapChainImageFormat, vk::SampleCountFlagBits::e1,
                vk::AttachmentLoadOp::eClear, vk::AttachmentStoreOp::eStore,
                vk::AttachmentLoadOp::eDontCare, vk::AttachmentStoreOp::eDontCare,
                vk::ImageLayout::eUndefined, vk::ImageLayout::ePresentSrcKHR
            );

            // Color Attachment Reference
            vk::AttachmentReference colorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);

            // Subpass Description
            vk::SubpassDescription subpass(
                {}, vk::PipelineBindPoint::eGraphics,
                0, nullptr, 1, &colorAttachmentRef
            );

            // Subpass Dependency (External -> Current Subpass)
            vk::SubpassDependency dependency(
                VK_SUBPASS_EXTERNAL, 0,
                vk::PipelineStageFlagBits::eColorAttachmentOutput,   // srcStageMask
                vk::PipelineStageFlagBits::eColorAttachmentOutput,   // dstStageMask
                {}, vk::AccessFlagBits::eColorAttachmentWrite        // srcAccessMask, dstAccessMask
            );

            // Render Pass Creation
            vk::RenderPassCreateInfo renderPassInfo(
                {}, 1, &colorAttachment, 1, &subpass, 1, &dependency
            );

            // Create the render pass and check for success
            auto result = device->createRenderPassUnique(renderPassInfo);
            if (result.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to create render pass");
            }

            // Move the created render pass into the member variable
            renderPass = std::move(result.value);
        }

        // --- Graphics Pipeline ---
        void createGraphicsPipeline() {
            auto vertShaderCode = readFile("vert.spv");
            auto fragShaderCode = readFile("frag.spv");

            // Create Shader Modules
            vk::UniqueShaderModule vertShaderModule = createShaderModule(device, vertShaderCode);
            vk::UniqueShaderModule fragShaderModule = createShaderModule(device, fragShaderCode);

            vk::PipelineShaderStageCreateInfo vertShaderStageInfo({}, vk::ShaderStageFlagBits::eVertex, *vertShaderModule, "main");
            vk::PipelineShaderStageCreateInfo fragShaderStageInfo({}, vk::ShaderStageFlagBits::eFragment, *fragShaderModule, "main");

            vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

            // Vertex Input Info
            auto bindingDescription = Vertex::getBindingDescription();
            auto attributeDescriptions = Vertex::getAttributeDescriptions();

            vk::PipelineVertexInputStateCreateInfo vertexInputInfo(
                {}, 1, &bindingDescription,
                static_cast<uint32_t>(attributeDescriptions.size()), attributeDescriptions.data());

            // Input Assembly
            vk::PipelineInputAssemblyStateCreateInfo inputAssembly({}, vk::PrimitiveTopology::eTriangleList);

            // Viewport and Scissor
            vk::Viewport viewport(0.0f, 0.0f, static_cast<float>(swapChainExtent.width), static_cast<float>(swapChainExtent.height), 0.0f, 1.0f);
            vk::Rect2D scissor({ 0, 0 }, swapChainExtent);
            vk::PipelineViewportStateCreateInfo viewportState({}, 1, &viewport, 1, &scissor);

            // Rasterizer
            vk::PipelineRasterizationStateCreateInfo rasterizer(
                {}, VK_FALSE, VK_FALSE, vk::PolygonMode::eFill,
                vk::CullModeFlagBits::eFront, vk::FrontFace::eClockwise, VK_FALSE, 1.0f);

            // Multisampling (No MSAA)
            vk::PipelineMultisampleStateCreateInfo multisampling(
                {}, vk::SampleCountFlagBits::e1, VK_FALSE, 0.0f, nullptr, VK_FALSE, VK_FALSE);

            // Color Blending (Default)
            vk::PipelineColorBlendAttachmentState colorBlendAttachment;
            colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

            vk::PipelineColorBlendStateCreateInfo colorBlending(
                {}, VK_FALSE, vk::LogicOp::eCopy, 1, &colorBlendAttachment);

            // Descriptor Set Layout (Make sure this is created before)
            vk::PipelineLayoutCreateInfo pipelineLayoutInfo({}, 1, &*descriptorSetLayout);
            auto pipelineLayoutResult = device->createPipelineLayoutUnique(pipelineLayoutInfo);
            if (pipelineLayoutResult.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to create pipeline layout!");
            }
            pipelineLayout = std::move(pipelineLayoutResult.value);

            // Create Graphics Pipeline
            vk::GraphicsPipelineCreateInfo pipelineInfo(
                {}, 2, shaderStages, &vertexInputInfo, &inputAssembly,
                nullptr, &viewportState, &rasterizer, &multisampling,
                nullptr, &colorBlending, nullptr,
                *pipelineLayout, *renderPass);

            auto pipelineResult = device->createGraphicsPipelineUnique(nullptr, pipelineInfo);
            if (pipelineResult.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to create graphics pipeline!");
            }

            graphicsPipeline = std::move(pipelineResult.value);
        }

        vk::UniqueShaderModule createShaderModule(vk::UniqueDevice& device, const std::vector<char>& code) {
            // Ensure SPIR-V bytecode is properly aligned
           // std::cout << "Shader file size: " << code.size() << " bytes" << std::endl;
            assert(code.size() % 4 == 0 && "SPIR-V bytecode size must be a multiple of 4");

            // Create shader module create info
            vk::ShaderModuleCreateInfo createInfo(
                {}, // Flags (reserved, must be 0)
                code.size(),
                reinterpret_cast<const uint32_t*>(code.data()) // SPIR-V code pointer
            );

            // Create and return the shader module using the UniqueDevice
            vk::ShaderModule shaderModule;
            if (device->createShaderModule(&createInfo, nullptr, &shaderModule) != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to create shader module!");
            }

            return vk::UniqueShaderModule(shaderModule, *device);
        }

        void initializeQueueFamilies(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface) {
            queueFamilyIndices = findQueueFamilies(physicalDevice, surface);
            if (!queueFamilyIndices.isComplete()) {
                throw std::runtime_error("Failed to find required queue families!");
            }
        }

        void createCommandPool() {
            // Ensure that the graphics queue family index is valid
            if (!queueFamilyIndices.graphicsFamily.has_value()) {
                throw std::runtime_error("Graphics family index is not set!");
            }

            vk::CommandPoolCreateInfo poolInfo(
                vk::CommandPoolCreateFlagBits::eResetCommandBuffer,  // Allow resetting command buffers
                queueFamilyIndices.graphicsFamily.value()           // Use the graphics queue family
            );

            // Create the command pool and access the unique pool with .value
            commandPool = device->createCommandPoolUnique(poolInfo).value;
        }

        // --- Command Buffers ---
        void createCommandBuffers() {
            // Resize commandBuffers to match the number of swapChainImageViews
            commandBuffers.resize(swapChainImageViews.size());

            // Allocate command buffers
            vk::CommandBufferAllocateInfo allocInfo(*commandPool, vk::CommandBufferLevel::ePrimary,
                static_cast<uint32_t>(commandBuffers.size()));

            // Allocate command buffers
            auto result = device->allocateCommandBuffersUnique(allocInfo);
            if (result.value.empty()) {
                throw std::runtime_error("Failed to allocate command buffers!");
            }

            // Move the allocated command buffers into the commandBuffers vector
            for (size_t i = 0; i < result.value.size(); ++i) {
                commandBuffers[i] = std::move(result.value[i]);
            }

            // Define the clear values
            std::vector<vk::ClearValue> clearValues;
            clearValues.emplace_back(vk::ClearColorValue(std::array<float, 4>{0.0f, 0.0f, 0.1f, 1.0f}));
            clearValues.emplace_back(vk::ClearDepthStencilValue(1.0f, 0));

            // Loop through and record command buffers
            for (size_t i = 0; i < commandBuffers.size(); i++) {
                vk::CommandBufferBeginInfo beginInfo;
                (void)commandBuffers[i]->begin(beginInfo);

                vk::RenderPassBeginInfo renderPassInfo(
                    *renderPass,
                    *framebuffers[i],
                    vk::Rect2D({ 0, 0 }, swapChainExtent),
                    static_cast<uint32_t>(clearValues.size()),
                    clearValues.data()
                );

                commandBuffers[i]->beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);
                commandBuffers[i]->bindPipeline(vk::PipelineBindPoint::eGraphics, *graphicsPipeline);
                commandBuffers[i]->bindVertexBuffers(0, { *vertexBuffer }, { 0 });
                commandBuffers[i]->bindIndexBuffer(*indexBuffer, 0, vk::IndexType::eUint16);
                commandBuffers[i]->bindDescriptorSets(
                    vk::PipelineBindPoint::eGraphics,
                    *pipelineLayout,
                    0,
                    { *descriptorSets[i] }, // Ensure descriptorSets[i] is valid
                    {}
                );
                commandBuffers[i]->drawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
                commandBuffers[i]->endRenderPass();
                (void)commandBuffers[i]->end();
            }
        }

        // --- Texture Creation ---
        void createTextureImage() {
            int texWidth, texHeight, texChannels;
            stbi_set_flip_vertically_on_load(true);
            stbi_uc* pixels = stbi_load("texture.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
            if (!pixels) {
                throw std::runtime_error(std::string("Failed to load texture image: ") + stbi_failure_reason());
            }

            vk::DeviceSize imageSize = texWidth * texHeight * 4;

            vk::UniqueBuffer stagingBuffer;
            vk::UniqueDeviceMemory stagingBufferMemory;

            // Allocate the staging buffer and memory using the 3-argument createBuffer function.
            std::tie(stagingBuffer, stagingBufferMemory) = createBuffer(
                imageSize,
                vk::BufferUsageFlagBits::eTransferSrc,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
            );

            // Map memory from the staging buffer
            auto mapResult = device->mapMemory(*stagingBufferMemory, 0, imageSize);
            if (mapResult.result != vk::Result::eSuccess || mapResult.value == nullptr) {
                throw std::runtime_error("Failed to map memory!");
            }

            memcpy(mapResult.value, pixels, static_cast<size_t>(imageSize));
            device->unmapMemory(*stagingBufferMemory);
            stbi_image_free(pixels);

            createImage(texWidth, texHeight, vk::Format::eR8G8B8A8Srgb,
                vk::ImageTiling::eOptimal,
                vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled,
                vk::MemoryPropertyFlagBits::eDeviceLocal, textureImage, textureImageMemory);

            transitionImageLayout(*textureImage, vk::Format::eR8G8B8A8Srgb,
                vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal);

            // Copy data from staging buffer to texture image.
            copyBufferToImage(*stagingBuffer, *textureImage, texWidth, texHeight);

            transitionImageLayout(*textureImage, vk::Format::eR8G8B8A8Srgb,
                vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal);

            stagingBufferMemory.reset();
        }


        // --- Texture Image View ---
        void createTextureImageView() {
            vk::ImageViewCreateInfo viewInfo = {};
            viewInfo.image = *textureImage;  // Correctly dereference the unique_ptr for vk::Image
            viewInfo.viewType = vk::ImageViewType::e2D;
            viewInfo.format = vk::Format::eR8G8B8A8Srgb;
            viewInfo.components = vk::ComponentMapping();
            viewInfo.subresourceRange = vk::ImageSubresourceRange(
                vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1);

            // Create ImageView
            auto result = device->createImageViewUnique(viewInfo);
            if (result.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to create image view!");
            }

            textureImageView = std::move(result.value);  // Move the created image view into textureImageView
        }

        void createImage(uint32_t width, uint32_t height, vk::Format format,
            vk::ImageTiling tiling, vk::ImageUsageFlags usage,
            vk::MemoryPropertyFlags properties,
            vk::UniqueImage& image, vk::UniqueDeviceMemory& imageMemory) {

            // Image creation
            vk::ImageCreateInfo imageInfo{};
            imageInfo.imageType = vk::ImageType::e2D;
            imageInfo.format = format;
            imageInfo.extent = vk::Extent3D(width, height, 1);
            imageInfo.mipLevels = 1;
            imageInfo.arrayLayers = 1;
            imageInfo.samples = vk::SampleCountFlagBits::e1;
            imageInfo.tiling = tiling;
            imageInfo.usage = usage;
            imageInfo.sharingMode = vk::SharingMode::eExclusive;
            imageInfo.flags = {}; // Ensure it's explicitly cleared.

            auto result = device->createImageUnique(imageInfo);
            if (result.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to create image! Error: " + std::to_string((int)result.result));
            }
            image = std::move(result.value);

            // Memory requirements
            vk::MemoryRequirements memRequirements = device->getImageMemoryRequirements(*image);
            uint32_t memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
            if (memoryTypeIndex == UINT32_MAX) {
                throw std::runtime_error("Failed to find suitable memory type!");
            }

            vk::MemoryAllocateInfo allocInfo{};
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = memoryTypeIndex;

            auto allocResult = device->allocateMemoryUnique(allocInfo);
            if (allocResult.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to allocate image memory! Error: " + std::to_string((int)allocResult.result));
            }
            imageMemory = std::move(allocResult.value);

            // Bind memory
            vk::Result bindResult = device->bindImageMemory(*image, *imageMemory, 0);
            if (bindResult != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to bind image memory! Error: " + std::to_string((int)bindResult));
            }
        }

        void createVertexBuffer() {
            vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

            vk::BufferCreateInfo bufferInfo(
                {},
                bufferSize,
                vk::BufferUsageFlagBits::eVertexBuffer,
                vk::SharingMode::eExclusive
            );

            // Create the vertex buffer
            auto result = device->createBufferUnique(bufferInfo);
            if (result.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to create vertex buffer!");
            }
            vertexBuffer = std::move(result.value);  // Move the created buffer into vertexBuffer

            // Allocate memory for the buffer
            vk::MemoryRequirements memRequirements = device->getBufferMemoryRequirements(*vertexBuffer);
            vk::MemoryAllocateInfo allocInfo(
                memRequirements.size,
                findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
            );

            auto allocResult = device->allocateMemoryUnique(allocInfo);
            if (allocResult.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to allocate vertex buffer memory!");
            }
            vertexBufferMemory = std::move(allocResult.value);  // Move the allocated memory into vertexBufferMemory

            // Bind the memory to the buffer
            (void)device->bindBufferMemory(*vertexBuffer, *vertexBufferMemory, 0);

            // Map the memory and copy vertex data
            auto mapResult = device->mapMemory(*vertexBufferMemory, 0, bufferSize);
            if (mapResult.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to map vertex buffer memory!");
            }
            void* data = mapResult.value;  // Extract the void* pointer

            memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
            device->unmapMemory(*vertexBufferMemory);
        }

        void createIndexBuffer() {
            vk::DeviceSize bufferSize = sizeof(indices[0]) * indices.size();

            vk::BufferCreateInfo bufferInfo(
                {},
                bufferSize,
                vk::BufferUsageFlagBits::eIndexBuffer,
                vk::SharingMode::eExclusive
            );

//#ifdef _DEBUG
//            std::cout << "Indices: ";
//            for (uint32_t index : indices) {
//                std::cout << index << " ";
//            }
//            std::cout << std::endl;
//#endif

            // Create the index buffer
            auto result = device->createBufferUnique(bufferInfo);
            if (result.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to create index buffer!");
            }
            indexBuffer = std::move(result.value);  // Move the created buffer into indexBuffer

            // Allocate memory for the buffer
            vk::MemoryRequirements memRequirements = device->getBufferMemoryRequirements(*indexBuffer);
            vk::MemoryAllocateInfo allocInfo(
                memRequirements.size,
                findMemoryType(memRequirements.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
            );

            auto allocResult = device->allocateMemoryUnique(allocInfo);
            if (allocResult.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to allocate index buffer memory!");
            }
            indexBufferMemory = std::move(allocResult.value);  // Move the allocated memory into indexBufferMemory

            // Bind the memory to the buffer
            (void)device->bindBufferMemory(*indexBuffer, *indexBufferMemory, 0);

            // Map the memory and copy index data
            auto mapResult = device->mapMemory(*indexBufferMemory, 0, bufferSize);
            if (mapResult.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to map index buffer memory!");
            }
            void* data = mapResult.value;  // Extract the void* pointer

            memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
            device->unmapMemory(*indexBufferMemory);
        }


        // --- Utility Functions ---
        uint32_t findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
            vk::PhysicalDeviceMemoryProperties memProperties = physicalDevice.getMemoryProperties();
            for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
                if ((typeFilter & (1 << i)) &&
                    (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                    return i;
                }
            }
            throw std::runtime_error("Failed to find suitable memory type!");
        }

        std::pair<vk::UniqueBuffer, vk::UniqueDeviceMemory> createBuffer(
            vk::DeviceSize size,
            vk::BufferUsageFlags usage,
            vk::MemoryPropertyFlags properties
        ) {
            if (!device) {
                throw std::runtime_error("Device not initialized!");
            }

            vk::BufferCreateInfo bufferInfo({}, size, usage);

            auto bufferResult = device->createBufferUnique(bufferInfo);
            if (bufferResult.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to create buffer!");
            }
            vk::UniqueBuffer buffer = std::move(bufferResult.value);

            vk::MemoryRequirements memRequirements = device->getBufferMemoryRequirements(*buffer);
            vk::MemoryAllocateInfo allocInfo(memRequirements.size,
                findMemoryType(memRequirements.memoryTypeBits, properties));

            auto memoryResult = device->allocateMemoryUnique(allocInfo);
            if (memoryResult.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to allocate buffer memory!");
            }
            vk::UniqueDeviceMemory bufferMemory = std::move(memoryResult.value);

            (void)device->bindBufferMemory(*buffer, *bufferMemory, 0);
            return { std::move(buffer), std::move(bufferMemory) };
        }

        void copyBuffer(vk::Buffer srcBuffer, vk::Buffer dstBuffer, vk::DeviceSize size) {
            // Allocate command buffer
            vk::CommandBufferAllocateInfo allocInfo(*commandPool, vk::CommandBufferLevel::ePrimary, 1);

            // Allocate command buffers and check result
            auto result = device->allocateCommandBuffersUnique(allocInfo);

            if (result.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to allocate command buffer!");
            }

            vk::UniqueCommandBuffer commandBuffer = std::move(result.value[0]);

            // Begin command buffer recording
            vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
            (void)commandBuffer->begin(beginInfo);

            // Perform the buffer copy
            vk::BufferCopy copyRegion(0, 0, size);  // From offset 0 to offset 0, with the specified size
            commandBuffer->copyBuffer(srcBuffer, dstBuffer, 1, &copyRegion);

            // End command buffer recording
            (void)commandBuffer->end();

            // Submit command buffer to the graphics queue
            vk::SubmitInfo submitInfo(0, nullptr, nullptr, 1, &*commandBuffer);

            // Submit with error checking
            try {
                (void)graphicsQueue.submit(1, &submitInfo, nullptr);
            }
            catch (const std::exception& e) {
                std::cerr << "Error during command buffer submission: " << e.what() << std::endl;
            }

            // Wait for the queue to finish processing the commands
            (void)graphicsQueue.waitIdle();
        }

        // Surface creation
        void createSurface() {
            if (!glfwVulkanSupported()) {
                throw std::runtime_error("Vulkan is not supported by GLFW!");
            }

            if (!window) {
                throw std::runtime_error("GLFW window is not created!");
            }

            VkSurfaceKHR rawSurface;
            VkResult result = glfwCreateWindowSurface(*instance, window, nullptr, &rawSurface);
            if (result != VK_SUCCESS) {
                throw std::runtime_error("Failed to create window surface! Vulkan error: " + std::to_string(result));
            }

            // Wrap the raw Vulkan handle in vk::UniqueSurfaceKHR
            surface = vk::UniqueSurfaceKHR(rawSurface, *instance);
        }

        // Depth resources
        void createDepthResources() {
            vk::Format depthFormat = findDepthFormat();
            createImage(swapChainExtent.width, swapChainExtent.height, depthFormat,
                vk::ImageTiling::eOptimal, vk::ImageUsageFlagBits::eDepthStencilAttachment,
                vk::MemoryPropertyFlagBits::eDeviceLocal, depthImage, depthImageMemory);
            depthImageView = createImageViewUnique(*depthImage, depthFormat, vk::ImageAspectFlagBits::eDepth);
        }

        // query depth format
        vk::Format findDepthFormat() {
            return findSupportedFormat(
                { vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint },
                vk::ImageTiling::eOptimal,
                vk::FormatFeatureFlagBits::eDepthStencilAttachment
            );
        }

        // query support format
        vk::Format findSupportedFormat(const std::vector<vk::Format>& candidates, vk::ImageTiling tiling, vk::FormatFeatureFlags features) {
            for (vk::Format format : candidates) {
                vk::FormatProperties props = physicalDevice.getFormatProperties(format);

                if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
                    return format;
                }
                if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
                    return format;
                }
            }

            throw std::runtime_error("Failed to find a supported format for depth buffer!");
        }


        // Framebuffers
        void createFramebuffers() {
            framebuffers.resize(swapChainImageViews.size());
            for (size_t i = 0; i < swapChainImageViews.size(); i++) {
                std::array<vk::ImageView, 2> attachments = {
                    *swapChainImageViews[i],
                    *depthImageView
                };

                vk::FramebufferCreateInfo framebufferInfo(
                    {}, *renderPass,
                    static_cast<uint32_t>(attachments.size()), attachments.data(),
                    swapChainExtent.width, swapChainExtent.height, 1);

                // Create framebuffer and check result
                auto result = device->createFramebufferUnique(framebufferInfo);
                if (result.result != vk::Result::eSuccess) {
                    throw std::runtime_error("Failed to create framebuffer!");
                }
                framebuffers[i] = std::move(result.value);
            }
        }

        // Texture sampler
        void createTextureSampler() {
            vk::SamplerCreateInfo samplerInfo(
                {}, vk::Filter::eLinear, vk::Filter::eLinear,
                vk::SamplerMipmapMode::eLinear,
                vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat,
                vk::SamplerAddressMode::eRepeat, 0.0f, VK_TRUE, 16.0f,
                VK_FALSE, vk::CompareOp::eAlways, 0.0f, 0.0f,
                vk::BorderColor::eIntOpaqueBlack, VK_FALSE);

            // Create sampler and check result
            auto result = device->createSamplerUnique(samplerInfo);
            if (result.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to create texture sampler!");
            }
            textureSampler = std::move(result.value);
        }

        // Uniform PLURAL buffers
        void createUniformBuffers() {
            uniformBuffers.resize(swapChainImages.size());
            uniformBuffersMemory.resize(swapChainImages.size()); // Store memory
            uniformBuffersMapped.resize(swapChainImages.size());

            for (size_t i = 0; i < swapChainImages.size(); i++) {
                vk::BufferCreateInfo bufferInfo({}, sizeof(UniformBufferObject), vk::BufferUsageFlagBits::eUniformBuffer);
                uniformBuffers[i] = device->createBufferUnique(bufferInfo).value;

                vk::MemoryRequirements memRequirements = device->getBufferMemoryRequirements(*uniformBuffers[i]);
                vk::MemoryAllocateInfo allocInfo(
                    memRequirements.size,
                    findMemoryType(memRequirements.memoryTypeBits,
                        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent)
                );

                // 🔥 Store the memory in the vector
                uniformBuffersMemory[i] = device->allocateMemoryUnique(allocInfo).value;
                (void)device->bindBufferMemory(*uniformBuffers[i], *uniformBuffersMemory[i], 0);

                // Map the memory
                auto mapResult = device->mapMemory(*uniformBuffersMemory[i], 0, sizeof(UniformBufferObject));
                if (mapResult.result != vk::Result::eSuccess) {
                    throw std::runtime_error("Failed to map uniform buffer memory!");
                }
                uniformBuffersMapped[i] = mapResult.value;
            }
        }

        // Update uniform buffer
        void updateUniformBuffer(uint32_t currentImage, const Camera::State& camera) {
            static auto startTime = std::chrono::high_resolution_clock::now();
            auto currentTime = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration<float>(currentTime - startTime).count();

            UniformBufferObject ubo{};

            // Apply controlled cube rotation
            ubo.model = glm::rotate(glm::mat4(1.0f), (cubeRotation*time), glm::vec3(0.0f, 0.0f, 1.0f));

            // Use functional approach to get the view matrix
            ubo.view = Camera::getViewMatrix(camera);
           // std::cout << "View Matrix:\n" << glm::to_string(ubo.view) << std::endl;

            // Set up perspective projection (flip Y for Vulkan)
            glm::mat4 proj = glm::perspective(glm::radians(45.0f),
                swapChainExtent.width / static_cast<float>(swapChainExtent.height),
                0.1f, 10.0f);
            proj[1][1] *= -1;
            ubo.proj = proj;

            memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
        }

        // Texture PLURAL samplers
        void createTextureSamplers() {
            textureSamplers.resize(swapChainImages.size());

            // Create texture samplers for each image (this is an example, adjust as needed)
            for (size_t i = 0; i < swapChainImages.size(); i++) {
                vk::SamplerCreateInfo samplerInfo({}, vk::Filter::eLinear, vk::Filter::eLinear,
                    vk::SamplerMipmapMode::eLinear, vk::SamplerAddressMode::eRepeat,
                    vk::SamplerAddressMode::eRepeat, vk::SamplerAddressMode::eRepeat,
                    0.0f, VK_FALSE, 16.0f, VK_FALSE, vk::CompareOp::eNever,
                    0.0f, 0.0f, vk::BorderColor::eIntOpaqueBlack, VK_FALSE);

                auto result = device->createSamplerUnique(samplerInfo);
                if (result.result != vk::Result::eSuccess) {
                    throw std::runtime_error("Failed to create texture sampler!");
                }
                textureSamplers[i] = std::move(result.value);  // Move ownership
            }
        }

        // Example function to create image views for textures
        void createTextureImageViews() {
            textureImageViews.resize(textures.size());  // textures is a vector of vk::UniqueImage objects

            for (size_t i = 0; i < textures.size(); i++) {
                // Dereference the Unique handle to pass a raw vk::Image to createImageViewUnique
                textureImageViews[i] = createImageViewUnique(*textures[i], textureFormat);
            }
        }

        // Allocate Descriptor Sets
        void createDescriptorSets() {
            std::vector<vk::DescriptorSetLayout> layouts(swapChainImages.size(), *descriptorSetLayout);
            vk::DescriptorSetAllocateInfo allocInfo(*descriptorPool, static_cast<uint32_t>(swapChainImages.size()), layouts.data());

            auto result = device->allocateDescriptorSetsUnique(allocInfo);
            if (result.result != vk::Result::eSuccess) {
                throw std::runtime_error("Failed to allocate descriptor sets!");
            }
            descriptorSets = std::move(result.value);

            for (size_t i = 0; i < swapChainImages.size(); ++i) {
                vk::DescriptorBufferInfo bufferInfo(*uniformBuffers[i], 0, sizeof(UniformBufferObject));
                vk::DescriptorImageInfo imageInfo(*textureSampler, *textureImageView, vk::ImageLayout::eShaderReadOnlyOptimal);

                std::array<vk::WriteDescriptorSet, 2> descriptorWrites{};
                descriptorWrites[0] = { *descriptorSets[i], 0, 0, 1, vk::DescriptorType::eUniformBuffer, nullptr, &bufferInfo };
                descriptorWrites[1] = { *descriptorSets[i], 1, 0, 1, vk::DescriptorType::eCombinedImageSampler, &imageInfo };

                device->updateDescriptorSets((uint32_t)descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
            }
        }

        // Descriptor Set Layout
        void createDescriptorSetLayout() {
            std::array<vk::DescriptorSetLayoutBinding, 2> bindings{};

            // Binding for the uniform buffer (vertex shader)
            bindings[0] = { 0, vk::DescriptorType::eUniformBuffer, 1,
                            vk::ShaderStageFlagBits::eVertex };

            // Binding for the combined image sampler (fragment shader)
            bindings[1] = { 1, vk::DescriptorType::eCombinedImageSampler, 1,
                            vk::ShaderStageFlagBits::eFragment };

            vk::DescriptorSetLayoutCreateInfo layoutInfo({}, (uint32_t)bindings.size(), bindings.data());

            // Create the descriptor set layout and check the result
            try {
                // Store the unique descriptor set layout here, Correct assignment with .value
                descriptorSetLayout = device->createDescriptorSetLayoutUnique(layoutInfo).value;
            }
            catch (const std::exception& e) {
                throw std::runtime_error("Failed to create descriptor set layout: " + std::string(e.what()));
            }
        }

        // Descriptor Pool and Descriptor Set Allocation
        void createDescriptorPool() {
            std::array<vk::DescriptorPoolSize, 2> poolSizes{};

            // Pool size for uniform buffer descriptors
            poolSizes[0] = { vk::DescriptorType::eUniformBuffer, static_cast<uint32_t>(swapChainImages.size()) };

            // Pool size for combined image sampler descriptors
            poolSizes[1] = { vk::DescriptorType::eCombinedImageSampler, static_cast<uint32_t>(swapChainImages.size()) };

            // Create DescriptorPoolCreateInfo with explicit casting
            vk::DescriptorPoolCreateInfo poolInfo(
                vk::DescriptorPoolCreateFlags(0),     // No special flags
                static_cast<uint32_t>(swapChainImages.size()), // Max sets
                static_cast<uint32_t>(poolSizes.size()),        // Number of pool sizes
                poolSizes.data()                       // Pointer to pool sizes
            );

            try {
                // Store the unique descriptor pool here
                descriptorPool = device->createDescriptorPoolUnique(poolInfo).value;
            }
            catch (const std::exception& e) {
                throw std::runtime_error("Failed to create descriptor pool: " + std::string(e.what()));
            }
        }

        // Sync objects
        void createSyncObjects() {
            // Resize vectors to hold two elements each (double buffering)
            imageAvailableSemaphores.resize(2);
            renderFinishedSemaphores.resize(2);
            inFlightFences.resize(2);

            // Semaphore and fence creation info
            vk::SemaphoreCreateInfo semaphoreInfo;
            vk::FenceCreateInfo fenceInfo(vk::FenceCreateFlagBits::eSignaled);

            for (size_t i = 0; i < 2; i++) {
                try {
                    // Create semaphores and fences using unique pointers
                    auto imageAvailableResult = device->createSemaphoreUnique(semaphoreInfo);
                    if (imageAvailableResult.result != vk::Result::eSuccess) {
                        throw std::runtime_error("Failed to create image available semaphore!");
                    }
                    imageAvailableSemaphores[i] = std::move(imageAvailableResult.value);

                    auto renderFinishedResult = device->createSemaphoreUnique(semaphoreInfo);
                    if (renderFinishedResult.result != vk::Result::eSuccess) {
                        throw std::runtime_error("Failed to create render finished semaphore!");
                    }
                    renderFinishedSemaphores[i] = std::move(renderFinishedResult.value);

                    auto fenceResult = device->createFenceUnique(fenceInfo);
                    if (fenceResult.result != vk::Result::eSuccess) {
                        throw std::runtime_error("Failed to create fence!");
                    }
                    inFlightFences[i] = std::move(fenceResult.value);

                }
                catch (const std::exception& e) {
                    std::cerr << "Error during synchronization object creation: " << e.what() << std::endl;
                    throw;  // Re-throw to propagate the error
                }
            }
        }

        // Helper function to start recording
        vk::UniqueCommandBuffer beginSingleTimeCommands() {
            vk::CommandBufferAllocateInfo allocInfo(
                *commandPool,  // Your command pool
                vk::CommandBufferLevel::ePrimary,
                1
            );

            // Allocate and begin recording
            vk::UniqueCommandBuffer cmdBuffer =
                std::move(device.get().allocateCommandBuffersUnique(allocInfo).value[0]);

            vk::CommandBufferBeginInfo beginInfo{
                vk::CommandBufferUsageFlagBits::eOneTimeSubmit
            };
            (void)cmdBuffer->begin(beginInfo);

            return cmdBuffer; // Ownership transferred via move
        }

        // Helper to submit commands
        void endSingleTimeCommands(vk::UniqueCommandBuffer& cmdBuffer) {
            (void)cmdBuffer->end();

            vk::SubmitInfo submitInfo;
            submitInfo.setCommandBuffers(*cmdBuffer);
            (void)graphicsQueue.submit(submitInfo, nullptr);
            (void)graphicsQueue.waitIdle(); // Or use fences for async operation
        }

        // Image transitions
        void transitionImageLayout(vk::Image image, vk::Format format, vk::ImageLayout oldLayout, vk::ImageLayout newLayout) {
            vk::UniqueCommandBuffer commandBuffer = beginSingleTimeCommands(); // Your function for command buffer allocation

            vk::ImageMemoryBarrier barrier{};
            barrier.oldLayout = oldLayout;
            barrier.newLayout = newLayout;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = image;

            // Handle depth-stencil formats properly
            if (format == vk::Format::eD32Sfloat || format == vk::Format::eD16Unorm) {
                barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth;
            }
            else {
                barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eDepth | vk::ImageAspectFlagBits::eStencil;
            }

            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;

            vk::PipelineStageFlags sourceStage;
            vk::PipelineStageFlags destinationStage;

            if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
                barrier.srcAccessMask = vk::AccessFlags{}; // Explicit zero-initialization
                barrier.dstAccessMask = vk::AccessFlagBits::eDepthStencilAttachmentRead | vk::AccessFlagBits::eDepthStencilAttachmentWrite;

                sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
                destinationStage = vk::PipelineStageFlagBits::eEarlyFragmentTests;
            }
            // Handle TransferDstOptimal case
            else if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
                barrier.srcAccessMask = vk::AccessFlags{}; // Explicit zero-initialization
                barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

                sourceStage = vk::PipelineStageFlagBits::eTopOfPipe;
                destinationStage = vk::PipelineStageFlagBits::eTransfer;
            }
            // Handle transition from TransferDstOptimal to ShaderReadOnlyOptimal
            else if (oldLayout == vk::ImageLayout::eTransferDstOptimal && newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
                barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
                barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;

                sourceStage = vk::PipelineStageFlagBits::eTransfer;
                destinationStage = vk::PipelineStageFlagBits::eFragmentShader;
            }
            else {
                std::cerr << "Unsupported transition: "
                    << vk::to_string(oldLayout)
                    << " -> "
                    << vk::to_string(newLayout)
                    << std::endl;
                throw std::invalid_argument("Unsupported layout transition for depth image!");
            }

            commandBuffer.get().pipelineBarrier(sourceStage, destinationStage, {}, {}, {}, barrier);

            endSingleTimeCommands(commandBuffer); // Function to submit and free the command buffer
        }

        // Buffer/image copying
        void copyBufferToImage(vk::Buffer& buffer, vk::Image image, uint32_t width, uint32_t height) {
            vk::UniqueCommandBuffer commandBuffer = beginSingleTimeCommands();

            vk::BufferImageCopy region(
                0, 0, 0,
                vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1),
                { 0, 0, 0 }, { width, height, 1 });

            commandBuffer->copyBufferToImage(buffer, image, vk::ImageLayout::eTransferDstOptimal, 1, &region);
            endSingleTimeCommands(commandBuffer);
        }

        // Draw frame implementation
        void drawFrame() {
            
            (void)device->waitForFences(1, &*inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

            // Acquire image index
            uint32_t imageIndex;
            auto result = device->acquireNextImageKHR(*swapChain, UINT64_MAX,
                *imageAvailableSemaphores[currentFrame], nullptr, &imageIndex);

            // camera might need to go here

            // Validate image index
            if (imageIndex >= swapChainImages.size()) {
                throw std::runtime_error("Acquired image index out of bounds!");
            }

            // Update UBO only if index is valid
            updateUniformBuffer(imageIndex, cam);


            if (result == vk::Result::eErrorOutOfDateKHR || framebufferResized) {
                framebufferResized = false;
                recreateSwapChain();
                return;
            }

            (void)device->resetFences(1, &*inFlightFences[currentFrame]);

            vk::SubmitInfo submitInfo(
                1, &*imageAvailableSemaphores[currentFrame],
                nullptr,
                1, &*commandBuffers[imageIndex],
                1, &*renderFinishedSemaphores[currentFrame]);

            (void)graphicsQueue.submit(1, &submitInfo, *inFlightFences[currentFrame]);

            vk::PresentInfoKHR presentInfo(
                1, &*renderFinishedSemaphores[currentFrame],
                1, &*swapChain, &imageIndex);

            result = presentQueue.presentKHR(&presentInfo);

            if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR) {
                recreateSwapChain();
            }

            currentFrame = (currentFrame + 1) % 2;
        }

        // Swap chain recreation
        void recreateSwapChain() {
            int width = 0, height = 0;
            glfwGetFramebufferSize(window, &width, &height);
            while (width == 0 || height == 0) {
                glfwGetFramebufferSize(window, &width, &height);
                glfwWaitEvents();
            }

            (void)device->waitIdle();

            cleanupSwapChain();

            createSwapChain();
            createImageViews();
            createRenderPass();      // Recreate render pass
            createGraphicsPipeline();// Recreate pipeline
            createDepthResources();
            createFramebuffers();
            createUniformBuffers();  // Recreate uniform buffers
            createDescriptorPool();  // Recreate descriptor pool
            createDescriptorSets();
            createCommandBuffers();
        }

        void cleanupSwapChain() {
            framebuffers.clear();
            swapChainImageViews.clear();
            swapChain.reset();

            // Clear depth resources
            depthImageView.reset();
            depthImageMemory.reset();
            depthImage.reset();

            // Clear uniform buffers
            uniformBuffers.clear();
            uniformBuffersMemory.clear(); // 🔥 Add this line
            uniformBuffersMapped.clear();
        }

        // File reading utility
        static std::vector<char> readFile(const std::string& filename) {
            std::ifstream file(filename, std::ios::ate | std::ios::binary);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file: " + filename);
            }

            size_t fileSize = (size_t)file.tellg();
            std::vector<char> buffer(fileSize);
            file.seekg(0);
            file.read(buffer.data(), fileSize);
            file.close();

            return buffer;
        }

        void mainLoop() {
            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();

                // Update camera here (if not already updated via callbacks)
                // For example, you might have an updateCamera(deltaTime) function:
                float deltaTime = calculateDeltaTime(); // Compute frame delta time
                updateCamera(deltaTime);  // This function would internally call Camera::processKeyboard, etc.

                drawFrame();
            }
            device->waitIdle();
        }

        void cleanup() {
            (void)device->waitIdle();
            cleanupSwapChain();

            // Clear command buffers and descriptor sets so they're destroyed before the command pool.
            commandBuffers.clear();
            descriptorSets.clear();

            // Destroy buffers (vertex, index, uniform)
            vertexBuffer.reset();
            vertexBufferMemory.reset();
            indexBuffer.reset();
            indexBufferMemory.reset();
            uniformBuffer.reset();
            uniformBufferMemory.reset();

            // Destroy swapchain images and image views explicitly
            swapChainImageViews.clear();
            swapChainImages.clear();
            swapChain.reset();

            // Destroy depth resources
            depthImageView.reset();
            depthImageMemory.reset();
            depthImage.reset();

            // Destroy texture resources
            textureSampler.reset();
            textureImageView.reset();
            textureImageMemory.reset();
            textureImage.reset();

            // Reset descriptor pool and layout
            descriptorPool.reset();
            descriptorSetLayout.reset();

            // Destroy pipeline and related objects
            pipelineLayout.reset();
            graphicsPipeline.reset();
            renderPass.reset();

            // Destroy command pool after command buffers have been cleared
            commandPool.reset();

            // Destroy synchronization objects
            for (size_t i = 0; i < 2; i++) {
                inFlightFences[i].reset();
                renderFinishedSemaphores[i].reset();
                imageAvailableSemaphores[i].reset();
            }

            // Now finally destroy the device, surface, and instance.
            device.reset();
            surface.reset();
            instance.reset();

            glfwDestroyWindow(window);
            glfwTerminate();
        }


    };
}

#ifdef VULKAN_CUBE_MAIN
int main() {
    VulkanCube::Application app;
    try {
        app.run();
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
#endif
