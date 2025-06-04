#include "kroPipe_queuFamilies.hpp"
#include "../debug/kroPipe_debug.hpp" 
#include "../window/kroPipe_window.hpp"
#include "../window/kroPipe_windowSurface.hpp" 

namespace KP {
namespace ENGINE {

bool QueueFamilyIndices::isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
}

KP::ENGINE::QueuFamilies OBJECT_queuFamilies;

QueueFamilyIndices QueuFamilies::findQueuFamilies(VkPhysicalDevice device){
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    
    
    int i = 0;
    
    for (const auto& queueFamily : queueFamilies){
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
            indices.graphicsFamily = i;
        }
        
        VkBool32 presentSupport = false;
        err = vkGetPhysicalDeviceSurfaceSupportKHR(device, i, KP::ENGINE::OBJECT_windowSurface.VK_surface, &presentSupport);
        check_vk_result(err);
        
        if (presentSupport) {
            indices.presentFamily = i;
        }
        
        if (indices.isComplete())
        break;
    
    i++;
    }

    if(queueFamilyCount == 0){
    throw std::runtime_error(fatalMessage("failed to find a queue family!"));
}

graphicsIndex = indices.graphicsFamily.value();


return indices;
}

} // namespace ENGINE
} // namespace KP