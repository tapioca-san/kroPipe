#ifndef BUFFER_H
#define BUFFER_H

#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace ENGINE {


    
extern const int MAX_FRAMES_IN_FLIGHT;

extern uint32_t currentFrame;

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

struct UniformBuffers{
    std::vector<VkBuffer>               uniformBuffers;
    std::vector<VkDeviceMemory>         uniformBuffersMemory;
    std::vector<void*>                  uniformBuffersMapped;  
    
    std::vector<VkDescriptorSet>        descriptorSets;
    VkDescriptorPool                    descriptorPool;
    VkDescriptorSetLayout               descriptorSetLayout;

};

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory); // enviar dados para a gpu atravez da CPU

    
void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

struct UboStorage{

    public: ////////////////////////////////////////////////////////////////
        
        UniformBuffers uniformBuffers;
        UniformBufferObject UBO;
        int objectId;    

        size_t bufferSize = sizeof(UniformBufferObject);
        
        /*
        */
        UboStorage();

        void create();
        
        void createDescriptorLayout();

        void update();
        
        void cleanUp();

    private: ////////////////////////////////////////////////////////////////


        void createDescriptorSetLayout(KP::ENGINE::UniformBuffers &uniformBuffers);

        void createUniformBuffers(KP::ENGINE::UniformBuffers &uniformBuffers);

        void updateUniformBuffer(KP::ENGINE::UniformBuffers &uniformBuffers, KP::ENGINE::UniformBufferObject &ubo, const uint32_t &currentImage);

        

        void createDescriptorSets(KP::ENGINE::UniformBuffers &uniformBuffers);

        void cleanupBuffer(KP::ENGINE::UniformBuffers &uniformBuffers);
};


} //BUFFER
} //KP
#endif//BUFFER_H