#include "kroPipe_debug.hpp"

namespace KP {
namespace ENGINE {

// Definições das variáveis globais/namespace (sem extern)
bool debug = true;
VkResult err;
VkAllocationCallbacks* VK_Allocator = nullptr;
std::vector<std::string> logError; // Definição da variável extern

// Definições das funções de debug
void check_vk_result(VkResult err, std::string informationError){ // this is for checking if creating
    if (err == VK_SUCCESS)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if(informationError != "") // Corrigido para usar informationError
        std::cerr << informationError << "\n"; // Adicionado newline para melhor formatação
    if (err < 0)
        abort(); // Não é comum abortar para todos os erros, mas mantido do seu código
}

void defaultErrorNoText(const std::string& type, const std::string& message) {
    if (message.empty()) {
        // Lançar erro ou logar, dependendo da severidade
        std::cerr << "[ " << type << " ] : Attempted to log empty message!\n";
        // throw std::runtime_error("Failed to " + type + " a message without text"); // Removido throw para mensagens vazias
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

std::string fatalMessage(const std::string& message) {
    // defaultMessage("FATAL", message); // O original só retornava a string
    // Se quiser logar antes de lançar
    std::cerr << "[ FATAL ] : " << message << "\n";
    logError.push_back(message);
    return message; // Retorna a string para ser usada na exceção
}

void infoMessage(const std::string& message) {
    defaultMessage("INFO", message);
}

void debugMessage(const std::string& message) {
    if (debug) // Usando a variável debug do namespace
        defaultMessage("DEBUG", message);
}

void deviceMessage(const std::string& message) {
    if (debug) // Usando a variável debug do namespace
        defaultMessage("DEVICE", message);
}

void validationLayersMessage(const std::string& message) {
    if (debug) // Usando a variável debug do namespace
        defaultMessage("VALIDATION LAYER", message);
}

void modelMessage(const std::string& message) {
    if (debug) // Usando a variável debug do namespace
        defaultMessage("MODEL", message);
}

// glfw_error_callback - Esta função não estava declarada no header, mas estava definida aqui.
// Se ela é usada, declare-a no header ou onde for usada.
// Mantida aqui por enquanto, mas considere onde ela deve ir.
/*
void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
*/

// Definição da função callback do Debugger (estática)
VKAPI_ATTR VkBool32 VKAPI_CALL Debugger::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        warnMessage(pCallbackData->pMessage); // Mensagens do Vulkan que são urgentes
    } else {
        validationLayersMessage(pCallbackData->pMessage); // Chamando a função do namespace
    }
    return VK_FALSE;
}

// Definições dos métodos da classe Debugger
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
    createInfo.pfnUserCallback = debugCallback; // Usando a função callback estática
    createInfo.pUserData = nullptr;
}

VkResult Debugger::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger); // Usando pAllocator
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void Debugger::setupDebugMessenger(VkInstance& instance, VkDebugUtilsMessengerEXT& debugMessenger) {
    if (!debug) return; // Usando a variável debug do namespace
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);
    // Usando o alocador global VK_Allocator na chamada para CreateDebugUtilsMessengerEXT
    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, VK_Allocator, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error(fatalMessage("Failed to set up debug messenger!")); // Chamando fatalMessage do namespace
    }
}

void Debugger::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator); // Usando pAllocator
    }
}

// Definição do objeto Debugger (sem extern)
KP::ENGINE::Debugger OBJECT_debugger;

} // namespace ENGINE
} // namespace KP