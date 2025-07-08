#include "kroPipe_imgui.hpp"

#include "../../Vulkan_Engine/pipeline/kroPipe_pipeline.hpp"
#include "../../Vulkan_Engine/instance/kroPipe_instance.hpp"
#include "../../Vulkan_Engine/window/kroPipe_window.hpp"
#include "../../Vulkan_Engine/debug/kroPipe_debug.hpp"
#include "../terminal/kroPipe_terminal.hpp"
#include "../object/kroPipe_object.hpp"

namespace KP {
namespace UTILS {


    Imgui::Imgui(GLFWwindow* window,const VkInstance& inst, const VkPhysicalDevice& physDevice, const VkDevice& dev,
          const VkQueue& queue, const uint32_t& graphicsQueueIndex,
          const VkPipelineCache& pipelineCache, VkRenderPass& renderPass)
        : window(window), instance(inst), physicalDevice(physDevice), device(dev),
          presentQueue(queue), graphicsIndex(graphicsQueueIndex),
          pipelineCache(pipelineCache), renderPass(renderPass) {
        setup();
    }

    void Imgui::newFrame() { // Init to draw GUI on the app. you have to use this before try to render on main loop
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        useMouse();
    }

    void Imgui::render(VkCommandBuffer &cmd){
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
    }

    void Imgui::drawWindows(){ // Use to draw in the while loop and see on the app
        
        KP::UTILS::OBJECT_terminal.showTerminal();
        ImGui::Begin("Objects Configuration Window");
        for(uint32_t i = 0; i < KP::UTILS::OBJECT_objectsManager.getAllObject()->size(); i++){
            ImGui::PushID(i);
            std::string headerName = "Object " + std::to_string(i);
            KP::UTILS::OBJECT_objectsManager.getObjectByID(i)->DrawTransformUI(headerName);
            ImGui::PopID();
        }
        ImGui::End(); 
        KP::UTILS::OBJECT_objectsManager.logID();
        /*
        
        ImGui::Begin("Debug Window");
        if(ImGui::CollapsingHeader("object")){
            ImGui::Checkbox("FreeView", &flyMode);
            ImGui::Checkbox("Show FPS", &framePerSecond);
            ImGui::DragFloat3("Clean Color", clearColor.float32, 0.01, 0.0, 1.0);
            
        }
        ImGui::End(); 
        */
        
    }

    void Imgui::cleanup() { // Use to cleanup everything about Dear IMGUI
        ImGui::PopStyleColor();
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        vkDestroyDescriptorPool(device, imguiPool, KP::ENGINE::VK_Allocator);
    }

    void Imgui::useMouse(){ // Use the mouse to can control the GUI throung mouse.
        glfwGetCursorPos(KP::ENGINE::OBJECT_window.GLFW_window, &xpos, &ypos);
        io->AddMousePosEvent((float)xpos, (float)ypos);
    }

    void Imgui::setup() {
        
        { // Descriptor Pool for Dear IMGUI use

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
            vkCreateDescriptorPool(device, &pool_info, nullptr, &imguiPool); 
        }

        
        {   // ImGui Init
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            io = &ImGui::GetIO();
            io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        }

        {   // CONFIGURATION
            //ImFont* terminalFont = io->Fonts->AddFontFromFileTTF("fonts/Terminus.ttf", 18.0f);
            //if (!terminalFont) {
            //    std::cerr << "[ERROR] Falha ao carregar fonte do terminal." << std::endl;
            //} else {
            //    io->FontDefault = terminalFont;
            //}
            ImGui::StyleColorsDark();
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
            ImGui::GetStyle().ScaleAllSizes(3.0f);
            io->FontGlobalScale = 2.5f; 
        }
        
        {   // INIT FOR DEAR IMGUI WITH VULKAN VARIABLES
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
            //init_info.CheckVkResultFn = check_vk_result;
            ImGui_ImplVulkan_Init(&init_info);
        }
    }

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer cmd);
    
    KP::UTILS::Imgui* OBJECT_imguiInterface = nullptr;
    


} // namespace UTILS
} // namespace KP

