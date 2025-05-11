#ifndef DEVICE_H 
#define DEVICE_H

#include "../VKkropipe/kroPipe_Log.hpp"
#include "../kroPipe_include.hpp"
#include "kroPipe_queuFamilies.hpp"
#include "kroPipe_swapchain.hpp"

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

namespace KP {
namespace DEVICE {

class Device{
    
    
public:
// Graphic cards informations
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    VkPhysicalDeviceFeatures supportedFeatures;

//

// VARIABLE
    float queuePriority = 0.0f; // this is for increase the priority of our fetch. when reached 1.0, our fetch got the high priority.

//
inline bool checkDeviceExtensionSupport(VkPhysicalDevice &device) {
    uint32_t extensionCount;

    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}


inline bool isDeviceSuitable(VkPhysicalDevice device) {
    KP::QUEUFAMILIES::QueueFamilyIndices indices = KP::QUEUFAMILIES::OBJECT_queuFamilies.findQueuFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        KP::SWAPCHAIN::SwapChainSupportDetails swapChainSupport = KP::SWAPCHAIN::OBJECT_swapChain.querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    
    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}



inline void pickPhysicalDevice(VkInstance& instance) {
    // Select Physical Device (GPU)
   
        // Select graphics queue family
        //g_QueueFamily = ImGui_ImplVulkanH_SelectQueueFamilyIndex(g_PhysicalDevice);
        //IM_ASSERT(g_QueueFamily != (uint32_t)-1);
    
    
        uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    
    if (deviceCount == 0) {
        throw std::runtime_error(fatalMensage("failed to find GPUs with Vulkan support!"));
    }
    
    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            g_PhysicalDevice = device;
            break;
        }
    }
    
    if (g_PhysicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error(fatalMensage("failed to find a suitable GPU!"));
    }
    
    
}


inline void createLogicalDevice() {
    KP::QUEUFAMILIES::QueueFamilyIndices indices = KP::QUEUFAMILIES::OBJECT_queuFamilies.findQueuFamilies(g_PhysicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    
    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;
  
    // swapchain activation
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();
    //
    

    if (debug) {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(g_PhysicalDevice, &createInfo, Allocator, &g_Device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(g_Device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(g_Device, indices.presentFamily.value(), 0, &KP::QUEUFAMILIES::OBJECT_queuFamilies.presentQueue);
}

}; //CLASS DEVICE

inline KP::DEVICE::Device OBJECT_device;
}//DEVICE
}//KP

#endif //DEVICE_H