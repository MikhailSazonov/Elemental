#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "LogicalDevice.hpp"
#include "SwapChain.hpp"
#include "CommandManager.hpp"
#include "Window.hpp"
#include "VertexBuffer.hpp"

namespace Graphic {
    class Operator {
        public:
            Operator(LogicalDevice& device, Window& window);

            ~Operator();

            bool drawFrame(SwapChain& swapChain, CommandManager& manager, Graphic::VertexBuffer& buffer);

        private:
            LogicalDevice& device;
            Window& window;

            std::vector<VkSemaphore> imageAvailableSemaphores;
            std::vector<VkSemaphore> renderFinishedSemaphores;
            std::vector<VkFence> inFlightFences;

            uint32_t current_frame{0};
    };
}
