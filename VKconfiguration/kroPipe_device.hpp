#ifndef DEVICE_H 
#define DEVICE_H

#include "../VKkropipe/kroPipe_Log.hpp"
#include "../kroPipe_include.hpp"
#include "kroPipe_queuFamilies.hpp"
#include "kroPipe_extension.hpp"
#include "kroPipe_swapchain.hpp"

namespace KP {
namespace DEVICE {


// Graphic cards informations
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    VkPhysicalDeviceFeatures supportedFeatures;

//

// VARIABLE
    float queuePriority = 0.0f; // this is for increase the priority of our fetch. when reached 1.0, our fetch got the high priority.

//
bool checkDeviceExtensionSupport(VkPhysicalDevice &device) {
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


bool isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices indices = findQueuFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport = KP::SWAPCHAIN::querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    
    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}



void pickPhysicalDevice(VkInstance& instance) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    
    if (deviceCount == 0) {
        throw std::runtime_error(fatalMensage("failed to find GPUs with Vulkan support!"));
    }
    
    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            break;
        }
    }
    
    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error(fatalMensage("failed to find a suitable GPU!"));
    }
    if (debug) {
        deviceMensage(deviceProperties.deviceName);
    }

}


void createLogicalDevice() {
    QueueFamilyIndices indices = findQueuFamilies(physicalDevice);

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

    if (vkCreateDevice(physicalDevice, &createInfo, Allocator, &device) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
}

}//DEVICE
}//KP

#endif //DEVICE_H
