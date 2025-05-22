#ifndef VERTEX_H
#define VERTEX_H

//#include "../VKkropipe/kroPipe_model.hpp"

#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace ENGINE {

class Vertex{
public:

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
VkCommandBuffer beginSingleTimeCommands();

void endSingleTimeCommands(VkCommandBuffer commandBuffer);

};//CLASS VERTEX

extern KP::ENGINE::Vertex OBJECT_vertex;

extern VkMemoryRequirements memRequirements;
extern VkPhysicalDeviceMemoryProperties memProperties;
}//VERTEX
}//KP


#endif //VERTEX_H