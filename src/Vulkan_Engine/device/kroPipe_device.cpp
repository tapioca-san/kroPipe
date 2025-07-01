#include "../queue/kroPipe_queuFamilies.hpp" 
#include "../swapchain/kroPipe_swapchain.hpp"
#include "../pipeline/kroPipe_GPGPU.hpp"
#include "../debug/kroPipe_debug.hpp" 
#include "kroPipe_device.hpp"


namespace KP {
namespace ENGINE {

std::vector<const char*> extensionAvailable;

VkPhysicalDeviceProperties Device::getDeviceProperties(){
    return deviceProperties;
}
VkPhysicalDeviceFeatures Device::getPhysicalDeviceFeatures(){
    return deviceFeatures;
}
VkPhysicalDeviceFeatures Device::getSupportedFeatures(){
    return supportedFeatures;
}
VkPhysicalDevice Device::getPhysicalDevice(){
    return VK_PhysicalDevice;
}
VkDevice Device::getDevice(){
    return VK_Device;
}


bool Device::checkDeviceExtensionSupport(VkPhysicalDevice &device) {
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


bool Device::isDeviceSuitable(VkPhysicalDevice device) {
    KP::ENGINE::QueueFamilyIndices indices = KP::ENGINE::OBJECT_queuFamilies.findQueuFamilies(device);

    bool extensionsSupported = checkDeviceExtensionSupport(device);

    vkGetPhysicalDeviceProperties(device, &deviceProperties); 
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);   

    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        KP::ENGINE::SwapChainSupportDetails swapChainSupport = KP::ENGINE::OBJECT_swapChain.querySwapChainSupport(device);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void Device::pickPhysicalDevice(VkInstance& instance) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) {
        throw std::runtime_error(fatalMessage("failed to find GPUs with Vulkan support!"));
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) { 
            VK_PhysicalDevice = device; 
            break;
        }
    }

    if (VK_PhysicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error(fatalMessage("failed to find a suitable GPU!")); 
    }
}

void Device::createLogicalDevice() {
    KP::ENGINE::QueueFamilyIndices indices = KP::ENGINE::OBJECT_queuFamilies.findQueuFamilies(VK_PhysicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsAndComputeFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeaturesToEnable{};
    deviceFeaturesToEnable.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeaturesToEnable; 
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (KP::ENGINE::debug) { 
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    } else {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(VK_PhysicalDevice, &createInfo, KP::ENGINE::VK_Allocator, &KP::ENGINE::OBJECT_device.VK_Device) != VK_SUCCESS) {
        throw std::runtime_error(fatalMessage("failed to create logical device!")); 
    }

    // queue gráfica guardada na queue families
    vkGetDeviceQueue(KP::ENGINE::OBJECT_device.getDevice(), indices.graphicsAndComputeFamily.value(), 0, OBJECT_GPGPU.getComputeQueue());
    vkGetDeviceQueue(KP::ENGINE::OBJECT_device.getDevice(), indices.graphicsAndComputeFamily.value(), 0, &KP::ENGINE::OBJECT_queuFamilies.graphicsQueue);
    vkGetDeviceQueue(KP::ENGINE::OBJECT_device.getDevice(), indices.presentFamily.value(), 0, &KP::ENGINE::OBJECT_queuFamilies.presentQueue);
}

KP::ENGINE::Device OBJECT_device;

} // namespace ENGINE
} // namespace KP