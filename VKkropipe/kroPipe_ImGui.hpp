#ifndef IMGUI_H
#define IMGUI_H

#include "../VKconfiguration/kroPipe_queuFamilies.hpp"
#include "../VKconfiguration/kroPipe_command.hpp"
#include "../VKconfiguration/kroPipe_window.hpp"
#include "../kroPipe_include.hpp"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>

namespace KP {
namespace IMGUI {

class Imgui {
public:
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
    }

    void render(VkCommandBuffer cmd) {
        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
    }

    void drawDemoWindows() {
        static bool show_demo_window = true;
        static bool show_another_window = false;
        static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::Checkbox("Demo Window", &show_demo_window);
        ImGui::Checkbox("Another Window", &show_another_window);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);
        ImGui::End();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);
        if (show_another_window) {
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            ImGui::End();
        }
    }

    void cleanup() {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        vkDestroyDescriptorPool(device, imguiPool, nullptr);
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


    static void check_vk_result(VkResult err) {
        if (err == VK_SUCCESS) return;
        fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
        if (err < 0) abort();
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
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = instance;
        init_info.PhysicalDevice =  physicalDevice;
        init_info.Device = device;
        init_info.Queue = presentQueue;
        init_info.DescriptorPool = imguiPool;
        init_info.RenderPass = renderPass;
        init_info.MinImageCount = 2;
        init_info.ImageCount = 3;
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
        init_info.CheckVkResultFn = check_vk_result;
        ImGui_ImplVulkan_Init(&init_info);
    }

    // Esses métodos devem estar no seu sistema
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer cmd);
};

} // namespace IMGUI
} // namespace KP


KP::IMGUI::Imgui* imguiInterface = nullptr;

#endif // IMGUI_H
