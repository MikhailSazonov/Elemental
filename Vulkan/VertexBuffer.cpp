#include "VertexBuffer.hpp"

#include <iostream>

uint32_t Graphic::VertexBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(phys_device.getRawDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

Graphic::VertexBuffer::VertexBuffer(LogicalDevice& device, PhysicalDevice& phys_device)
    : device(device), phys_device(phys_device) {
    createVertexBuffer();
}

void Graphic::VertexBuffer::createVertexBuffer() {
    auto bufferInfo = createVertexBufferInfo(1000);

    if (vkCreateBuffer(device.getRawDevice(), &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    vkGetBufferMemoryRequirements(device.getRawDevice(), vertexBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(device.getRawDevice(), &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(device.getRawDevice(), vertexBuffer, vertexBufferMemory, 0);

}

Graphic::VertexBuffer::~VertexBuffer() {
    vkDestroyBuffer(device.getRawDevice(), vertexBuffer, nullptr);
    vkFreeMemory(device.getRawDevice(), vertexBufferMemory, nullptr);
}

VkBufferCreateInfo Graphic::VertexBuffer::createVertexBufferInfo(size_t amount) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(Graphic::Vertex) * amount;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    return bufferInfo;
}

void Graphic::VertexBuffer::fillBuffer() {
    auto bufferInfo = createVertexBufferInfo(vertices_to_draw.size());
    void* data;
    vkMapMemory(device.getRawDevice(), vertexBufferMemory, 0, bufferInfo.size, 0, &data);
    memcpy(data, vertices_to_draw.data(), (size_t) bufferInfo.size);
    vkUnmapMemory(device.getRawDevice(), vertexBufferMemory);
}


void Graphic::VertexBuffer::pushVertex(Graphic::Vertex&& vertex) {
    vertex.pos[0] -= 0.7;
    vertex.pos[0] *= 2;
    vertex.pos[1] *= 2;
    vertices_to_draw.push_back(std::move(vertex));
}


// for (auto& vert : vertices_to_draw) {
//     std::cout << vert.pos[0] << ' ' << vert.pos[1] << '\n';
//     std::cout << vert.color[0] << ' ' << vert.color[1] << ' ' << vert.color[2] << '\n';
//     std::cout << "---------\n";
// }
// std::cout << "=========\n";
