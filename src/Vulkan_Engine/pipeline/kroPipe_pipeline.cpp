#include "../swapchain/kroPipe_swapchain.hpp" 
#include "../device/kroPipe_device.hpp" 
#include "../device/kroPipe_device.hpp" 
#include "../texture/kroPipe_MSAA.hpp"
#include "../depth/kroPipe_depth.hpp" 
#include "../debug/kroPipe_debug.hpp"   
#include <vulkan/vulkan_core.h>
#include "kroPipe_pipeline.hpp"


namespace KP {
namespace ENGINE {

VkPipelineCache  PipelineCache = VK_NULL_HANDLE;
VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
std::string      directoryProject = "/home/pipebomb/dev/cpp/vulkan/teste/src"; 
std::string      directoryShader = "/Vulkan_Engine/shader/";
VkPipeline       graphicsPipeline = VK_NULL_HANDLE;


VkRenderPass VK_renderPass = VK_NULL_HANDLE; 

KP::ENGINE::Pipeline OBJECT_pipeline;


VkShaderModule createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    KP::ENGINE::err = vkCreateShaderModule(*KP::ENGINE::OBJECT_device.getPointerDevice(), &createInfo, KP::ENGINE::VK_Allocator, &shaderModule);
    KP::ENGINE::check_vk_result(KP::ENGINE::err, "Failed to create shader module!");

    return shaderModule;
}

shaderModule Pipeline::UseShaders(const std::string& directoryPath, const std::string& vertShaderPath, const std::string& fragShaderPath) {
    KP::ENGINE::shaderModule outShaderModule;

    auto vertShaderCode = OBJECT_fileEditor.readFile<std::vector<char>>(directoryPath + vertShaderPath);
    auto fragShaderCode = OBJECT_fileEditor.readFile<std::vector<char>>(directoryPath + fragShaderPath);

    outShaderModule.vertShaderModule = createShaderModule(vertShaderCode);
    outShaderModule.fragShaderModule = createShaderModule(fragShaderCode);

    outShaderModule.shaderStages[0] = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, VK_SHADER_STAGE_VERTEX_BIT, outShaderModule.vertShaderModule, "main"};
    outShaderModule.shaderStages[1] = {VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO, nullptr, 0, VK_SHADER_STAGE_FRAGMENT_BIT, outShaderModule.fragShaderModule, "main"};

    return outShaderModule;
}

void Pipeline::destroyGraphicsPipeline(VkShaderModule vertShaderModule, VkShaderModule fragShaderModule){
    vkDestroyShaderModule(*KP::ENGINE::OBJECT_device.getPointerDevice(), fragShaderModule, KP::ENGINE::VK_Allocator);
    vkDestroyShaderModule(*KP::ENGINE::OBJECT_device.getPointerDevice(), vertShaderModule, KP::ENGINE::VK_Allocator);
}

void Pipeline::createPipeline(shaderModule shader){
    auto bindingDescription = KP::ENGINE::VertexVulkan::getBindingDescription();
    auto attributeDescriptions = KP::ENGINE::VertexVulkan::getAttributeDescriptions();
    
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineCacheCreateInfo cacheCreateInfo{};
    cacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    cacheCreateInfo.initialDataSize = 0;
    cacheCreateInfo.pInitialData = nullptr;
    
    KP::ENGINE::err = vkCreatePipelineCache(*KP::ENGINE::OBJECT_device.getPointerDevice(), &cacheCreateInfo, KP::ENGINE::VK_Allocator, &PipelineCache);
    KP::ENGINE::check_vk_result(err, "failed to create pipeline cache!");

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;
    
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
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

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;

    if(setLayout.data() == nullptr){
        throw std::runtime_error(fatalMessage("Erro setLayout has no values"));
    }
    pipelineLayoutInfo.pSetLayouts = setLayout.data();

    KP::ENGINE::err = vkCreatePipelineLayout(*KP::ENGINE::OBJECT_device.getPointerDevice(), &pipelineLayoutInfo, KP::ENGINE::VK_Allocator, &pipelineLayout);
    KP::ENGINE::check_vk_result(err, "failed to create pipeline layout!");

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shader.shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = VK_renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    
    KP::ENGINE::err = vkCreateGraphicsPipelines(*KP::ENGINE::OBJECT_device.getPointerDevice(), PipelineCache, 1, &pipelineInfo, KP::ENGINE::VK_Allocator, &graphicsPipeline); // Cria o membro graphicsPipeline
    KP::ENGINE::check_vk_result(KP::ENGINE::err, "failed to create graphics pipeline!");

}

