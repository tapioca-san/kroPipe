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
        setup();
    }
        
    ~Imgui(){
        cleanup_imgui();
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


    static void check_vk_result(VkResult err){
        if (err == VK_SUCCESS)
            return;
        fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
        if (err < 0)
            abort();
    }


    void setup() {
        KP::COMMANDBUFFER::createDescriptorPool(descriptorPool);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        ImGui::StyleColorsDark();
        
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
        init_info.MinImageCount = currentFrame;
        init_info.ImageCount = 2;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.RenderPass = renderPass;
        init_info.Allocator = Allocator;
        init_info.CheckVkResultFn = check_vk_result;
        ImGui_ImplVulkan_Init(&init_info);
    }

    void cleanup_imgui() {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }



};


}//IMGUI
}//KP


#endif//IMGUI_H