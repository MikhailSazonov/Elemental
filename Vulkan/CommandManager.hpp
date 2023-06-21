#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "QueueFamilyIndices.hpp"
#include "LogicalDevice.hpp"
#include "VertexBuffer.hpp"

namespace Graphic {
    class CommandManager {
        public:
            CommandManager(QueueFamilyIndices& indices, LogicalDevice& device,
            VkPipeline& graphicsPipeline, VkViewport& viewport, VkRect2D& scissor,
            VkRenderPass& renderPass, std::vector<VkFramebuffer>& swapChainFramebuffers,
            VkExtent2D swapChainExtent);

            ~CommandManager();

            void Draw();

            void recordCommandBuffer(uint32_t imageIndex, uint32_t current_frame, VertexBuffer& buffer);

            void createCommandBuffers();

        private:
            LogicalDevice& device;
            VkPipeline& graphicsPipeline;

            VkCommandPool commandPool;

            std::vector<VkCommandBuffer> commandBuffers;

            VkRenderPass& renderPass;
            std::vector<VkFramebuffer>& swapChainFramebuffers;
            VkExtent2D swapChainExtent;

        public:
            inline std::vector<VkCommandBuffer>& getCommandBuffers() { return commandBuffers; }
    };
}