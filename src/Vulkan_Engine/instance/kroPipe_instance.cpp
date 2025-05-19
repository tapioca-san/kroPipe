#include "kroPipe_instance.hpp"
#include "../window/kroPipe_window.hpp" // Necessário para mWindow (usado em getRequiredExtensions)

// Não há definições para OBJECT_debugger ou VK_instance aqui se elas são definidas
// em kroPipe_debug.cpp e em algum ponto principal (main.cpp ou app.cpp).
// Ah, o original DEFINIA OBJECT_debugger e VK_instance aqui. Vamos manter.

namespace KP {
namespace ENGINE {

// Definições das variáveis globais/namespace (sem extern)
VkInstance VK_instance = VK_NULL_HANDLE; // Definição

// Definições dos métodos da classe Instance

// Definição do callback de redimensionamento (estático)
void Instance::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    // Placeholder para lógica futura de redimensionamento do framebuffer
    // Se você associou o objeto Window via glfwSetWindowUserPointer,
    // pode obtê-lo aqui:
    // auto app = reinterpret_cast<Aplication*>(glfwGetWindowUserPointer(window));
    // app->framebufferResized = true; // Exemplo
}

// Nota: A função Debugger::debugCallback foi movida para kroPipe_debug.cpp


void Instance::createInstance() {
    // Usando debug, validationLayers (membro da classe), OBJECT_debugger, fatalMessage do namespace
    if (KP::ENGINE::debug && !checkValidationLayerSupport()) { // Usando debug do namespace e método da classe
        throw std::runtime_error(KP::ENGINE::fatalMessage("Debug requested, but validation layers not available!"));
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Anork";
    appInfo.pEngineName = "KROPIPE";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0; // Considere VK_API_VERSION_1_1, 1_2 ou 1_3 se precisar de features mais recentes

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions(); // Chamando método da classe
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (KP::ENGINE::debug) { // Usando debug do namespace
        // Usando validationLayers (membro da classe)
        createInfo.enabledLayerCount = static_cast<uint32_t>(this->validationLayers.size());
        createInfo.ppEnabledLayerNames = this->validationLayers.data();

        // Usando OBJECT_debugger do namespace e seu método
        KP::ENGINE::OBJECT_debugger.populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo; // pNext precisa ser void* ou type*
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    // Usando VK_instance (que é extern, definido neste arquivo .cpp)
    if (vkCreateInstance(&createInfo, nullptr, &VK_instance) != VK_SUCCESS) {
        throw std::runtime_error(KP::ENGINE::fatalMessage("Failed to create instance!")); // Chamando fatalMessage do namespace
    }
}

bool Instance::checkValidationLayerSupport() {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    // Usando validationLayers (membro da classe)
    for (const char* layerName : this->validationLayers) {
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            return false;
        }
    }

    return true;
}

std::vector<const char*> Instance::getRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (KP::ENGINE::debug) { // Usando debug do namespace
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

} // namespace ENGINE
} // namespace KP