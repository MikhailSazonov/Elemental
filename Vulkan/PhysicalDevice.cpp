#include "PhysicalDevice.hpp"

#include <iostream>

Graphic::PhysicalDevice::PhysicalDevice(Graphic::Instance& instance, Graphic::QueueFamilyIndices& indices, Surface& surface)
    : instance(instance), indices(indices) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance.getRawInstance(), &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance.getRawInstance(), &deviceCount, devices.data());

    for (auto& device : devices) {
        if (isDeviceSuitable(device, surface)) {
            physicalDevice = device;
            return;
        }
    }

    throw std::runtime_error("failed to find a suitable GPU!");
}

bool Graphic::PhysicalDevice::isDeviceSuitable(VkPhysicalDevice& device, Graphic::Surface& surface) {
    indices.findQueueFamilies(device, surface);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported) {
        Graphic::SwapChainSupportDetails swapChainSupport = Utils::querySwapChainSupport(device, surface);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.IsComplete() && extensionsSupported && swapChainAdequate;
}

bool Graphic::PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice& device) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (auto& req_ext : requiredExtensions) {
        bool not_found = true;
        for (auto& av_ext : availableExtensions) {
            if (av_ext.extensionName == req_ext) {
                not_found = false;
                break;
            }
        }
        if (not_found) {
            return false;
        }
    }
    return true;
}
