#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Utils.hpp"
#include "Instance.hpp"

namespace Graphic {

    class DebugMessenger {
        public:
            DebugMessenger(Instance& instance);

            ~DebugMessenger();

        private:
            VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

            void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
            const VkAllocationCallbacks* pAllocator);

        private:
            VkDebugUtilsMessengerEXT debugMessenger;
            Instance& instance;
    };

}