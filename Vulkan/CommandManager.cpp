#include "CommandManager.hpp"

Graphic::CommandManager::CommandManager(Graphic::QueueFamilyIndices& indices, LogicalDevice& device, VkPipeline& graphicsPipeline,
    VkViewport& viewport, VkRect2D& scissor, VkRenderPass& renderPass,
    std::vector<VkFramebuffer>& swapChainFramebuffers, VkExtent2D swapChainExtent)
    : device(device), graphicsPipeline(graphicsPipeline),
    renderPass(renderPass), swapChainFramebuffers(swapChainFramebuffers),
    swapChainExtent(swapChainExtent) {

    // CREATE THE POOL
    QueueFamilyIndices queueFamilyIndices = indices;

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(device.getRawDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }

    // ALLOCATE THE BUFFERS
    createCommandBuffers();

}

void Graphic::CommandManager::createCommandBuffers() {
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(device.getRawDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

Graphic::CommandManager::~CommandManager() {
    vkDestroyCommandPool(device.getRawDevice(), commandPool, nullptr);
}

void Graphic::CommandManager::recordCommandBuffer(uint32_t imageIndex, uint32_t current_frame, Graphic::VertexBuffer& buffer) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;
    beginInfo.pInheritanceInfo = nullptr;

    if (vkBeginCommandBuffer(commandBuffers[current_frame], &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex];

    renderPassInfo.renderArea.offset = {0, 0};

    renderPassInfo.renderArea.extent = swapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffers[current_frame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffers[current_frame], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) swapChainExtent.width;
    viewport.height = (float) swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffers[current_frame], 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(commandBuffers[current_frame], 0, 1, &scissor);

    VkBuffer vertexBuffers[] = {buffer.getBuffer()};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffers[current_frame], 0, 1, vertexBuffers, offsets);

    vkCmdDraw(commandBuffers[current_frame], buffer.getVerticesToDraw().size(), 1, 0, 0);

    vkCmdEndRenderPass(commandBuffers[current_frame]);

    if (vkEndCommandBuffer(commandBuffers[current_frame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}
