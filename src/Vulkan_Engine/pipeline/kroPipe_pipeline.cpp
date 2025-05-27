#include "kroPipe_pipeline.hpp"
// Incluídos no .hpp agora que definições estão separadas
// #include "../device/kroPipe_device.hpp"
// #include "../../kroPipe_depedence.hpp"
// #include "../debug/kroPipe_debug.hpp"
// #include "../depth/kroPipe_depth.hpp"
// #include "kroPipe_vertex_data.hpp"



namespace KP {
namespace ENGINE {

// Definições das variáveis globais/namespace (sem extern)
VkPipelineCache                                 PipelineCache = VK_NULL_HANDLE; // Definição
std::vector<VkDescriptorSetLayout>              setLayout; // Definição (inicializado vazio)
VkPipelineLayout                                pipelineLayout = VK_NULL_HANDLE; // Definição
VkPipeline                                      graphicsPipeline = VK_NULL_HANDLE; // Definição
std::string                                     directoryProject = "/home/pipebomb/dev/cpp/vulkan/teste/src"; // Definição
std::string                                     directoryShader = "/Vulkan_Engine/shader/";


// Definição ÚNICA do Render Pass (sem extern)
VkRenderPass VK_renderPass = VK_NULL_HANDLE; // Definição

// Definição do objeto Pipeline (sem extern)
KP::ENGINE::Pipeline OBJECT_pipeline;

// Definição dos métodos estáticos da classe Pipeline

std::vector<char> Pipeline::readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        // Usando fatalMessage do namespace
        throw std::runtime_error(KP::ENGINE::fatalMessage("failed to open file! (maybe shaders path is incorrect): " + filename)); // Adicionado nome do arquivo
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

VkShaderModule Pipeline::createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    // O código está em bytes, mas a API Vulkan espera um ponteiro para uint32_t
    // O reinterpret_cast é seguro aqui porque garantimos que o tamanho é múltiplo de 4
    // (compilador de shader SPV garante isso).
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    // Usando OBJECT_device, VK_Allocator, err, check_vk_result do namespace
    KP::ENGINE::err = vkCreateShaderModule(KP::ENGINE::OBJECT_device.VK_Device, &createInfo, KP::ENGINE::VK_Allocator, &shaderModule);
    KP::ENGINE::check_vk_result(KP::ENGINE::err, "Failed to create shader module!");

    return shaderModule;
}

shaderModule Pipeline::UseShaders(const std::string& directoryPath, const std::string& vertShaderPath, const std::string& fragShaderPath) {
    KP::ENGINE::shaderModule outShaderModule;

    // Usando readFile e createShaderModule como métodos estáticos da classe Pipeline
    auto vertShaderCode = Pipeline::readFile(directoryPath + vertShaderPath);
    auto fragShaderCode = Pipeline::readFile(directoryPath + fragShaderPath);

    outShaderModule.vertShaderModule = Pipeline::createShaderModule(vertShaderCode);
    outShaderModule.fragShaderModule = Pipeline::createShaderModule(fragShaderCode);

    outShaderModule.shaderStages[0] = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, VK_SHADER_STAGE_VERTEX_BIT, outShaderModule.vertShaderModule, "main", nullptr}; // Adicionado pSpecializationInfo = nullptr
    outShaderModule.shaderStages[1] = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, VK_SHADER_STAGE_FRAGMENT_BIT, outShaderModule.fragShaderModule, "main", nullptr}; // Adicionado pSpecializationInfo = nullptr

    return outShaderModule;
}


// Definição dos métodos da classe Pipeline

void Pipeline::destroyGraphicsPipeline(VkShaderModule vertShaderModule, VkShaderModule fragShaderModule){
    // Usando OBJECT_device, VK_Allocator do namespace
    // Estes handles de shaderModule devem ser destruídos após a criação da pipeline,
    // não necessariamente neste método de limpeza da pipeline inteira.
    vkDestroyShaderModule(KP::ENGINE::OBJECT_device.VK_Device, fragShaderModule, KP::ENGINE::VK_Allocator);
    vkDestroyShaderModule(KP::ENGINE::OBJECT_device.VK_Device, vertShaderModule, KP::ENGINE::VK_Allocator);
}

