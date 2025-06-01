#ifndef KROPIPE_FRAMEBUFFER_HPP
#define KROPIPE_FRAMEBUFFER_HPP

#include "../../kroPipe_depedence.hpp"

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