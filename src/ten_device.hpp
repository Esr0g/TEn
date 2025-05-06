#pragma once
#include <optional>

#include "ten_core.hpp"
#include "ten_window.hpp"

namespace TEn {

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities{};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class TEnDevice {
   public:
    const int MAX_FRAMES_IN_FLIGHT = 2;
    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    TEnDevice(TEnWindow &window, TEnCore &core);
    ~TEnDevice();

    TEnDevice(const TEnDevice &) = delete;
    void operator=(const TEnDevice &) = delete;
    TEnDevice(TEnDevice &&) = delete;
    TEnDevice &operator=(TEnDevice &&) = delete;

    QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice &device);
    SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice &device);

    VkSurfaceKHR &surface() { return surface_; };
    VkDevice &device() { return device_; };
    VkPhysicalDevice &physicalDevice() { return physicalDevice_; };
    VkQueue &graphicsQueue() { return graphicsQueue_; };
    VkQueue &presentQueue() { return presentQueue_; };
    VkCommandPool &commandPool() { return commandPool_; };

   private:
    void pickPhysicalDevice();
    void createLogicalDevice();
    bool isDeviceSuitable(const VkPhysicalDevice &device);
    bool checkDeviceExtensionSupport(const VkPhysicalDevice &device);
    void createCommandPool();

    TEnWindow &tenWindow;
    TEnCore &tenCore;

    VkSurfaceKHR surface_;

    VkDevice device_ = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;

    VkQueue graphicsQueue_ = VK_NULL_HANDLE;
    VkQueue presentQueue_ = VK_NULL_HANDLE;

    VkCommandPool commandPool_ = VK_NULL_HANDLE;
};

}  // namespace TEn