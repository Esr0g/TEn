#include "utils.hpp"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace TEn {

std::string vkResultToString(const VkResult result) {
    switch (result) {
        case VK_SUCCESS:
            return "VK_SUCCESS";
        case VK_NOT_READY:
            return "VK_NOT_READY";
        case VK_TIMEOUT:
            return "VK_TIMEOUT";
        case VK_EVENT_SET:
            return "VK_EVENT_SET";
        case VK_EVENT_RESET:
            return "VK_EVENT_RESET";
        case VK_INCOMPLETE:
            return "VK_INCOMPLETE";
        case VK_ERROR_OUT_OF_HOST_MEMORY:
            return "VK_ERROR_OUT_OF_HOST_MEMORY";
        case VK_ERROR_OUT_OF_DEVICE_MEMORY:
            return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
        case VK_ERROR_INITIALIZATION_FAILED:
            return "VK_ERROR_INITIALIZATION_FAILED";
        case VK_ERROR_DEVICE_LOST:
            return "VK_ERROR_DEVICE_LOST";
        case VK_ERROR_MEMORY_MAP_FAILED:
            return "VK_ERROR_MEMORY_MAP_FAILED";
        case VK_ERROR_LAYER_NOT_PRESENT:
            return "VK_ERROR_LAYER_NOT_PRESENT";
        case VK_ERROR_EXTENSION_NOT_PRESENT:
            return "VK_ERROR_EXTENSION_NOT_PRESENT";
        case VK_ERROR_FEATURE_NOT_PRESENT:
            return "VK_ERROR_FEATURE_NOT_PRESENT";
        case VK_ERROR_INCOMPATIBLE_DRIVER:
            return "VK_ERROR_INCOMPATIBLE_DRIVER";
        case VK_ERROR_TOO_MANY_OBJECTS:
            return "VK_ERROR_TOO_MANY_OBJECTS";
        case VK_ERROR_FORMAT_NOT_SUPPORTED:
            return "VK_ERROR_FORMAT_NOT_SUPPORTED";
        case VK_ERROR_FRAGMENTED_POOL:
            return "VK_ERROR_FRAGMENTED_POOL";
        case VK_ERROR_UNKNOWN:
            return "VK_ERROR_UNKNOWN";

        // Extension VK_KHR_surface
        case VK_ERROR_SURFACE_LOST_KHR:
            return "VK_ERROR_SURFACE_LOST_KHR";
        case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
            return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";

        // Extension VK_KHR_swapchain
        case VK_SUBOPTIMAL_KHR:
            return "VK_SUBOPTIMAL_KHR";
        case VK_ERROR_OUT_OF_DATE_KHR:
            return "VK_ERROR_OUT_OF_DATE_KHR";

        // Extension VK_EXT_debug_utils
        case VK_ERROR_VALIDATION_FAILED_EXT:
            return "VK_ERROR_VALIDATION_FAILED_EXT";

        // Extension VK_KHR_ray_tracing
        case VK_ERROR_INVALID_SHADER_NV:
            return "VK_ERROR_INVALID_SHADER_NV";

        // Extension VK_KHR_video_queue
        case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
            return "VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
            return "VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
            return "VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
            return "VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
            return "VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR";
        case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
            return "VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR";

        default:
            return "UNKNOWN_VK_RESULT (" + std::to_string(static_cast<int>(result)) + ")";
    }
}

std::runtime_error vkRuntimeError(const std::string& message,
                                  const std::optional<VkResult> result) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << "\033[1;31m";
    ss << "[" << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << "] ";

    if (result.has_value()) {
        ss << "[Runtime Error]: " << message
           << " | VkResult : " << vkResultToString(result.value());
    } else {
        ss << "[Runtime Error]: " << message;
    }
    ss << "\033[0m";
    ss << std::endl;

    return std::runtime_error(ss.str());
}

void log(const std::string& message, const Log messageSeverity) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    std::stringstream timeStream;
    timeStream << "[" << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S") << "] ";

    std::cerr << pickColorByLogSeverity(messageSeverity) << timeStream.str()
              << pickStartMessageByLogSeverity(messageSeverity) << message << "\033[0m"
              << std::endl;
}

std::string pickColorByLogSeverity(const Log messageSeverity) {
    std::string colorCode;

    switch (messageSeverity) {
        case INFO:
            colorCode = "\033[0;37m";  // Blanc
            break;
        case WARNING:
            colorCode = "\033[0;33m";  // Orange
            break;
        case ERROR:
            colorCode = "\033[0;31m";  // Rouge
            break;
        case DEBUG:
            colorCode = "\033[0;36m";  // Cyan
            break;
        default:
            colorCode = "\033[0;37m";  // Gris clair
            break;
    }

    return colorCode;
}

std::string pickStartMessageByLogSeverity(const Log messageSeverity) {
    std::string startMessage;

    switch (messageSeverity) {
        case INFO:
            startMessage = "[Info]: ";
            break;
        case WARNING:
            startMessage = "[Warning]: ";
            break;
        case ERROR:
            startMessage = "[Error]: ";
            break;
        case DEBUG:
            startMessage = "[Debug]: ";
            break;
        default:
            startMessage = "[Unknown]: ";
            break;
    }

    return startMessage;
}

std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw vkRuntimeError("failed to open file !");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

}  // namespace TEn