void Pipeline::createGraphicsPipeline() {
   KP::ENGINE::shaderModule shader = Pipeline::UseShaders(KP::ENGINE::directoryProject, directoryShader + "vert.spv", directoryShader + "frag.spv");

    createPipeline(shader);

    vkDestroyShaderModule(*KP::ENGINE::OBJECT_device.getPointerDevice(), shader.fragShaderModule, KP::ENGINE::VK_Allocator);
    vkDestroyShaderModule(*KP::ENGINE::OBJECT_device.getPointerDevice(), shader.vertShaderModule, KP::ENGINE::VK_Allocator);
    
}
void Pipeline::createRenderPass() {
    if(*KP::ENGINE::OBJECT_msaa.getAbleMsaa() == true && *KP::ENGINE::OBJECT_msaa.getPointerMsaaSamples() == VK_SAMPLE_COUNT_1_BIT){
        throw std::runtime_error(fatalMessage("Msaa is Enable but your GPU can't support it"));
    }
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = swapChainImageFormat;
    if(*KP::ENGINE::OBJECT_msaa.getAbleMsaa() == true){
        colorAttachment.samples = *KP::ENGINE::OBJECT_msaa.getPointerMsaaSamples();
    }
    else{
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    }
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; 
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    if(*KP::ENGINE::OBJECT_msaa.getAbleMsaa() == true){
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }
    else {
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    }
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = KP::ENGINE::OBJECT_depth.findDepthFormat();
    if(*KP::ENGINE::OBJECT_msaa.getAbleMsaa() == true){
        depthAttachment.samples = *KP::ENGINE::OBJECT_msaa.getPointerMsaaSamples();
    }
    else {
        depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    }
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    
    VkAttachmentDescription colorAttachmentResolve{};
    colorAttachmentResolve.format = swapChainImageFormat;
    colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentResolveRef{};
    colorAttachmentResolveRef.attachment = 2;
    colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
    if(*KP::ENGINE::OBJECT_msaa.getAbleMsaa() == true){
        subpass.pResolveAttachments = &colorAttachmentResolveRef;
    }
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    std::vector<VkAttachmentDescription> attachments; 
    if(*KP::ENGINE::OBJECT_msaa.getAbleMsaa() == true){
        attachments = {colorAttachment, depthAttachment, colorAttachmentResolve};
    }
    else {
        attachments = {colorAttachment, depthAttachment };
    }
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;
    if (vkCreateRenderPass(*KP::ENGINE::OBJECT_device.getPointerDevice(), &renderPassInfo, nullptr, &KP::ENGINE::VK_renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
}

void Pipeline::CleanUpPipeline(){
    vkDestroyPipeline(*KP::ENGINE::OBJECT_device.getPointerDevice(), graphicsPipeline, KP::ENGINE::VK_Allocator); 
    vkDestroyPipelineCache(*KP::ENGINE::OBJECT_device.getPointerDevice(), PipelineCache, KP::ENGINE::VK_Allocator); 
    vkDestroyPipelineLayout(*KP::ENGINE::OBJECT_device.getPointerDevice(), pipelineLayout, KP::ENGINE::VK_Allocator); 
    vkDestroyRenderPass(*KP::ENGINE::OBJECT_device.getPointerDevice(), VK_renderPass, KP::ENGINE::VK_Allocator); 
}

} // namespace ENGINE
} // namespace KP