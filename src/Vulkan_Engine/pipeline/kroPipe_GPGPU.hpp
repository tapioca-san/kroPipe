#ifndef KROPIPE_PIPELINE_HPP
#define KROPIPE_PIPELINE_HPP

#include "../../kroPipe_depedence.hpp"
#include "kroPipe_pipeline.hpp"


namespace KP{
namespace ENGINE {

extern std::string  directoryProject; 
extern std::string  directoryShader;


VkShaderModule createShaderModule(const std::vector<char>& code);

class GPGPU{

    public:

    
    void createCompute();

    VkQueue* getComputeQueue();

    private:

    void createComputeDescriptorSetLayout();

    void createComputePipeline(const std::string& directoryPath, const std::string& computeShader);

    VkDescriptorSetLayout computeDescriptorSetLayout;
    VkPipelineLayout computePipelineLayout;
    VkPipeline computePipeline;
    VkQueue computeQueue;
};

extern KP::ENGINE::GPGPU OBJECT_GPGPU;

} //ENGINE
} //KP

#endif //PIPELINE_HPP