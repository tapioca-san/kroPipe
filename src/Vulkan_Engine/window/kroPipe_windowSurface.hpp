#ifndef WINDOWSURFACE_H
#define WINDOWSURFACE_H

#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace ENGINE {


class WindowSurface{
    
public:

VkSurfaceKHR VK_surface;// Inicializado para garantir estado conhecido

void createSurface(VkInstance &instance, GLFWwindow *window, VkSurfaceKHR &surface); // createSurface é para conseguir ter o "ID" da janela, mandando esse "ID" pro vulkan poder renderizar o que queremos. Tudo nisso é guarado na váriavel do tipo VkSurfaceKHR

};//CLASS WINDOWSURFACE
extern KP::ENGINE::WindowSurface OBJECT_windowSurface;
}//WINDOWSURFACE
}//KP


#endif//WINDOWSURFACE_H