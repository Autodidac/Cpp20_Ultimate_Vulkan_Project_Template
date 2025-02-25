// VulkanDevice.hpp - Header-only Vulkan Device creation and management
#pragma once
#include <vulkan/vulkan.h>
#include "crossvulkandispatch.hpp"

namespace epoch::vulkan {

    // Creates a Vulkan logical device using the instance dispatch table.
    inline auto createDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo& createInfo, const InstanceDispatchTable& instTable) -> VkDevice {
        VkDevice device = VK_NULL_HANDLE;
        if (instTable.vkCreateDevice && physicalDevice) {
            if (instTable.vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) == VK_SUCCESS) {
                return device;
            }
        }
        return VK_NULL_HANDLE;
    }

    // Destroy a Vulkan device using its dispatch table.
    inline void destroyDevice(VkDevice device, const DeviceDispatchTable& table) {
        if (table.vkDestroyDevice && device) {
            table.vkDestroyDevice(device, nullptr);
        }
    }

}
