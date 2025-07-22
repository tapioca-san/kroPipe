#ifndef KROPIPE_PIPELINE_HPP
#define KROPIPE_PIPELINE_HPP

#include "../fileEditor/kroPipe_file.hpp"
#include "../buffers/kroPipe_buffer.hpp"
#include "../../kroPipe_depedence.hpp"
#include "kroPipe_vertex_data.hpp"  


namespace KP {
namespace ENGINE {

extern VkPipelineCache  PipelineCache; 
extern std::string      directoryProject; 
extern VkPipelineLayout pipelineLayout; 
extern VkPipeline       graphicsPipeline;

extern VkRenderPass VK_renderPass;

VkShaderModule createShaderModule(const std::vector<char>& code);

struct shaderModule {
    VkPipelineShaderStageCreateInfo shaderStages[2];
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};

class Pipeline{

    UniformBuffers uniformBuffaers;

public:
    VkVertexInputBindingDescription bindingDescription = KP::ENGINE::VertexVulkan::getBindingDescription();
    std::array<VkVertexInputAttributeDescription, 9> attributeDescriptions = KP::ENGINE::VertexVulkan::getAttributeDescriptions();

    static shaderModule UseShaders(const std::string& directoryPath, const std::string& vertShaderPath, const std::string& fragShaderPath);


    void destroyGraphicsPipeline(VkShaderModule vertShaderModule, VkShaderModule fragShaderModule);
    void createPipeline(shaderModule shader);
    void createGraphicsPipeline(); 
    void createRenderPass();
    void CleanUpPipeline();

}; 

extern KP::ENGINE::Pipeline OBJECT_pipeline;

} // namespace ENGINE
} // namespace KP

#endif //KROPIPE_PIPELINE_HPP