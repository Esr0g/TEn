#include "ten_core.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "utils.hpp"

namespace TEn {

TEnCore::TEnCore(std::string name) : applicationName(name) {
    createInstance();
    setupDebugMessenger();
}

TEnCore::~TEnCore() {
    if (enableValidationLayers) {
        DestroyDebugUtilsMessengerEXT(_instance, debugMessenger, nullptr);
    }

    vkDestroyInstance(_instance, nullptr);

    glfwTerminate();
}

VkInstance &TEnCore::instance() { return _instance; }

void TEnCore::createInstance() {
    if (enableValidationLayers && !checkValidationLayerSupport()) {
        throw vkRuntimeError("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = applicationName.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
    } else {
        createInfo.enabledLayerCount = 0;

        createInfo.pNext = nullptr;
    }

    VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);
    if (result != VK_SUCCESS) {
        throw vkRuntimeError("failed to create instance!", result);
    }
}

void TEnCore::setupDebugMessenger() {
    if (!enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    VkResult result =
        CreateDebugUtilsMessengerEXT(_instance, &createInfo, nullptr, &debugMessenger);
    if (result != VK_SUCCESS) {
        throw vkRuntimeError("failed to set up debug messenger!", result);
    }
}

std::vector<const char *> TEnCore::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    if (glfwExtensionCount == 0) {
        throw vkRuntimeError(
            "An error occured while getting required instance extensions. GLFW might not be "
            "initialized.");
    }

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

bool TEnCore::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName : validationLayers) {
        bool layerFound = false;

        auto it = std::find_if(availableLayers.begin(), availableLayers.end(),
                               [layerName](const VkLayerProperties &layerProperties) {
                                   return std::strcmp(layerProperties.layerName, layerName) == 0;
                               });
        if (it == availableLayers.end()) {
            return false;
        }
    }

    return true;
}

void TEnCore::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
}

VKAPI_ATTR VkBool32 VKAPI_CALL
TEnCore::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                       VkDebugUtilsMessageTypeFlagsEXT messageType,
                       const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream timeStream;
    timeStream << "[" << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << "] ";

    std::string colorCode = pickColoBySeverity(messageSeverity);

    std::cerr << colorCode << timeStream.str() << "[Validation layer]: " << pCallbackData->pMessage
              << "\033[0m" << std::endl;

    return VK_FALSE;
}

VkResult TEnCore::CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void TEnCore::DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                            VkDebugUtilsMessengerEXT debugMessenger,
                                            const VkAllocationCallbacks *pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

std::string TEnCore::pickColoBySeverity(
    const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity) {
    std::string colorCode;
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            colorCode = "\033[0;37m";  // Gris clair
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            colorCode = "\033[0;37m";  // Blanc
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            colorCode = "\033[0;33m";  // Orange
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            colorCode = "\033[0;31m";  // Rouge
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
            colorCode = "\033[1;31m";  // Rouge
            break;
        default:
            colorCode = "\033[0;35m";
            break;
    }
    return colorCode;
}

}  // namespace TEn
