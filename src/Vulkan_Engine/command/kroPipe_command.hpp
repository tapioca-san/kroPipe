#ifndef COMMANDBUFFER_H
#define COMMANDBUFFER_H

#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace ENGINE {

extern VkCommandPool commandPool; // declaração da variavel com extern
    
extern std::vector<VkCommandBuffer> commandBuffers;


class Command{

public:    
    
void createCommandPool();
void createCommandBuffers();
void createDescriptorPool(VkDescriptorPool &descriptorPool);
void destroyCommandPool();

}; // COMMAND
inline KP::ENGINE::Command OBJECT_command;
}//COMMANDBUFFER
}//KP

#endif //COMMANDBUFFER_H