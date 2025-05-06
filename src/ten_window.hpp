#pragma once

#include <string>

#include "ten_core.hpp"

namespace TEn {
class TEnWindow {
   public:
    TEnWindow(int w, int h, std::string name);
    ~TEnWindow();

    TEnWindow(const TEnWindow &) = delete;
    TEnWindow &operator=(const TEnWindow &) = delete;

    bool shouldClose();
    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

    VkExtent2D getExtent();
    void waitGlfwResizeEvent();

    bool isFrameBufferResized() { return framebufferResized; }
    void setFrameBufferResized(const bool b) { framebufferResized = b; }

   private:
    void initWindow();
    static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

    int width;
    int height;

    std::string windowName;
    GLFWwindow *window = nullptr;

    bool framebufferResized = false;
};
}  // namespace TEn