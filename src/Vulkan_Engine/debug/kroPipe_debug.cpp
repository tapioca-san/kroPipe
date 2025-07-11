#include "kroPipe_debug.hpp"
#include <stdexcept>
#include <vulkan/vulkan_core.h>

namespace KP {
namespace ENGINE {
bool debug = true;
VkResult err;
VkAllocationCallbacks* VK_Allocator = nullptr;
std::vector<std::string> logError; 

void check_vk_result(VkResult err, std::string informationError){
    if (err == VK_SUCCESS)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if(informationError != "") 
        std::cerr << informationError << "\n"; 
    if (err < 0)
        abort(); 
}

void defaultErrorNoText(const std::string& type, const std::string& message) {
    if (message.empty()) {
        std::cerr << "[ " << type << " ] : Attempted to log empty message!\n";
    }
}

void defaultMessage(const std::string& type, const std::string& message) {
    defaultErrorNoText(type, message);
    if (!message.empty()) {
        logError.push_back(message);
        std::cerr << "[ " << type << " ] : " << message << "\n";
    }
}

void warnMessage(const std::string& message) {
    defaultMessage("WARN", message);
}

void errorMessage(const std::string& message) {
    defaultMessage("ERROR", message);
}

void infoMessage(const std::string& message) {
    defaultMessage("INFO", message);
}

void debugMessage(const std::string& message) {
    if (debug)
        defaultMessage("DEBUG", message);
}

void deviceMessage(const std::string& message) {
    if (debug)
        defaultMessage("DEVICE", message);
}

void validationLayersMessage(const std::string& message) {
    if (debug)
        defaultMessage("VALIDATION LAYER", message);
}

void modelMessage(const std::string& message) {
    if (debug)
        defaultMessage("MODEL", message);
}

void Debugger::setDebugName(VkDevice device, uint64_t objectHandle, VkObjectType objectType, const std::string& name) {
    if(KP::ENGINE::debug){
        VkDebugUtilsObjectNameInfoEXT nameInfo{};
        nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        nameInfo.objectType = objectType;
        nameInfo.objectHandle = objectHandle;
        nameInfo.pObjectName = name.c_str();

        auto func = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetDeviceProcAddr(device, "vkSetDebugUtilsObjectNameEXT");
        if (func != nullptr) {
            func(device, &nameInfo);
        }
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL Debugger::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        warnMessage(pCallbackData->pMessage); 
    } else {
        validationLayersMessage(pCallbackData->pMessage);
    }
    return VK_FALSE;
}

void Debugger::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;
}

VkResult Debugger::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void Debugger::setupDebugMessenger(VkInstance& instance, VkDebugUtilsMessengerEXT& debugMessenger) {
    if(KP::ENGINE::debug){
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);
        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, VK_Allocator, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error(fatalMessage("Failed to set up debug messenger!")); 
        }
    }
}

void Debugger::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator); 
    }
}

KP::ENGINE::Debugger OBJECT_debugger;

} // namespace ENGINE
} // namespace KP