#ifndef QUEUFAMILIES_H
#define QUEUFAMILIES_H

#include "../VKkropipe/kroPipe_Log.hpp"
#include "../kroPipe_include.hpp"
#include "kroPipe_windowSurface.hpp"
#include "kroPipe_extension.hpp"


VkQueue presentQueue;



QueueFamilyIndices findQueuFamilies(VkPhysicalDevice device){
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
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

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
    return indices;
}


#endif //QUEUFAMILIES_H