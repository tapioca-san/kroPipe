#ifndef DEBUG_NAME_H
#define DEBUG_NAME_H

#include "../kroPipe_include.hpp"

extern VkDevice device;

template <class T>
VkObjectType GetVkObjectType();

template <>
inline VkObjectType GetVkObjectType<VkBuffer>() { return VK_OBJECT_TYPE_BUFFER; }
template <>
inline VkObjectType GetVkObjectType<VkImage>() { return VK_OBJECT_TYPE_IMAGE; }
template <>
inline VkObjectType GetVkObjectType<VkShaderModule>() { return VK_OBJECT_TYPE_SHADER_MODULE; }
template <>
inline VkObjectType GetVkObjectType<VkPipeline>() { return VK_OBJECT_TYPE_PIPELINE; }

template <class T>
void AddDebugName(T handle, const char* name) {
    VkDebugUtilsObjectNameInfoEXT nameInfo = {};
    nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
    nameInfo.objectType = GetVkObjectType<T>();
    nameInfo.objectHandle = reinterpret_cast<uint64_t>(handle);
    nameInfo.pObjectName = name;
    vkSetDebugUtilsObjectNameEXT(device, &nameInfo);
}

/*
void setAllObjectNames() {
    AddDebugName<VkSurfaceKHR>(surface, "kP_surface");
    AddDebugName<VkDevice>(device, "LogicalDevice");
    AddDebugName<VkQueue>(graphicsQueue, "GraphicsQueue");
    AddDebugName<VkBuffer>(vertexBuffer, "VertexBuffer");
    AddDebugName<VkBuffer>(indexBuffer, "IndexBuffer");
    AddDebugName<VkImage>(textureImage, "TextureImage");
    AddDebugName<VkImageView>(textureImageView, "TextureImageView");
    AddDebugName<VkSampler>(textureSampler, "TextureSampler");
    AddDebugName<VkPipeline>(graphicsPipeline, "GraphicsPipeline");
    AddDebugName<VkRenderPass>(renderPass, "RenderPass");
}
*/

#endif // DEBUG_NAME_H
