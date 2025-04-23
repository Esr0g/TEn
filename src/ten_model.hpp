#pragma once

#include "ten_device.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>

namespace TEn {
class TEnModel {
   public:
    struct Vertex {
        glm::vec2 position;

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    TEnModel(TEnDevice &device, const std::vector<Vertex> &vertices);
    ~TEnModel();

    TEnModel(const TEnModel &) = delete;
    TEnModel operator=(const TEnModel &) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

    static std::vector<Vertex> getThreeFromOne(std::vector<Vertex> &triangle);
    static std::vector<Vertex> getTriangleAt(std::vector<Vertex> &vertices, int position);
    static std::vector<Vertex> sierpinskiTriangle(std::vector<Vertex> &vertices, int depth);

   private:
    void createVertexBuffers(const std::vector<Vertex> &vertices);

    TEnDevice &tenDevice;
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    uint32_t vertexCount;
};
}  // namespace TEn