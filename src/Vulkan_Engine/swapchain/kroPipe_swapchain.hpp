#ifndef KROPIPE_SWAPCHAIN_HPP
#define KROPIPE_SWAPCHAIN_HPP

#include "../../kroPipe_depedence.hpp"
#include "../window/kroPipe_window.hpp" // Necessário para OBJECT_window, mWindow
#include "../device/kroPipe_device.hpp" // Necessário para OBJECT_device
#include "../queue/kroPipe_queuFamilies.hpp" // Necessário para QueueFamilyIndices
#include "../debug/kroPipe_debug.hpp" // Necessário para check_vk_result, err

#include "../buffers/kroPipe_frameBuffer.hpp" // Necessário para std::vector<VkFramebuffer>
#include "../depth/kroPipe_depth.hpp"       // Necessário para depthImageView, depthImage, depthImageMemory
#include "../image/kroPipe_imageView.hpp"   // Necessário para swapChainImageViews, swapChainImages



namespace KP {
namespace ENGINE {

// Variáveis globais/namespace relacionadas ao Swap Chain (declaradas extern)
extern std::vector<VkImage> swapChainImages; 
extern std::vector<VkImageView> swapChainImageViews; 

extern VkFormat swapChainImageFormat;
extern VkExtent2D swapChainExtent;
extern VkSwapchainKHR swapChain; // Assinando como extern
extern std::vector<VkFramebuffer> swapChainFramebuffers; // Assinando como extern
extern bool vsync; // Assinando como extern

// Struct para detalhes de suporte do Swap Chain
struct SwapChainSupportDetails{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

// Classe SwapChain
class SwapChain{

public:
    // Handles do Swap Chain (agora temos apenas um, 'swapChain', declarado extern)
    // uint32_t formatCount; // Não é mais necessário como membro
    // uint32_t presentModeCount; // Não é mais necessário como membro
    // bool swapChainAdequate = false; // Não é mais necessário como membro

    // Método para obter detalhes de suporte do Swap Chain
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    // Funções auxiliares (agora estáticas membros da classe)
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    // Métodos de criação e limpeza do Swap Chain
    void createSwapChain();
    void cleanupSwapChain();
    void recreateSwapChain();

}; //CLASS SWAPCHAIN

// Objeto SwapChain (declarado extern)
extern KP::ENGINE::SwapChain OBJECT_swapChain;

} // namespace SWAPCHAIN
} // namespace KP

#endif //KROPIPE_SWAPCHAIN_HPP