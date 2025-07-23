
#ifndef KROPIPE_UBOHANDLER_H
#define KROPIPE_UBOHANDLER_H

#include "../../kroPipe_depedence.hpp"
#include "kroPipe_bufferManager.hpp"

namespace KP {
namespace ENGINE {


extern std::vector<VkDescriptorSetLayout> setLayout; // uso para o pipeline 

extern uint32_t currentFrame;

struct UniformBufferObject {
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};
    
void createDescriptorSetLayout(KP::ENGINE::UniformBuffers &uniformBuffers);

struct UboStorage{

    public: ////////////////////////////////////////////////////////////////
        
        UniformBuffers uniformBuffers;
        UniformBufferObject UBO;

        uint32_t objectId;    

        size_t bufferSize = sizeof(UniformBufferObject);

        UboStorage(uint32_t &objectID);

        void create();
        
        void createDescriptorLayout();

        void update();
        
        void cleanUp();

    private: ////////////////////////////////////////////////////////////////

        bool cleaned;

        void createUniformBuffers(KP::ENGINE::UniformBuffers &uniformBuffers);

        void updateUniformBuffer(KP::ENGINE::UniformBuffers &uniformBuffers, KP::ENGINE::UniformBufferObject &ubo, const uint32_t &currentImage);

        void cleanupBuffer(KP::ENGINE::UniformBuffers &uniformBuffers);
};


extern KP::ENGINE::UboStorage OBJECT_sceneUBO;


} //BUFFER
} //KP

#endif//KROPIPE_UBOHANDLER_H