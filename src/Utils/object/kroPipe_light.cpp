#include "../../Vulkan_Engine/buffers/kroPipe_buffer.hpp"
#include "../../Vulkan_Engine/device/kroPipe_device.hpp"
#include "../../Vulkan_Engine/debug/kroPipe_debug.hpp"
#include "../object/kroPipe_object.hpp"
#include "kroPipe_light.hpp"


namespace KP {
namespace UTILS {

KP::UTILS::light::light() {
    
}

void KP::UTILS::light::updateLightToShaders() {


  KP::UTILS::lightData lightUbo;
  lightUbo.position = glm::vec3(10.0, 0.0, 1.0);

  lightUbo.color = glm::vec3(0.0, 0.0, 1.0);
  data.intensity = 11.0f;
  memcpy(uniformBufferMapped, &lightUbo, sizeof(lightUbo));
}

void KP::UTILS::light::createUniformBuffers(){
  VkDeviceSize bufferSize = sizeof(KP::UTILS::lightData);
  KP::ENGINE::createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer, uniformBufferMemory);
  KP::ENGINE::err = vkMapMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(), uniformBufferMemory, 0, bufferSize, 0, &uniformBufferMapped);
  KP::ENGINE::check_vk_result(KP::ENGINE::err);
}

void KP::UTILS::light::definition(){
  data.intensity = 11.0f;
    data.position = KP::UTILS::OBJECT_objectsManager.getObjectByID(1)->getData().Position; 
    data.direction = glm::vec3(0.0f); 
    data.color = glm::vec3(1.0f); 
}

void KP::UTILS::light::clean(){
    vkDestroyBuffer(*KP::ENGINE::OBJECT_device.getPointerDevice(), uniformBuffer, KP::ENGINE::VK_Allocator);
    vkFreeMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(), uniformBufferMemory, KP::ENGINE::VK_Allocator);
}


KP::UTILS::light lightTest;

} // namespace UTILS
} // namespace KP
