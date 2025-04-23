#pragma once

#include <string>
#include <vector>

#include "ten_device.hpp"

namespace TEn {

struct PipelineConfigInfo {
    VkViewport viewport;
    VkRect2D scissor;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
};

class TEnPipeline {
   public:
    TEnPipeline(TEnDevice& device, const std::string& vertFilepath, const std::string& fragFilepath,
                const PipelineConfigInfo& configInfo);
    ~TEnPipeline();

    TEnPipeline(const TEnPipeline&) = delete;
    void operator=(const TEnPipeline&) = delete;

    void bind(VkCommandBuffer commandBuffer);

    static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

   private:
    static std::vector<char> readFile(const std::string& filepath);
    void createGraphicsPipeline(const std::string vertFilepath, const std::string fragFilePath,
                                const PipelineConfigInfo configInfo);
    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    TEnDevice& tenDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};
}  // namespace TEn