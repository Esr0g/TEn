#pragma once
#include <vulkan/vulkan.h>

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace TEn {

enum Log {
    INFO = 0,
    WARNING = 1,
    ERROR = 2,
    DEBUG = 3,
};

std::string vkResultToString(const VkResult result);
std::runtime_error vkRuntimeError(const std::string &message,
                                  const std::optional<VkResult> result = std::nullopt);
void log(const std::string &message, const Log messageSeverity = INFO);
std::vector<char> readFile(const std::string &filename);
static std::string pickColorByLogSeverity(const Log messageSeverity);
static std::string pickStartMessageByLogSeverity(const Log messageSeverity);
}  // namespace TEn