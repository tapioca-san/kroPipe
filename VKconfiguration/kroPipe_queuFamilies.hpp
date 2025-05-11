#ifndef QUEUFAMILIES_H
#define QUEUFAMILIES_H

#include "../VKkropipe/kroPipe_Log.hpp"
#include "../kroPipe_include.hpp"

namespace KP {
namespace QUEUFAMILIES {

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

class queuFamilies{
    
    public:
VkQueue presentQueue;



inline QueueFamilyIndices findQueuFamilies(VkPhysicalDevice device){
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
        err = vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        check_vk_result(err);
        
        if (presentSupport) {
            indices.presentFamily = i;
        }
        
        if (indices.isComplete())
        break;
    
    i++;
}

if(queueFamilyCount == 0){
    throw std::runtime_error(fatalMensage("failed to find a queue family!"));
}

graphicsIndex = indices.graphicsFamily.value();


return indices;
}
};//CLASS QUEUFAMILIES

inline KP::QUEUFAMILIES::queuFamilies OBJECT_queuFamilies;
}//QUEUFAMILIES
}//KP
#endif //QUEUFAMILIES_H