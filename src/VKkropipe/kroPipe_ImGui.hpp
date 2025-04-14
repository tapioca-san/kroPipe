#ifndef IMGUI_H
#define IMGUI_H

#include "../VKconfiguration/kroPipe_queuFamilies.hpp"
#include "../VKconfiguration/kroPipe_command.hpp"
#include "../VKconfiguration/kroPipe_window.hpp"
#include "../kroPipe_include.hpp"

namespace KP {
namespace IMGUI {


class Imgui{
    
public:

    Imgui(const VkInstance &inst, const VkPhysicalDevice &physDevice, const VkDevice &dev, const VkQueue &queue, const uint32_t &graphicsQueueIndex, const VkPipelineCache &pipelineCache, VkRenderPass &renderPass)
        : instance(inst), physicalDevice(physDevice), device(dev), presentQueue(queue), graphicsIndex(graphicsQueueIndex), pipelineCache(pipelineCache), renderPass(renderPass) { 
            KP::COMMANDBUFFER::createDescriptorPool(descriptorPool);
    }

    ~Imgui(){
        cleanup_imgui();
    }

    void setup() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        
        ImGui_ImplGlfw_InitForVulkan(mWindow, true);
        
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = instance;
        init_info.PhysicalDevice = physicalDevice;
        init_info.Device = device;
        init_info.QueueFamily = graphicsIndex;
        init_info.Queue = presentQueue;
        init_info.PipelineCache = pipelineCache;
        init_info.DescriptorPool = descriptorPool;
        init_info.Subpass = 0;
        init_info.MinImageCount = 2;
        init_info.ImageCount = 2;
        init_info.RenderPass = renderPass;
        ImGui_ImplVulkan_Init(&init_info);
    }


private:

    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue presentQueue;
    uint32_t graphicsIndex;
    VkPipelineCache pipelineCache;
    VkDescriptorPool descriptorPool;
    VkRenderPass renderPass;



    void cleanup_imgui() {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }



};


}//IMGUI
}//KP


#endif//IMGUI_H