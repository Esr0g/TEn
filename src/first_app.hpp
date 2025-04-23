#pragma once

#include <memory>
#include <vector>

#include "ten_device.hpp"
#include "ten_model.hpp"
#include "ten_pipeline.hpp"
#include "ten_swap_chain.hpp"
#include "ten_window.hpp"

namespace TEn {
class FirstApp {
   public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();
    ~FirstApp();

    FirstApp(const FirstApp &) = delete;
    FirstApp &operator=(const FirstApp &) = delete;

    void run();

   private:
    void loadModels();
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffer();
    void drawFrame();

    TEnWindow tenWindow;
    TEnDevice tenDevice;
    TEnSwapChain tenSwapChain;
    std::unique_ptr<TEnPipeline> tenPipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    std::unique_ptr<TEnModel> tenModel;
};
}  // namespace TEn