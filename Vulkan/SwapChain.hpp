#pragma once

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>

#include <cstdint>
#include <limits>
#include <algorithm>

#include "QueueFamilyIndices.hpp"
#include "PhysicalDevice.hpp"
#include "LogicalDevice.hpp"
#include "Surface.hpp"
#include "Window.hpp"
#include "Utils.hpp"

namespace Graphic {

class PhysicalDevice;
class Surface;

class SwapChain {
    public:
        SwapChain(PhysicalDevice& physicalDevice,
        LogicalDevice& device, Surface& surface,
        QueueFamilyIndices& indices,
        Window& window);

        void createSwapChain();

        ~SwapChain();

    private:
        VkSwapchainCreateInfoKHR createSwapChainInfo(VkExtent2D& extent, Surface& surface,
        uint32_t imageCount, VkSurfaceFormatKHR& surfaceFormat, QueueFamilyIndices& indices,
        VkPresentModeKHR& presentMode, SwapChainSupportDetails& swapChainSupport);

        VkImageViewCreateInfo createImageViewInfo(VkImage& image_view);

        // SAVINGS
        PhysicalDevice& physicalDevice;
        LogicalDevice& device;
        Surface& surface;
        QueueFamilyIndices& indices;
        Window& window;


        // OWN FIELDS
        VkSwapchainKHR swapChain;

        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;

        std::vector<VkImageView> swapChainImageViews;

    public:
        inline VkExtent2D& getSwapChainExtent() { return swapChainExtent; }

        inline VkFormat& getImageFormat() { return swapChainImageFormat; }

        inline std::vector<VkImageView>& getImageViews() { return swapChainImageViews; }

        inline VkSwapchainKHR& getSwapChainRaw() { return swapChain; }
};

}
