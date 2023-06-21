#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Instance.hpp"
#include "Surface.hpp"
#include "QueueFamilyIndices.hpp"
#include "SwapChain.hpp"
#include "Utils.hpp"

#include <set>
#include <algorithm>

namespace Graphic {
    class PhysicalDevice {
        public:
            PhysicalDevice(Instance& instance, QueueFamilyIndices& indices, Surface& surface);

            inline VkPhysicalDevice& getRawDevice() { return physicalDevice; }

        private:
            bool isDeviceSuitable(VkPhysicalDevice& device, Surface& surface);

            bool checkDeviceExtensionSupport(VkPhysicalDevice& device);

            Instance& instance;
            QueueFamilyIndices& indices;

            VkPhysicalDevice physicalDevice{VK_NULL_HANDLE};

            VkQueue presentQueue;
    };
}
