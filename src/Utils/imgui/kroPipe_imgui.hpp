#ifndef IMGUI_H
#define IMGUI_H

#include "../../kroPipe_depedence.hpp"


inline VkClearColorValue clearColor = {{0.0f, 0.0f, 0.0f, 1.0f}};

namespace KP {
namespace UTILS {

class Imgui {
public:
    

    ImGuiIO *io = nullptr;

    Imgui(GLFWwindow* window, const VkInstance& inst, const VkPhysicalDevice& physDevice, const VkDevice& dev,
          const VkQueue& queue, const uint32_t& graphicsQueueIndex,
          const VkPipelineCache& pipelineCache, VkRenderPass& renderPass);

    void newFrame();

    void render(VkCommandBuffer &cmd);

    void drawWindows();

    void cleanup();

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

    static void check_vk_result(VkResult err);
    
    void useMouse();

    void setup();

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer cmd);
};

extern KP::UTILS::Imgui* OBJECT_imguiInterface;


} // namespace UTILS
} // namespace KP




#endif // IMGUI_H