void Pipeline::createPipeline(shaderModule shader){
    // Usando membros da classe e objetos/funções do namespace KP::ENGINE

    // A descrição dos vértices pode ser obtida diretamente da struct se quiser
    // auto bindingDescription = KP::ENGINE::VertexVulkan::getBindingDescription();
    // auto attributeDescriptions = KP::ENGINE::VertexVulkan::getAttributeDescriptions();
    // Usando membros da classe Pipeline que já obtiveram essas descrições
    auto bindingDescription = this->bindingDescription;
    auto attributeDescriptions = this->attributeDescriptions;


    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();


    // Cache da Pipeline
    VkPipelineCacheCreateInfo cacheCreateInfo{};
    cacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    // Se você tiver dados de cache salvos, carregue-os aqui:
    // cacheCreateInfo.initialDataSize = size;
    // cacheCreateInfo.pInitialData = data;

    // Usando OBJECT_device, VK_Allocator, err, check_vk_result do namespace
    KP::ENGINE::err = vkCreatePipelineCache(KP::ENGINE::OBJECT_device.VK_Device, &cacheCreateInfo, KP::ENGINE::VK_Allocator, &PipelineCache); // Cria o membro g_PipelineCache
    KP::ENGINE::check_vk_result(KP::ENGINE::err, "failed to create pipeline cache!");


    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;
    // Viewports e scissors serão definidos dinamicamente


    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE; // Requer feature
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL; // ou VK_POLYGON_MODE_LINE para wireframe
    rasterizer.lineWidth = 1.0f; // Requer feature wideLines para > 1.0f
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE; // ou VK_FRONT_FACE_CLOCKWISE dependendo do seu modelo/config. Original era COUNTER_CLOCKWISE
    rasterizer.depthBiasEnable = VK_FALSE;


    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE; // Requer feature sampleRateShading
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; // Usar 1 para não-multisampling


    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE; // Habilita teste de profundidade
    depthStencil.depthWriteEnable = VK_TRUE; // Permite escrever no buffer de profundidade
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS; // Comparação para o teste de profundidade
    depthStencil.depthBoundsTestEnable = VK_FALSE; // Requer feature depthBounds
    depthStencil.stencilTestEnable = VK_FALSE; // Stencil desabilitado


    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE; // Desabilita blending

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE; // Desabilita logic op
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Blend constants não usados com blendEnable = FALSE
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;


    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();


    // Layout da Pipeline
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(setLayout.size()); // Usando setLayout do namespace
    pipelineLayoutInfo.pSetLayouts = setLayout.data();
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Se não usar push constants
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    // Usando OBJECT_device, VK_Allocator, err, check_vk_result do namespace
    KP::ENGINE::err = vkCreatePipelineLayout(KP::ENGINE::OBJECT_device.VK_Device, &pipelineLayoutInfo, KP::ENGINE::VK_Allocator, &pipelineLayout); // Cria o membro pipelineLayout
    KP::ENGINE::check_vk_result(KP::ENGINE::err, "failed to create pipeline layout!");

    // Info principal da Pipeline Gráfica
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shader.shaderStages; // Usando a struct shader passada

    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;

    pipelineInfo.layout = pipelineLayout; // Usando o membro pipelineLayout
    pipelineInfo.renderPass = VK_renderPass; // Usando VK_renderPass do namespace
    pipelineInfo.subpass = 0; // Índice do subpass dentro do render pass

    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Opcional para derivar pipelines
    pipelineInfo.basePipelineIndex = -1; // Opcional para derivar pipelines

    // Usando OBJECT_device, g_PipelineCache, VK_Allocator, err, check_vk_result do namespace
    KP::ENGINE::err = vkCreateGraphicsPipelines(KP::ENGINE::OBJECT_device.VK_Device, PipelineCache, 1, &pipelineInfo, KP::ENGINE::VK_Allocator, &graphicsPipeline); // Cria o membro graphicsPipeline
    KP::ENGINE::check_vk_result(KP::ENGINE::err, "failed to create graphics pipeline!");

    // Módulos de shader podem ser destruídos após a criação da pipeline
    vkDestroyShaderModule(KP::ENGINE::OBJECT_device.VK_Device, shader.fragShaderModule, KP::ENGINE::VK_Allocator);
    vkDestroyShaderModule(KP::ENGINE::OBJECT_device.VK_Device, shader.vertShaderModule, KP::ENGINE::VK_Allocator);
}

