#ifndef KROPIPE_INSTANCE_HPP
#define KROPIPE_INSTANCE_HPP

#include "../../kroPipe_depedence.hpp"
#include "../debug/kroPipe_debug.hpp" // Necessário para Debugger, debug, validationLayersMessage, warnMessage

namespace KP {
namespace ENGINE {

// Variáveis globais/namespace (declaradas extern)
extern KP::ENGINE::Debugger OBJECT_debugger;

extern VkInstance VK_instance; // Criada varivel instance apenas acessivel se consultar dentro do objeto OBJECT_instace (declarada com extern)

// Classe Instance (Gerenciador da Instância Vulkan e setup inicial)
class Instance {
private:
    // Callback de redimensionamento (pode ser estático ou um método da classe)
    // Se for estático, precisa acessar o objeto Instance via UserPointer da janela GLFW.
    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

public:


    // Variáveis membro da classe (validationLayers pode ser membro ou global/namespace)
    // Mantido como membro da classe Instance como no original
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    // Métodos da classe Instance
    void createInstance();
    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();

    // Nota: A função debugCallback do Debugger não é membro de Instance,
    // mas sim de Debugger. Sua definição estava neste bloco no original,
    // mas deve ir para kroPipe_debug.cpp (e já foi movida para lá).
};


extern KP::ENGINE::Instance OBJECT_instance; //Declara uma instância local da classe Instance

} // namespace ENGINE
} // namespace KP

#endif // KROPIPE_INSTANCE_HPP