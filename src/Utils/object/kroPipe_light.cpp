#include "../../Vulkan_Engine/buffers/kroPipe_bufferManager.hpp"
#include "../../Vulkan_Engine/device/kroPipe_device.hpp"
#include "../../Vulkan_Engine/debug/kroPipe_debug.hpp"
#include "../object/kroPipe_object.hpp"
#include "kroPipe_light.hpp"


namespace KP {
namespace UTILS {

light::light() {
    
}

void light::updateLightToShaders() {


  lightUbo.position = glm::vec3(10.0, 0.0, 1.0);

  lightUbo.color = glm::vec3(0.0, 0.0, 1.0);
  data.intensity = 11.0f;
  KP::ENGINE::OBJECT_bufferManager.sendBufferDataToGpu<KP::UTILS::lightData>(bufferID, lightUbo);
}

void light::createUniformBuffers(){
  VkDeviceSize bufferSize = sizeof(KP::UTILS::lightData);
  bufferID = KP::ENGINE::OBJECT_bufferManager.addBuffer<KP::UTILS::lightData>(lightUbo);
}

void light::definition(){
  data.intensity = 11.0f;
  data.position = KP::UTILS::OBJECT_objectsManager.getObjectByID(1)->getData().Position; 
  data.direction = glm::vec3(0.0f); 
  data.color = glm::vec3(1.0f); 
}



uint32_t light::getBufferID(){
  return bufferID;
}

KP::UTILS::light lightTest;

} // namespace UTILS
} // namespace KP
