#ifndef KROPIPE_PIPELINE_HPP
#define KROPIPE_PIPELINE_HPP

#include "../../kroPipe_depedence.hpp"
// Incluir headers para objetos e variáveis usados (agora que definições estão em .cpp)
#include "../buffers/kroPipe_buffer.hpp"
#include "../device/kroPipe_device.hpp" // OBJECT_device
#include "../debug/kroPipe_debug.hpp"   // check_vk_result, err, VK_Allocator, fatalMessage
#include "../depth/kroPipe_depth.hpp"   // OBJECT_depth (para findDepthFormat)
#include "../swapchain/kroPipe_swapchain.hpp" // swapChainImageFormat (já declarado extern aqui)
#include "kroPipe_vertex_data.hpp"      // VertexVulkan


namespace KP {
namespace ENGINE {

extern VkPipelineCache                      PipelineCache; 
extern VkPipelineLayout                     pipelineLayout; 
extern VkPipeline                           graphicsPipeline;
extern std::string                          directoryProject; 

extern VkRenderPass VK_renderPass;


struct shaderModule {
    VkPipelineShaderStageCreateInfo shaderStages[2];
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};

class Pipeline{

    UniformBuffers uniformBuffaers;

public:
    VkVertexInputBindingDescription bindingDescription = KP::ENGINE::VertexVulkan::getBindingDescription();
    std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions = KP::ENGINE::VertexVulkan::getAttributeDescriptions();

    static std::vector<char> readFile(const std::string& filename);
    static VkShaderModule createShaderModule(const std::vector<char>& code);
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