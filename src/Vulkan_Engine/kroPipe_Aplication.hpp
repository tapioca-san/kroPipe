#ifndef KROPIPE_APP_HPP
#define KROPIPE_APP_HPP

#include "../kroPipe_depedence.hpp"


// Classe principal da Aplicação
class Aplication{

public:
    VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE; // Inicializado

    // Método de inicialização
    void init();

    void clean();
    // Métodos de limpeza e loop principal (adicione conforme necessário)
    
    void run();

};

// Objeto da Aplicação (declarado extern) - Definição única em app.cpp
extern Aplication OBJECT_aplication;

#endif//#ifndef KROPIPE_APP_HPP