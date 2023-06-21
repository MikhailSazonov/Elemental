#include "SwapChain.hpp"

#include <iostream>

Graphic::SwapChain::SwapChain(Graphic::PhysicalDevice& physicalDevice,
Graphic::LogicalDevice& device, Graphic::Surface& surface,
Graphic::QueueFamilyIndices& indices, Graphic::Window& window)
    : physicalDevice(physicalDevice),
      device(device), surface(surface),
      indices(indices), window(window) {
    createSwapChain();
}

void Graphic::SwapChain::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = Utils::querySwapChainSupport(physicalDevice.getRawDevice(), surface);

    VkSurfaceFormatKHR surfaceFormat = Utils::chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = Utils::chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = Utils::chooseSwapExtent(swapChainSupport.capabilities, window.getRawWindow());

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    auto swapChainInfo = createSwapChainInfo(extent, surface, imageCount, surfaceFormat, indices, presentMode, swapChainSupport);

    if (vkCreateSwapchainKHR(device.getRawDevice(), &swapChainInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(device.getRawDevice(), swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device.getRawDevice(), swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;


    swapChainImageViews.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        auto createInfo = createImageViewInfo(swapChainImages[i]);
        if (vkCreateImageView(device.getRawDevice(), &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
}

Graphic::SwapChain::~SwapChain() {
    vkDestroySwapchainKHR(device.getRawDevice(), swapChain, nullptr);

    for (auto& imageView : swapChainImageViews) {
        vkDestroyImageView(device.getRawDevice(), imageView, nullptr);
    }
}

VkImageViewCreateInfo Graphic::SwapChain::createImageViewInfo(VkImage& image_view) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = image_view;

    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = swapChainImageFormat;

    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    return createInfo;

}

VkSwapchainCreateInfoKHR Graphic::SwapChain::createSwapChainInfo(VkExtent2D& extent, Surface& surface,
uint32_t imageCount, VkSurfaceFormatKHR& surfaceFormat, Graphic::QueueFamilyIndices& indices,
VkPresentModeKHR& presentMode, Graphic::SwapChainSupportDetails& swapChainSupport) {
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.surface = surface.getRawSurface();
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = nullptr;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    return createInfo;
}
