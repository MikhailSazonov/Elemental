#include "LogicalDevice.hpp"

Graphic::LogicalDevice::LogicalDevice(Graphic::PhysicalDevice& physical_device, Graphic::QueueFamilyIndices& indices)
    : physical_device(physical_device), indices(indices) {

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;

    for (uint32_t queueFamily : uniqueQueueFamilies) {
        auto queueCreateInfo = createQueueInfo(indices, queuePriority, queueFamily);
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};

    auto createInfo = createLogicalDeviceInfo(deviceFeatures, queueCreateInfos);

    if (vkCreateDevice(physical_device.getRawDevice(), &createInfo, nullptr, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

VkDeviceCreateInfo Graphic::LogicalDevice::createLogicalDeviceInfo(VkPhysicalDeviceFeatures& deviceFeatures,
std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos) {
    VkDeviceCreateInfo createInfo{};

    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    // createInfo.enabledExtensionCount = 0;

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    return createInfo;
}

VkDeviceQueueCreateInfo Graphic::LogicalDevice::createQueueInfo(QueueFamilyIndices& indices, float& queuePriority,
uint32_t queueFamily) {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;

    queueCreateInfo.pQueuePriorities = &queuePriority;

    return queueCreateInfo;
}

Graphic::LogicalDevice::~LogicalDevice() {
    vkDestroyDevice(device, nullptr);
}
