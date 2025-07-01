#ifndef BUFFER_H
#define BUFFER_H

#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace ENGINE {

extern std::vector<VkDescriptorSetLayout> setLayout; // uso para o pipeline 

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

    
void createDescriptorSetLayout(KP::ENGINE::UniformBuffers &uniformBuffers);

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

struct UboStorage{

    public: ////////////////////////////////////////////////////////////////
        
        UniformBuffers uniformBuffers;
        UniformBufferObject UBO;

        
        uint32_t objectId;    

        size_t bufferSize = sizeof(UniformBufferObject);
        
        /*
        */
        UboStorage(uint32_t &objectID);

        void create();
        
        void createDescriptorLayout();

        void update();
        
        void cleanUp();

    private: ////////////////////////////////////////////////////////////////

        bool cleaned;

        void createUniformBuffers(KP::ENGINE::UniformBuffers &uniformBuffers);

        void updateUniformBuffer(KP::ENGINE::UniformBuffers &uniformBuffers, KP::ENGINE::UniformBufferObject &ubo, const uint32_t &currentImage);

        

        void createDescriptorSets(KP::ENGINE::UniformBuffers &uniformBuffers);

        void cleanupBuffer(KP::ENGINE::UniformBuffers &uniformBuffers);
};


extern KP::ENGINE::UboStorage OBJECT_sceneUBO;

} //BUFFER
} //KP
#endif//BUFFER_H