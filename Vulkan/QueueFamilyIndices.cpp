#include "QueueFamilyIndices.hpp"

void Graphic::QueueFamilyIndices::findQueueFamilies(VkPhysicalDevice& device, Graphic::Surface& surface) {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;

    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsFamily = i;

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface.getRawSurface(), &presentSupport);

            if (presentSupport) {
                presentFamily = i;
                return;
            }

        }

        i++;
    }

    throw std::runtime_error("failed to find queue family!");
}