void Pipeline::createGraphicsPipeline() {
    // Usando directoryFileManually e métodos estáticos UseShaders, createPipeline
    KP::ENGINE::shaderModule shader = Pipeline::UseShaders(KP::ENGINE::directoryProject, directoryShader + "vert.spv", directoryShader + "frag.spv");

    createPipeline(shader); // Chamando método da classe

    // Os módulos de shader são destruídos dentro de createPipeline agora
    // vkDestroyShaderModule(KP::ENGINE::OBJECT_device.VK_Device, shader.fragShaderModule, KP::ENGINE::VK_Allocator);
    // vkDestroyShaderModule(KP::ENGINE::OBJECT_device.VK_Device, shader.vertShaderModule, KP::ENGINE::VK_Allocator);
}

void Pipeline::createRenderPass() {
    // Usando swapChainImageFormat do namespace, OBJECT_depth, VK_renderPass, OBJECT_device, VK_Allocator, err, check_vk_result do namespace

    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = KP::ENGINE::swapChainImageFormat; // Usando variável do namespace
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // Limpa no início do render pass
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // Armazena o resultado para apresentação
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Layout inicial
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // Layout final para apresentação

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = KP::ENGINE::OBJECT_depth.findDepthFormat(); // Usando OBJECT_depth do namespace
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // Limpa no início do render pass
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Layout inicial
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // Layout final para uso

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0; // Índice 0 na lista de attachments
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Layout durante o subpass

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1; // Índice 1 na lista de attachments
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL; // Layout durante o subpass

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef; // Associa o attachment de profundidade

    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL; // Dependência externa (antes do render pass)
    dependency.dstSubpass = 0; // Dependência para o nosso subpass (índice 0)
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT; // Estágios onde a escrita/leitura externa acontece
    dependency.srcAccessMask = 0; // Nenhuma operação de acesso anterior precisa ser concluída
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT; // Estágios onde nosso subpass vai escrever/ler
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT; // Operações de acesso que devem esperar

    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment}; // Array de todos os attachments
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    // Usando OBJECT_device, VK_Allocator, err, check_vk_result do namespace
    KP::ENGINE::err = vkCreateRenderPass(KP::ENGINE::OBJECT_device.VK_Device, &renderPassInfo, KP::ENGINE::VK_Allocator, &VK_renderPass); // Cria o membro VK_renderPass (que é extern)
    KP::ENGINE::check_vk_result(KP::ENGINE::err, "failed to create render pass!");
}

void Pipeline::CleanUpPipeline(){
    // Usando membros da classe e OBJECT_device, VK_Allocator do namespace
    vkDestroyPipeline(KP::ENGINE::OBJECT_device.VK_Device, graphicsPipeline, KP::ENGINE::VK_Allocator); // Destroi o membro graphicsPipeline
    vkDestroyPipelineCache(KP::ENGINE::OBJECT_device.VK_Device, PipelineCache, KP::ENGINE::VK_Allocator); // Destroi o membro g_PipelineCache
    vkDestroyPipelineLayout(KP::ENGINE::OBJECT_device.VK_Device, pipelineLayout, KP::ENGINE::VK_Allocator); // Destroi o membro pipelineLayout
    vkDestroyRenderPass(KP::ENGINE::OBJECT_device.VK_Device, VK_renderPass, KP::ENGINE::VK_Allocator); // Destroi VK_renderPass (que é extern)
}

} // namespace ENGINE
} // namespace KP