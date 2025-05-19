#ifndef APP_HPP
#define APP_HPP

// Inclua todos os headers dos componentes que a Aplicação principal usa
#include "window/kroPipe_windowSurface.hpp"
#include "buffers/kroPipe_frameBuffer.hpp"
#include "swapchain/kroPipe_swapchain.hpp"
#include "queue/kroPipe_queuFamilies.hpp"
#include "instance/kroPipe_instance.hpp"
#include "image/kroPipe_imageView.hpp"
#include "window/kroPipe_window.hpp"
#include "device/kroPipe_device.hpp"
#include "depth/kroPipe_depth.hpp"
#include "debug/kroPipe_debug.hpp" // Necessário para debugMessenger e variáveis de debug

// Variáveis globais/namespace relacionadas a debug (já declaradas em kroPipe_debug.hpp)
// extern bool debug;
// extern std::vector<std::string> logError;
// extern KP::ENGINE::Debugger OBJECT_debugger;


// Classe principal da Aplicação
class Aplication{

public:
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE; // Inicializado

    // Método de inicialização
    void init();

    // Métodos de limpeza e loop principal (adicione conforme necessário)
    // void run();
    // void cleanup();

};

// Objeto da Aplicação (declarado extern) - Definição única em app.cpp
extern Aplication OBJECT_aplication;

#endif//APP_HPP