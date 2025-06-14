#include "../window/kroPipe_windowSurface.hpp" // OBJECT_windowsurface
#include "../buffers/kroPipe_frameBuffer.hpp" // OBJECT_frameBuffer
#include "../queue/kroPipe_queuFamilies.hpp" // OBJECT_queuFamilies, QueueFamilyIndices
#include "../image/kroPipe_imageView.hpp"   // OBJECT_imageView, swapChainImageViews, swapChainImages
#include "../window/kroPipe_window.hpp" // OBJECT_window, mWindow
#include "../render/kroPipe_render.hpp"
#include "../device/kroPipe_device.hpp" // OBJECT_device
#include "../debug/kroPipe_debug.hpp" // check_vk_result, err, VK_Allocator
#include "../depth/kroPipe_depth.hpp"       // OBJECT_depth, depthImageView, depthImage, depthImageMemory
#include "kroPipe_swapchain.hpp"

namespace KP {
namespace ENGINE {

// Definições das variáveis globais/namespace (sem extern)
std::vector<VkImage> swapChainImages; 
std::vector<VkImageView> swapChainImageViews; 
VkFormat swapChainImageFormat;
VkExtent2D swapChainExtent;
VkSwapchainKHR swapChain = VK_NULL_HANDLE; // Definição
std::vector<VkFramebuffer> swapChainFramebuffers; // Definição
bool vsync = true; // Definição (valor inicial de exemplo)

// Definição do objeto SwapChain (sem extern)
KP::ENGINE::SwapChain OBJECT_swapChain;

// Definição do método querySwapChainSupport da classe SwapChain
SwapChainSupportDetails SwapChain::querySwapChainSupport(VkPhysicalDevice device) {
    SwapChainSupportDetails details;

    // Usando OBJECT_window do namespace
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, KP::ENGINE::OBJECT_windowSurface.VK_surface, &details.capabilities);

    uint32_t formatCount;
    // Usando OBJECT_window do namespace
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, KP::ENGINE::OBJECT_windowSurface.VK_surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        // Usando OBJECT_window do namespace
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, KP::ENGINE::OBJECT_windowSurface.VK_surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    // Usando OBJECT_window do namespace
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, KP::ENGINE::OBJECT_windowSurface.VK_surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        // Usando OBJECT_window do namespace
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, KP::ENGINE::OBJECT_windowSurface.VK_surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

// Definição das funções auxiliares estáticas
VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width, height;
        // Usando mWindow do namespace
        glfwGetFramebufferSize(KP::ENGINE::OBJECT_window.GLFW_window, &width, &height);
        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };
        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return actualExtent;
    }
}

