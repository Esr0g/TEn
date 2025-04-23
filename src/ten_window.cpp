#include "ten_window.hpp"

#include <stdexcept>

namespace TEn {

TEnWindow::TEnWindow(int w, int h, std::string name) : width(w), height(h), windowName(name) {
    initWindow();
}

TEnWindow::~TEnWindow() {
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

bool TEnWindow::shouldClose() { return glfwWindowShouldClose(this->window); }

VkExtent2D TEnWindow::getExtent() {
    return {static_cast<uint32_t>(this->width), static_cast<uint32_t>(this->height)};
}

void TEnWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
    if (glfwCreateWindowSurface(instance, this->window, nullptr, surface) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create window surface");
    }
}

void TEnWindow::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    this->window =
        glfwCreateWindow(this->width, this->height, this->windowName.c_str(), nullptr, nullptr);
}
}  // namespace TEn