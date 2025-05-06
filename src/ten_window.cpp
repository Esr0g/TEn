#include "ten_window.hpp"

#include <iostream>
#include <stdexcept>
#include <utils.hpp>

namespace TEn {

TEnWindow::TEnWindow(int w, int h, std::string name) : width(w), height(h), windowName(name) {
    initWindow();
}

TEnWindow::~TEnWindow() { glfwDestroyWindow(window); }

bool TEnWindow::shouldClose() { return glfwWindowShouldClose(window); }

void TEnWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    VkResult result = glfwCreateWindowSurface(instance, window, nullptr, surface);
    if (result != VK_SUCCESS) {
        throw vkRuntimeError("Failed to create window surface.", result);
    }
}

VkExtent2D TEnWindow::getExtent() {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
    return actualExtent;
}

void TEnWindow::waitGlfwResizeEvent() {
    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
    }
}

void TEnWindow::initWindow() {
    if (!glfwInit()) {
        throw vkRuntimeError("Failed to init GLFW !");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void TEnWindow::framebufferResizeCallback(GLFWwindow *window, int width, int height) {
    auto app = reinterpret_cast<TEnWindow *>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

}  // namespace TEn