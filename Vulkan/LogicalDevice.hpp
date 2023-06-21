#pragma once

#include "PhysicalDevice.hpp"
#include "QueueFamilyIndices.hpp"

namespace Graphic {
    class PhysicalDevice;

    class LogicalDevice {
        public:
            LogicalDevice(PhysicalDevice& physical_device, QueueFamilyIndices& indices);

            inline VkDevice& getRawDevice() { return device; }

            ~LogicalDevice();

        private:
            VkDeviceCreateInfo createLogicalDeviceInfo(VkPhysicalDeviceFeatures& deviceFeatures,
            std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos);

            VkDeviceQueueCreateInfo createQueueInfo(QueueFamilyIndices& indices,
            float& queuePriority, uint32_t queueFamily);

        private:
            PhysicalDevice& physical_device;
            QueueFamilyIndices& indices;

            VkDevice device;
            VkQueue graphicsQueue;
            VkQueue presentQueue;

        public:
            VkQueue& getGraphicsQueue() { return graphicsQueue; }

            VkQueue& getPresentQueue() { return presentQueue; }
    };
}