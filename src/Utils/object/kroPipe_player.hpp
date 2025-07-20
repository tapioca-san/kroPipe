#ifndef KROPIPE_PLAYER_H
#define KROPIPE_PLAYER_H

#include "../../Vulkan_Engine/pipeline/kroPipe_vertex_data.hpp"
#include "../../Vulkan_Engine/buffers/kroPipe_buffer.hpp"
#include "../camera/kroPipe_camera.hpp"
#include "../../kroPipe_depedence.hpp"


namespace KP {
namespace UTILS {

struct createInfo_player {

  uint32_t *ObjectID;
  KP::UTILS::Camera camera;
  
};

class player {

public:
  player(KP::UTILS::createInfo_player &info);

  KP::UTILS::createInfo_player *getData();

private:
  createInfo_player data;
};

} // namespace UTILS
} // namespace KP

#endif//MROPIPE_PLAYER_H