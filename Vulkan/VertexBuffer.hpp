#pragma once

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#endif

#include <GLFW/glfw3.h>

#include "LogicalDevice.hpp"
#include "PhysicalDevice.hpp"

namespace Graphic {
    class VertexBuffer {
        public:
            VertexBuffer(LogicalDevice& device, PhysicalDevice& phys_device);

            VkBufferCreateInfo createVertexBufferInfo(size_t amount);

            uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

            void createVertexBuffer();

            void fillBuffer();

            ~VertexBuffer();

            void pushVertex(Vertex&& vertex);

            inline void resetVertices() { vertices_to_draw.clear(); }

        private:
            LogicalDevice& device;
            PhysicalDevice& phys_device;

            VkBuffer vertexBuffer;
            VkMemoryRequirements memRequirements;
            VkDeviceMemory vertexBufferMemory;

            std::vector<Vertex> vertices_to_draw;

        public:
            inline VkBuffer& getBuffer() { return vertexBuffer; }

            inline std::vector<Vertex>& getVerticesToDraw() { return vertices_to_draw; }
    };
}