#ifndef KROPIPE_DEBUG_HPP
#define KROPIPE_DEBUG_HPP

#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace ENGINE {

// Variáveis globais/namespace (declaradas extern)
extern bool debug;
extern VkResult err;
extern VkAllocationCallbacks* VK_Allocator; // Assinando como extern
extern std::vector<std::string> logError; // Assinando como extern

// Funções de debug (declaradas)
void check_vk_result(VkResult err, std::string informationError = "");
void defaultErrorNoText(const std::string& type, const std::string& message);
void defaultMessage(const std::string& type, const std::string& message);

void warnMessage(const std::string& message);
void errorMessage(const std::string& message);
void infoMessage(const std::string& message);
void debugMessage(const std::string& message);
#define fatalMessage(msg) ( "\n///////////////////////////////////////////////////////////////////////\n [ DEBUGGER FOUND ERROR ]\nMessage: " + std::string(msg) + " \nFile: " + __FILE__ + ":" + std::to_string(__LINE__) + " \nFunction Error: " + __func__ )

void deviceMessage(const std::string& message);
void validationLayersMessage(const std::string& message);
void modelMessage(const std::string& message);

// Classe Debugger
class Debugger {
public:
    // A função de callback do Vulkan deve ser static. ela é a responsavel para mandar mensagem para nós
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData);

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
    void setupDebugMessenger(VkInstance& instance, VkDebugUtilsMessengerEXT& debugMessenger);
    void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
};

extern KP::ENGINE::Debugger OBJECT_debugger;

} // namespace ENGINE
} // namespace KP

#endif//KROPIPE_DEBUG_HPP