#include "first_app.hpp"

#include <array>

namespace TEn {

FirstApp::FirstApp()
    : tenWindow(this->WIDTH, this->HEIGHT, "Salut Vulkan !"),
      tenDevice(this->tenWindow),
      tenSwapChain(this->tenDevice, this->tenWindow.getExtent()) {
    loadModels();
    createPipelineLayout();
    createPipeline();
    createCommandBuffer();
}

FirstApp::~FirstApp() { vkDestroyPipelineLayout(tenDevice.device(), pipelineLayout, nullptr); }

void FirstApp::run() {
    while (!this->tenWindow.shouldClose()) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(tenDevice.device());
}

void FirstApp::loadModels() {
    std::vector<TEnModel::Vertex> triangle{{{0.0f, -0.9f}}, {{0.9f, 0.9f}}, {{-0.9f, 0.9f}}};
    auto vertices = TEnModel::sierpinskiTriangle(triangle, 6);

    tenModel = std::make_unique<TEnModel>(tenDevice, vertices);
}

void FirstApp::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(this->tenDevice.device(), &pipelineLayoutInfo, nullptr,
                               &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout.");
    }
}

void FirstApp::createPipeline() {
    auto pipelineConfig =
        TEnPipeline::defaultPipelineConfigInfo(tenSwapChain.width(), tenSwapChain.height());
    pipelineConfig.renderPass = tenSwapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    tenPipeline = std::make_unique<TEnPipeline>(tenDevice, "shaders/simple_shader.vert.spv",
                                                "shaders/simple_shader.frag.spv", pipelineConfig);
}

void FirstApp::createCommandBuffer() {
    commandBuffers.resize(tenSwapChain.imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = tenDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(tenDevice.device(), &allocInfo, commandBuffers.data()) !=
        VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers.");
    }

    for (int i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("Failder to begin recording command buffer.");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = tenSwapChain.getRenderPass();
        renderPassInfo.framebuffer = tenSwapChain.getFrameBuffer(i);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = tenSwapChain.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        tenPipeline->bind(commandBuffers[i]);
        tenModel->bind(commandBuffers[i]);
        tenModel->draw(commandBuffers[i]);

        vkCmdEndRenderPass(commandBuffers[i]);
        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to record command buffer.");
        }
    }
}

void FirstApp::drawFrame() {
    uint32_t imageIndex;
    auto result = tenSwapChain.acquireNextImage(&imageIndex);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image.");
    }

    result = tenSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image.");
    }
}

}  // namespace TEn