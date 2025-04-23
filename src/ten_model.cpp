#include "ten_model.hpp"

#include <string.h>

#include <iostream>

namespace TEn {
TEnModel::TEnModel(TEnDevice& device, const std::vector<Vertex>& vertices) : tenDevice(device) {
    createVertexBuffers(vertices);
}

TEnModel::~TEnModel() {
    vkDestroyBuffer(tenDevice.device(), vertexBuffer, nullptr);
    vkFreeMemory(tenDevice.device(), vertexBufferMemory, nullptr);
}

void TEnModel::draw(VkCommandBuffer commandBuffer) {
    vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}

void TEnModel::bind(VkCommandBuffer commandBuffer) {
    VkBuffer buffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void TEnModel::createVertexBuffers(const std::vector<Vertex>& vertices) {
    vertexCount = static_cast<uint32_t>(vertices.size());
    assert(vertexCount >= 3 && "Vertex count must be at least 3");
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
    tenDevice.createBuffer(
        bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer,
        vertexBufferMemory);

    void* data;
    vkMapMemory(tenDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(tenDevice.device(), vertexBufferMemory);
}

std::vector<TEnModel::Vertex> TEnModel::getThreeFromOne(std::vector<TEnModel::Vertex>& triangle) {
    auto A = triangle.at(0).position;
    auto B = triangle.at(1).position;
    auto C = triangle.at(2).position;

    glm::vec2 AB = {(A.x + B.x) / 2.0f, (A.y + B.y) / 2.0f};
    glm::vec2 BC = {(B.x + C.x) / 2.0f, (B.y + C.y) / 2.0f};
    glm::vec2 CA = {(C.x + A.x) / 2.0f, (C.y + A.y) / 2.0f};

    return std::vector<Vertex>{{A}, {AB}, {CA}, {AB}, {B}, {BC}, {CA}, {BC}, {C}};
}

std::vector<TEnModel::Vertex> TEnModel::getTriangleAt(std::vector<TEnModel::Vertex>& vertices,
                                                      int position) {
    if (position == 0) {
        return std::vector<TEnModel::Vertex>(vertices.begin(), vertices.begin() + 3);
    }

    if (position == 1) {
        return std::vector<TEnModel::Vertex>(vertices.begin() + 3, vertices.begin() + 6);
    }

    if (position == 2) {
        return std::vector<TEnModel::Vertex>(vertices.begin() + 6, vertices.end());
    }

    return std::vector<TEnModel::Vertex>{};
}

std::vector<TEnModel::Vertex> TEnModel::sierpinskiTriangle(std::vector<TEnModel::Vertex>& vertices,
                                                           int depth) {
    if (depth == 0) {
        return vertices;
    }

    if (depth == 1) {
        return TEnModel::getThreeFromOne(vertices);
    }

    auto temp = TEnModel::getThreeFromOne(vertices);
    auto triangleA = getTriangleAt(temp, 0);
    auto subTriangleA = sierpinskiTriangle(triangleA, depth - 1);
    auto triangleB = getTriangleAt(temp, 1);
    auto subTriangleB = sierpinskiTriangle(triangleB, depth - 1);
    auto triangleC = getTriangleAt(temp, 2);
    auto subTriangleC = sierpinskiTriangle(triangleC, depth - 1);

    std::vector<TEnModel::Vertex> merged{};

    merged.insert(merged.end(), subTriangleA.begin(), subTriangleA.end());
    merged.insert(merged.end(), subTriangleB.begin(), subTriangleB.end());
    merged.insert(merged.end(), subTriangleC.begin(), subTriangleC.end());

    return merged;
}

std::vector<VkVertexInputBindingDescription> TEnModel::Vertex::getBindingDescriptions() {
    std::vector<VkVertexInputBindingDescription> bindingDescription(1);
    bindingDescription[0].binding = 0;
    bindingDescription[0].stride = sizeof(Vertex);
    bindingDescription[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
}

std::vector<VkVertexInputAttributeDescription> TEnModel::Vertex::getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attributeDescription(1);

    attributeDescription[0].binding = 0;
    attributeDescription[0].location = 0;
    attributeDescription[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescription[0].offset = 0;

    return attributeDescription;
}
}  // namespace TEn