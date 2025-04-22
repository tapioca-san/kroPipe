#ifndef IMGUI_H
#define IMGUI_H

#include "../VKconfiguration/kroPipe_window.hpp"
#include "../kroPipe_include.hpp"
#include "kroPipe_object.hpp"

namespace KP {
namespace IMGUI {

class Imgui {
public:
    
    ImGuiIO *io = nullptr;
    
    Imgui(GLFWwindow* window,const VkInstance& inst, const VkPhysicalDevice& physDevice, const VkDevice& dev,
          const VkQueue& queue, const uint32_t& graphicsQueueIndex,
          const VkPipelineCache& pipelineCache, VkRenderPass& renderPass)
        : window(window), instance(inst), physicalDevice(physDevice), device(dev),
          presentQueue(queue), graphicsIndex(graphicsQueueIndex),
          pipelineCache(pipelineCache), renderPass(renderPass) {
        setup();
    }

    void newFrame() {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        useMouse();
    }

    void render(VkCommandBuffer cmd) {
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
    }

    void drawDemoWindows() {
        char* buf;
        ImGui::Begin("Hello, world!");
        if (ImGui::CollapsingHeader("Velocidade Avançada")){
            ImGui::DragFloat("float X", &allObjects[sortedID[1]]->data.Position.x, 0.1f);
            ImGui::DragFloat("float Y", &allObjects[sortedID[1]]->data.Position.y, 0.1f);
            ImGui::DragFloat("float Z", &allObjects[sortedID[1]]->data.Position.z, 0.1f);
        }
        
    }

    void cleanup() {
        ImGui::PopStyleColor();
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        vkDestroyDescriptorPool(device, imguiPool, Allocator);
    }

private:
    GLFWwindow* window;
    VkInstance instance;
    VkPhysicalDevice physicalDevice;
    VkDevice device;
    VkQueue presentQueue;
    uint32_t graphicsIndex;
    VkPipelineCache pipelineCache;
    VkRenderPass renderPass;
    VkDescriptorPool imguiPool;

    double xpos, ypos;

    static void check_vk_result(VkResult err) {
        if (err == VK_SUCCESS) return;
        fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
        if (err < 0) abort();
    }

    void useMouse(){
        glfwGetCursorPos(mWindow, &xpos, &ypos);
        io->AddMousePosEvent((float)xpos, (float)ypos);
    }

    void setup() {
        // Descriptor Pool
        VkDescriptorPoolSize pool_sizes[] = {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 100 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 100 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 100 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100 }
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000;
        pool_info.poolSizeCount = (uint32_t)(sizeof(pool_sizes) / sizeof(pool_sizes[0]));
        pool_info.pPoolSizes = pool_sizes;
        check_vk_result(vkCreateDescriptorPool(device, &pool_info, nullptr, &imguiPool));

        // ImGui Init
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = &ImGui::GetIO();
        io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        ImGui::StyleColorsDark();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // Cor vermelha

        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = instance;
        init_info.PhysicalDevice =  physicalDevice;
        init_info.Device = device;
        init_info.Queue = presentQueue;
        init_info.DescriptorPool = imguiPool;
        init_info.RenderPass = renderPass;
        init_info.PipelineCache = pipelineCache;
        init_info.MinImageCount = 2;
        init_info.ImageCount = 3;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.CheckVkResultFn = check_vk_result;
        ImGui_ImplVulkan_Init(&init_info);
    }

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer cmd);
};

} // namespace IMGUI
} // namespace KP


KP::IMGUI::Imgui* imguiInterface = nullptr;

#endif // IMGUI_H