// Definição dos métodos de criação e limpeza do Swap Chain
void SwapChain::createSwapChain() {
    // Usando membros da classe e objetos/funções do namespace KP::ENGINE
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(KP::ENGINE::OBJECT_device.getPhysicalDevice());

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats); // Chamando static member
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes); // Chamando static member
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities); // Chamando static member

    uint32_t imageCount = MAX_FRAMES_IN_FLIGHT;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = KP::ENGINE::OBJECT_windowSurface.VK_surface; // Usando objeto do namespace

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // Usando objeto/funções do namespace
    KP::ENGINE::QueueFamilyIndices indices = KP::ENGINE::OBJECT_queuFamilies.findQueuFamilies(KP::ENGINE::OBJECT_device.getPhysicalDevice());
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    // Usando a variável vsync do namespace
    if(KP::ENGINE::vsync){
        createInfo.presentMode = presentMode;
    }
    else{
        createInfo.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR; // Modo sem VSync
    }
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE; // Não recriando a partir de um antigo neste método

    // Usando err, VK_Allocator, check_vk_result do namespace
    KP::ENGINE::err = vkCreateSwapchainKHR(KP::ENGINE::OBJECT_device.getDevice(), &createInfo, KP::ENGINE::VK_Allocator, &swapChain); // Cria o membro swapChain
    KP::ENGINE::check_vk_result(KP::ENGINE::err, "failed to create swap chain!");

    // Obter imagens do swap chain (usando swapChainImages do namespace)
    vkGetSwapchainImagesKHR(KP::ENGINE::OBJECT_device.getDevice(), swapChain, &imageCount, nullptr);
    KP::ENGINE::swapChainImages.resize(imageCount);
    KP::ENGINE::err = vkGetSwapchainImagesKHR(KP::ENGINE::OBJECT_device.getDevice(), swapChain, &imageCount, KP::ENGINE::swapChainImages.data());
    KP::ENGINE::check_vk_result(KP::ENGINE::err, "Failed to get swap chain images"); // Adicionado mensagem de erro

    // Atualizar variáveis de formato e extent (usando swapChainImageFormat, swapChainExtent do namespace)
    KP::ENGINE::swapChainImageFormat = surfaceFormat.format;
    KP::ENGINE::swapChainExtent = extent;

    // Nota: A parte que redimensiona swapChainImages novamente abaixo parece duplicada. Removi a duplicação.
    /*
    std::vector<VkImage> swapChainImages; // Isto criaria um novo vector local
    vkGetSwapchainImagesKHR(KP::ENGINE::OBJECT_device.VK_Device, VK_swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(KP::ENGINE::OBJECT_device.VK_Device, VK_swapChain, &imageCount, swapChainImages.data());
    */
}

void SwapChain::cleanupSwapChain(){
    // Usando objetos/variáveis/alocador do namespace KP::ENGINE
    vkDestroyImageView(KP::ENGINE::OBJECT_device.getDevice(), KP::ENGINE::depthImageView, KP::ENGINE::VK_Allocator); // Usando depthImageView do namespace
    vkDestroyImage(KP::ENGINE::OBJECT_device.getDevice(), KP::ENGINE::depthImage, KP::ENGINE::VK_Allocator);       // Usando depthImage do namespace
    vkFreeMemory(KP::ENGINE::OBJECT_device.getDevice(), KP::ENGINE::depthImageMemory, KP::ENGINE::VK_Allocator); // Usando depthImageMemory do namespace

    // Usando swapChainFramebuffers do namespace
    for (auto framebuffer : KP::ENGINE::swapChainFramebuffers) {
        vkDestroyFramebuffer(KP::ENGINE::OBJECT_device.getDevice(), framebuffer, KP::ENGINE::VK_Allocator);
    }

    // Usando swapChainImageViews do namespace
    for (auto imageView : KP::ENGINE::swapChainImageViews) {
        vkDestroyImageView(KP::ENGINE::OBJECT_device.getDevice(), imageView, KP::ENGINE::VK_Allocator);
    }

    // Destroi o membro swapChain
    vkDestroySwapchainKHR(KP::ENGINE::OBJECT_device.getDevice(), swapChain, KP::ENGINE::VK_Allocator);
}

void SwapChain::recreateSwapChain(){
    // Usando mWindow do namespace
    int width = 0, height = 0;
    glfwGetFramebufferSize(KP::ENGINE::OBJECT_window.GLFW_window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(KP::ENGINE::OBJECT_window.GLFW_window, &width, &height);
        glfwWaitEvents();
    }

    // Usando OBJECT_device do namespace
    vkDeviceWaitIdle(KP::ENGINE::OBJECT_device.getDevice());

    cleanupSwapChain(); // Chamando método da classe

    createSwapChain(); // Chamando método da classe
    // Usando OBJECT_imageView, OBJECT_depth, OBJECT_frameBuffer do namespace e chamando seus métodos
    KP::ENGINE::OBJECT_imageView.createImageViews();
    KP::ENGINE::OBJECT_depth.createDepthResources();
    KP::ENGINE::OBJECT_frameBuffer.createFrameBuffers();
}

} // namespace ENGINE
} // namespace KP