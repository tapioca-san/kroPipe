#include "kroPipe_Aplication.hpp"
#include "instance/kroPipe_instance.hpp"
#include "window/kroPipe_window.hpp"


// Definição única do objeto da Aplicação (sem extern)
Aplication OBJECT_aplication;

// Definição do método de inicialização da classe Aplication
void Aplication::init(){
    // Chamando métodos e usando objetos/variáveis do namespace KP::ENGINE
    // Instância da classe Instance - pode ser membro da classe Aplication se quiser

    KP::ENGINE::OBJECT_instance.createInstance(); // Chama o método na instância local

    // Usando OBJECT_debugger e VK_instance do namespace, e debugMessenger (membro da classe Aplication)
    KP::ENGINE::OBJECT_debugger.setupDebugMessenger(KP::ENGINE::VK_instance, debugMessenger);

    KP::ENGINE::OBJECT_windowSurface.createSurface(KP::ENGINE::VK_instance, KP::ENGINE::GLFW_window, KP::ENGINE::OBJECT_windowSurface.VK_surface);
    
    // Usando OBJECT_device e VK_instance do namespace
    KP::ENGINE::OBJECT_device.pickPhysicalDevice(KP::ENGINE::VK_instance);

    // Usando OBJECT_device do namespace
    KP::ENGINE::OBJECT_device.createLogicalDevice();

    // Criar Surface depois de pickPhysicalDevice (que pode precisar dela para isDeviceSuitable)
    // E antes de createSwapChain (que *definitivamente* precisa dela)
    // A criação da surface geralmente fica associada à janela.
    // Onde a surface é criada no seu código original?
    // Precisa criar a surface usando glfwCreateWindowSurface após ter a instância Vulkan e a janela GLFW.
    // Adicione a criação da surface aqui ou em uma etapa separada de inicialização.

    /*
    // Exemplo de criação da surface (assumindo mWindow existe e VK_instance foi criada)
    // Usando OBJECT_window, mWindow, VK_instance do namespace, e err, check_vk_result do namespace
    KP::ENGINE::err = glfwCreateWindowSurface(KP::ENGINE::VK_instance, KP::ENGINE::mWindow, KP::ENGINE::VK_Allocator, &KP::ENGINE::OBJECT_window.VK_surface);
    KP::ENGINE::check_vk_result(KP::ENGINE::err, "Failed to create window surface!");
    */

    // Depois de criar logical device e surface, você pode criar o swapchain e outros recursos
    /*
    KP::ENGINE::OBJECT_swapChain.createSwapChain();
    KP::ENGINE::OBJECT_imageView.createImageViews();
    KP::ENGINE::OBJECT_renderPass.createRenderPass(); // Se RenderPass for um objeto separado ou membro
    // KP::ENGINE::OBJECT_pipeline.createRenderPass(); // RenderPass é membro de Pipeline no seu código
    KP::ENGINE::OBJECT_pipeline.createRenderPass(); // Chamando método da classe Pipeline
    KP::ENGINE::OBJECT_depth.createDepthResources();
    KP::ENGINE::OBJECT_frameBuffer.createFrameBuffers();
    KP::ENGINE::OBJECT_pipeline.createGraphicsPipeline();
    // Criar buffers de comando, etc.
    */


    std::cerr << "Funcionou asd" << std::endl; // Mensagem de debug/sucesso
}

// Adicione outros métodos da classe Aplication (run, cleanup, etc.) aqui

/*
void Aplication::cleanup() {
    // Chame os métodos de limpeza na ordem inversa da criação
    KP::ENGINE::OBJECT_pipeline.CleanUpPipeline(); // Limpa pipeline, cache, layout, render pass
    KP::ENGINE::OBJECT_frameBuffer.CleanUpFramerBuffer(); // Limpa framebuffers (já destrói image views e recursos de profundidade na limpeza do swapchain)
    KP::ENGINE::OBJECT_swapChain.cleanupSwapChain(); // Limpa swapchain, image views do swapchain, e recursos de profundidade (conforme seu cleanupSwapChain)

    // Limpar recursos do debug messenger
    if (KP::ENGINE::debug) {
        KP::ENGINE::OBJECT_debugger.DestroyDebugUtilsMessengerEXT(KP::ENGINE::VK_instance, debugMessenger, KP::ENGINE::VK_Allocator);
    }

    // Limpar logical device
    if (KP::ENGINE::OBJECT_device.VK_Device != VK_NULL_HANDLE) {
         vkDestroyDevice(KP::ENGINE::OBJECT_device.VK_Device, KP::ENGINE::VK_Allocator);
    }


    // Limpar surface
    if (KP::ENGINE::VK_instance != VK_NULL_HANDLE && KP::ENGINE::OBJECT_window.VK_surface != VK_NULL_HANDLE) {
        vkDestroySurfaceKHR(KP::ENGINE::VK_instance, KP::ENGINE::OBJECT_window.VK_surface, KP::ENGINE::VK_Allocator);
    }


    // Limpar instância Vulkan
    if (KP::ENGINE::VK_instance != VK_NULL_HANDLE) {
        vkDestroyInstance(KP::ENGINE::VK_instance, KP::ENGINE::VK_Allocator);
    }

    // A janela GLFW é destruída pelo destrutor de OBJECT_window ou chamada explícita
    // glfwTerminate é chamado pelo destrutor de OBJECT_window
}
*/
