#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>
#include <cstring>
#include <exception>

#include "Utils.hpp"

namespace Graphic {

    class Instance {
        public:
            Instance();

            ~Instance();

            VkInstance& getRawInstance();

        private:
            VkApplicationInfo createApplicationInfo();

            VkInstanceCreateInfo createInstanceInfo(VkApplicationInfo& appInfo);

            bool checkValidationLayerSupport();

            std::vector<const char*> getRequiredExtensions();

        private:
            VkInstance instance;
    };
}
