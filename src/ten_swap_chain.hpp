#pragma once

#include "ten_device.hpp"
#include "ten_window.hpp"

namespace TEn {
class TEnSwapChain {
   public:
    TEnSwapChain(TEnWindow& window, TEnDevice& device);
    ~TEnSwapChain();

    TEnSwapChain(const TEnSwapChain&) = delete;
    TEnSwapChain& operator=(const TEnSwapChain&) = delete;

    VkRenderPass renderPass() { return renderPass_; };

    void acquireNextImage(uint32_t* imageIndex);
    void submitCommandBuffer(VkPipeline& pipeline, uint32_t* imageIndex);

   private:
    void createSwapChain();
    void cleanupSwapChain();
    void recreateSwapChain();
    void createImageViews();
    void createRenderPass();
    void createFramebuffers();
    void createSyncObjects();
    void createCommandBuffers();
    void recordCommandBuffer(VkPipeline& pipeline, VkCommandBuffer commandBuffer,
                             uint32_t* imageIndex);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capapilities);

    TEnWindow& tenWindow;
    TEnDevice& tenDevice;

    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;
    VkExtent2D swapChainExtent = {};

    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkRenderPass renderPass_ = VK_NULL_HANDLE;

    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    std::vector<VkCommandBuffer> commandBuffers;

    uint32_t currentFrame = 0;
    const int MAX_FRAMES_IN_FLIGHT = 2;
};
}  // namespace TEn