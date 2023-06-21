#pragma once

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif

#include <GLFW/glfw3.h>

const uint32_t WIDTH = 1000;
const uint32_t HEIGHT = 800;

namespace Graphic {
    class Window {
        public:
            Window();

            ~Window();

            inline bool shouldClose() {
                return glfwWindowShouldClose(window);
            }

            inline GLFWwindow* getRawWindow() { return window; }

        public:
            bool framebufferResized{false};

        private:
            GLFWwindow* window;
    };
}