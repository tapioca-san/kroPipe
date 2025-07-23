#ifndef KROPIPE_BUFFER_H
#define KROPIPE_BUFFER_H

#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace ENGINE {

struct UniformBuffers {
    uint32_t lastID = 0;

    std::vector<VkBuffer>        uniformBuffers;
    std::vector<VkDeviceMemory>  uniformBuffersMemory;
    std::vector<void*>           uniformBuffersMapped;

    std::vector<VkDescriptorSet> descriptorSets;
    VkDescriptorPool             descriptorPool;
    VkDescriptorSetLayout        descriptorSetLayout;
};

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                  VkBuffer& buffer, VkDeviceMemory& bufferMemory);

class BufferManager {
private:

    void storeBuffer(std::shared_ptr<VkBuffer> buffer, std::shared_ptr<VkDeviceMemory> buffersMemory, void* uniformBuffersMapped);

    UniformBuffers buffers;

public:
    template<class t>
    uint32_t addBuffer(t classType);

    template<class t>
    void sendBufferDataToGpu(uint32_t bufferID, t data);

    void cleanUp();

    VkBuffer getUniformBufferByID(uint32_t bufferID);
    
    VkDeviceMemory getUniformBufferMemoryByID(uint32_t bufferID);
    

};

extern BufferManager OBJECT_bufferManager;

} // namespace ENGINE
} // namespace KP

#endif // KROPIPE_BUFFER_H
