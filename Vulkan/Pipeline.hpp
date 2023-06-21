#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Vertex.hpp"
#include "LogicalDevice.hpp"
#include "Utils.hpp"

namespace Graphic {
    class Pipeline {
        public:
            Pipeline(LogicalDevice& device, VkExtent2D& swapChainExtent, VkFormat& imageFormat, std::vector<VkImageView>& swapChainImageViews);

            void createFrameBuffers(VkExtent2D& swapChainExtent, std::vector<VkImageView>& swapChainImageViews);

            ~Pipeline();

        private:
            VkShaderModule createShaderModule(const std::vector<char>& code);

            VkPipelineShaderStageCreateInfo vertShaderStageCreateInfo();

            VkPipelineShaderStageCreateInfo fragStageCreateInfo();

            void initdynamicStateInfo();

            void initMultisamplingInfo();

            void initvertexPipelineVertexInfo();

            void initColorBlendAttachment();

            void initColorBlendInfo();

            void initPipelineLayout();

            void initRenderPass(VkFormat imageFormat);

            void initinputAssemblyInfo();

            void initviewportStateInfo(VkViewport& viewport, VkRect2D& scissor);

            void initRasterizerInfo();

            void initpipelineInfo(VkPipelineShaderStageCreateInfo* shaderStages);

        private:
            LogicalDevice& device;

            VkShaderModule vertShaderModule;
            VkShaderModule fragShaderModule;

            VkPipelineColorBlendAttachmentState colorBlendAttachment;

            VkRenderPass renderPass;
            VkPipelineLayout pipelineLayout;

            VkPipeline graphicsPipeline;

            VkAttachmentDescription colorAttachment{};


            // info

            VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
            VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
            VkPipelineViewportStateCreateInfo viewportState{};
            VkPipelineRasterizationStateCreateInfo rasterizer{};
            VkPipelineMultisampleStateCreateInfo multisampling{};
            VkPipelineColorBlendStateCreateInfo colorBlending{};
            VkPipelineDynamicStateCreateInfo dynamicState{};
            VkGraphicsPipelineCreateInfo pipelineInfo{};

            VkVertexInputBindingDescription bindingDescription{};
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};


            // framebuf

            std::vector<VkFramebuffer> swapChainFramebuffers;

            // viewport, scissors

            VkViewport viewport;
            VkRect2D scissor;

        public:
            inline VkPipeline& getGraphicsPipeline() { return graphicsPipeline; }

            inline VkViewport& getViewport() { return viewport; }

            inline VkRect2D& getScissor() { return scissor; }

            inline VkRenderPass& getRenderPass() { return renderPass; }

            inline std::vector<VkFramebuffer>& getSwapChainFrameBuffers() { return swapChainFramebuffers; }
    };
}
