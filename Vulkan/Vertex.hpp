#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <array>
#include <vector>

namespace Graphic {
    struct Vertex {
        glm::vec2 pos;
        glm::vec3 color;

    };

    VkVertexInputBindingDescription getBindingDescription();

    std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions();
}
