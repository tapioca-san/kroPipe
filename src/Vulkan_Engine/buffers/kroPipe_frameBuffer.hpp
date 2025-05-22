#ifndef KROPIPE_FRAMEBUFFER_HPP
#define KROPIPE_FRAMEBUFFER_HPP

#include "../../kroPipe_depedence.hpp"
// Incluir headers para os objetos e variáveis usados (agora que definições estão em .cpp)
#include "../image/kroPipe_imageView.hpp" // Necessário para swapChainImageViews
#include "../device/kroPipe_device.hpp" // Necessário para OBJECT_device
#include "../debug/kroPipe_debug.hpp" // Necessário para fatalMessage, VK_Allocator
#include "../depth/kroPipe_depth.hpp" // Necessário para depthImageView

namespace KP {
namespace ENGINE {

class FrameBuffer{
public:
    void createFrameBuffers();
    void CleanUpFramerBuffer();

}; //CLASS FRAMEBUFFER

// Objeto FrameBuffer (declarado extern)
extern KP::ENGINE::FrameBuffer OBJECT_frameBuffer;

} // namespace ENGINE
} // namespace KP

#endif//KROPIPE_FRAMEBUFFER_HPP