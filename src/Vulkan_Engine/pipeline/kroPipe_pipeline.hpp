#ifndef KROPIPE_PIPELINE_HPP
#define KROPIPE_PIPELINE_HPP

#include "../../kroPipe_depedence.hpp"
// Incluir headers para objetos e variáveis usados (agora que definições estão em .cpp)
#include "../device/kroPipe_device.hpp" // OBJECT_device
#include "../debug/kroPipe_debug.hpp"   // check_vk_result, err, VK_Allocator, fatalMessage
#include "../depth/kroPipe_depth.hpp"   // OBJECT_depth (para findDepthFormat)
#include "../swapchain/kroPipe_swapchain.hpp" // swapChainImageFormat (já declarado extern aqui)
#include "kroPipe_vertex_data.hpp"      // VertexVulkan

#include <vector>   // std::vector
#include <string>   // std::string
#include <fstream>  // std::ifstream
#include <array>    // std::array
#include <stdexcept>// std::runtime_error

namespace KP {
namespace ENGINE {

// Variáveis globais/namespace (declaradas extern)
extern VkPipelineCache          g_PipelineCache; // Assinando como extern
extern std::vector<VkDescriptorSetLayout> setLayout; // Assinando como extern
extern VkPipelineLayout pipelineLayout; // Assinando como extern
extern VkPipeline graphicsPipeline; // Assinando como extern
extern std::string directoryFileManually; // Assinando como extern

// Definição ÚNICA do Render Pass (declarada extern)
// A definição real estará no .cpp
extern VkRenderPass VK_renderPass;


// Struct para módulos de shader (sem mudanças, apenas movida)
struct shaderModule {
    VkPipelineShaderStageCreateInfo shaderStages[2];
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};

class Pipeline{

public:
    // Membros relacionados à descrição dos vértices (mantidos como membros se forem específicos desta pipeline)
    // Se forem genéricos, poderiam ser obtidos de kroPipe_vertex_data.hpp diretamente.
    // Mantido como no original por enquanto.
    VkVertexInputBindingDescription bindingDescription = KP::ENGINE::VertexVulkan::getBindingDescription();
    std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = KP::ENGINE::VertexVulkan::getAttributeDescriptions();

    // Métodos auxiliares (agora estáticos membros da classe)
    static std::vector<char> readFile(const std::string& filename);
    static VkShaderModule createShaderModule(const std::vector<char>& code);
    static shaderModule UseShaders(const std::string& directoryPath, const std::string& vertShaderPath, const std::string& fragShaderPath);


    // Métodos da classe Pipeline
    void destroyGraphicsPipeline(VkShaderModule vertShaderModule, VkShaderModule fragShaderModule); // Ainda precisa dos handles dos módulos
    void createPipeline(shaderModule shader); // Cria a pipeline usando os módulos
    void createGraphicsPipeline(); // Método de alto nível para criar a pipeline completa
    void createRenderPass();
    void CleanUpPipeline();

}; //CLASS PIPELINE

// Objeto Pipeline (declarado extern)
extern KP::ENGINE::Pipeline OBJECT_pipeline;

} // namespace ENGINE
} // namespace KP

#endif //KROPIPE_PIPELINE_HPP