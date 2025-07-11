#ifndef KROPIPE_RENDER_HPP
#define KROPIPE_RENDER_HPP

#include "../command/kroPipe_command.hpp"


namespace KP {
namespace ENGINE {
        
extern int MAX_FRAMES_IN_FLIGHT;
extern std::vector<VkSemaphore> imageAvailableSemaphores;
extern std::vector<VkSemaphore> renderFinishedSemaphores;
extern std::vector<VkFence> inFlightFences;
extern uint32_t imageIndex;
extern bool framebufferResized;

class Render{

private:

public:

Render();

void destroyRender();
void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

VkFenceCreateInfo FenceCreateInfo(VkFenceCreateFlags flags);

VkSemaphoreCreateInfo SemaphoreCreateInfo(VkSemaphoreCreateFlags flags);

VkResult acquireNextImage();

void drawFrame();

void createSyncObjects();

};//CLASS RENDER

extern KP::ENGINE::Render OBJECT_render;

}//ENGINE
}//KP
#endif//KROPIPE_RENDER_HPP