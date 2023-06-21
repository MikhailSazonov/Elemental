#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <optional>
#include <vector>

#include "Surface.hpp"

namespace Graphic {

class LogicalDevice;
class Surface;

class QueueFamilyIndices {
    friend class LogicalDevice;

    friend class SwapChain;

    public:
        void findQueueFamilies(VkPhysicalDevice& device, Surface& surface);

        inline bool IsComplete() {
            return presentFamily.has_value() && graphicsFamily.has_value();
        }

    public:
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;
};

}
