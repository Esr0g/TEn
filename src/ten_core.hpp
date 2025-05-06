#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

namespace TEn {

const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

class TEnCore {
   public:
    TEnCore(std::string name);
    ~TEnCore();

    TEnCore(const TEnCore &) = delete;
    TEnCore &operator=(const TEnCore &) = delete;

    VkInstance &instance();
    bool isValidationLayersEnabled() { return enableValidationLayers; }

   private:
    void createInstance();
    void setupDebugMessenger();

    std::vector<const char *> getRequiredExtensions();
    static bool checkValidationLayerSupport();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
    static VKAPI_ATTR VkBool32 VKAPI_CALL
    debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                  const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData);
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
                                          const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                          const VkAllocationCallbacks *pAllocator,
                                          VkDebugUtilsMessengerEXT *pDebugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                       const VkAllocationCallbacks *pAllocator);
    static std::string pickColoBySeverity(
        const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity);

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    VkDebugUtilsMessengerEXT debugMessenger;
    VkInstance _instance = VK_NULL_HANDLE;
    std::string applicationName;
};
}  // namespace TEn