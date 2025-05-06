#pragma once

#include <fstream>
#include <optional>
#include <vector>

#include "ten_device.hpp"
#include "ten_swap_chain.hpp"
#include "ten_window.hpp"

namespace TEn {

class FirstApp {
   public:
    static constexpr uint32_t WINDOW_WIDTH = 800;
    static constexpr uint32_t WINDOW_HEIGHT = 600;

    FirstApp();
    ~FirstApp();

    FirstApp(const FirstApp &) = delete;
    FirstApp &operator=(const FirstApp &) = delete;

    void run();

    void createGraphicsPipeline();
    VkShaderModule createShaderModule(const std::vector<char> &code);
    void drawFrame();

   private:
    TEnWindow tenWindow;
    TEnCore tenCore;
    TEnDevice tenDevice;
    TEnSwapChain tenSwapChain;

    VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
    VkPipeline graphicsPipeline = VK_NULL_HANDLE;
};
}  // namespace TEn