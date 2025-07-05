#ifndef KROPIPE_TEXTURE_H
#define KROPIPE_TEXTURE_H

#include "../device/kroPipe_device.hpp"
#include "../vertex/kroPipe_vertex.hpp"
#include "../../kroPipe_depedence.hpp"
#include "../buffers/kroPipe_buffer.hpp"

namespace KP {
namespace ENGINE {


extern VkDeviceMemory textureImageMemory;
extern VkImageView textureImageView;
extern VkSampler textureSampler;
extern VkImage textureImage;


void createImage(uint32_t width, uint32_t height, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

void createTextureImage();

VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

void createTextureImageView();

void createTextureSampler();

}//TEXTURE
}//KP


#endif//KROPIPE_TEXTURE_H