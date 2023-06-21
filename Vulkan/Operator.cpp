#include "Operator.hpp"

Graphic::Operator::Operator(LogicalDevice& device, Window& window)
    : device(device), window(window) {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {

        if (vkCreateSemaphore(device.getRawDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
        vkCreateSemaphore(device.getRawDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
        vkCreateFence(device.getRawDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create semaphores!");
        }
    }
}

Graphic::Operator::~Operator() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
        vkDestroySemaphore(device.getRawDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(device.getRawDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroyFence(device.getRawDevice(), inFlightFences[i], nullptr);
    }
}

bool Graphic::Operator::drawFrame(Graphic::SwapChain& swapChain, Graphic::CommandManager& manager, Graphic::VertexBuffer& buffer) {
    vkWaitForFences(device.getRawDevice(), 1, &inFlightFences[current_frame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device.getRawDevice(), swapChain.getSwapChainRaw(),
    UINT64_MAX, imageAvailableSemaphores[current_frame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.framebufferResized) {
        window.framebufferResized = false;
        return false;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vkResetFences(device.getRawDevice(), 1, &inFlightFences[current_frame]);

    vkResetCommandBuffer(manager.getCommandBuffers()[current_frame], 0);
    manager.recordCommandBuffer(imageIndex, current_frame, buffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[current_frame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &manager.getCommandBuffers()[current_frame];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[current_frame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, inFlightFences[current_frame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain.getSwapChainRaw()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    presentInfo.pResults = nullptr;

    result = vkQueuePresentKHR(device.getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        return false;
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;

    return true;

}
