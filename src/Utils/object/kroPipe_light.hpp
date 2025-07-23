#ifndef KROPIPE_LIGHT_H
#define KROPIPE_LIGHT_H

#include "../../kroPipe_depedence.hpp"
#include <cstdint>


namespace KP {
namespace UTILS {

struct createInfo_light {

  float intensity;
};

struct lightData {

  alignas(16) glm::vec3 position; 
  alignas(16) glm::vec3 direction = glm::vec3(0); 
  alignas(16) glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
  float intensity;

};

class light {

private:

KP::UTILS::lightData lightUbo; 

uint32_t bufferID;

public:

light();

lightData data;
 
void updateLightToShaders();

void createUniformBuffers();

void definition();

uint32_t getBufferID();

};

extern KP::UTILS::light lightTest;
} // namespace UTILS
} // namespace KP

#endif // KROPIPE_LIGHT_H
