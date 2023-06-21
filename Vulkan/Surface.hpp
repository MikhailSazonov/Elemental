#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Instance.hpp"
#include "Window.hpp"

namespace Graphic {
    class Instance;

    class Surface {
        public:
            Surface(Instance& instance, Window& window);

            inline VkSurfaceKHR& getRawSurface() { return surface; }

            ~Surface();

        private:
            VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

        private:
            VkSurfaceKHR surface;
            Instance& instance;
            Window& window;
    };
}