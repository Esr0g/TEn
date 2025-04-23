#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace TEn {

class TEnWindow {
   public:
    TEnWindow(int w, int h, std::string name);
    ~TEnWindow();

    TEnWindow(const TEnWindow &) = delete;
    TEnWindow &operator=(const TEnWindow &) = delete;

    bool shouldClose();
    VkExtent2D getExtent();

    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

   private:
    void initWindow();

    const int width;
    const int height;

    std::string windowName;
    GLFWwindow *window;
};
}  // namespace TEn