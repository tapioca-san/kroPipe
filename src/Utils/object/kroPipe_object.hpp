#ifndef KROPIPE_OBJECT_H
#define KROPIPE_OBJECT_H

#include "../../Vulkan_Engine/buffers/kroPipe_buffer.hpp"
#include "../../Vulkan_Engine/pipeline/kroPipe_vertex_data.hpp"
#include "../camera/kroPipe_camera.hpp"
#include "kroPipe_model.hpp"

#define MAX_BONE_INFLUENCE 4

namespace KP {
namespace UTILS {

struct ObjectsManager; // redefinition below
class player;          // redefinition below
class light;

struct ObjectData {
  glm::vec3 Position{};
  glm::vec3 Rotation{};
  glm::vec3 Scale{};
  glm::vec3 velocity{};

  std::shared_ptr<KP::UTILS::VAO> vao;
  std::string modelPath;

  uint32_t playerID = -1;

  float raio = 0.0f;
  float width = 0.0f, height = 0.0f, depth = 0.0f;

  std::shared_ptr<float> vertices = nullptr;

  size_t numVertices = 0;
  float floorPos = 0.0f;
  float floorPoslowest = 0.0f;

  bool is_touchingX = false;
  bool is_touchingY = false;
  bool is_touchingZ = false;
  bool is_onCollition = false;
  bool is_onObject = false;
  bool is_OnAir = false;
  bool gravityMode = true;

  std::vector<std::string> object_type;

  std::shared_ptr<KP::UTILS::light> light = nullptr;
  std::shared_ptr<KP::UTILS::Model> model = nullptr;
  uint32_t ID = -1;
};

struct createInfo_object {

  glm::vec3 position;
  float floorPos;
  std::vector<std::string> object_type;
  std::string modelPath;
  ObjectsManager *ptr_ObjectsManager;
};

class Object {
private:
  float calculateRaio(ObjectData &object);

  void calculateAABB(ObjectData &object);

  ObjectData data;

  KP::UTILS::player *model;

public:
  ObjectData &getData();
  Object(createInfo_object &Info);
  void DrawTransformUI(std::string &headerName);

  void draw(VkCommandBuffer &commandBuffer);

  void clean();
};

/*
-------------------------
Documentação

struct createInfo_object {
    glm::vec3 position;
    float floorPos;
    bool is_myself;
    KP::UTILS::Model* model; // só passamos para ter o caminho
};

-------------------------

*/

struct ObjectsManager {
private:
  std::vector<std::shared_ptr<Model>> allModel;
  std::vector<std::shared_ptr<Object>> allObject;

public:
  std::vector<std::shared_ptr<uint32_t>> playersID;
  std::vector<std::shared_ptr<uint32_t>> camerasID;
  std::vector<std::shared_ptr<uint32_t>> objectsID;
  std::vector<std::shared_ptr<uint32_t>> lightsID;
  std::vector<std::shared_ptr<uint32_t>> nullID;
  std::vector<uint32_t> ID;

  std::shared_ptr<uint32_t> lastID = std::make_shared<uint32_t>(-1);

  void addObject(std::shared_ptr<Object> ObjectData);
  uint32_t getLastId();
  void logID();

  std::shared_ptr<Model> getModelByID(uint32_t ID);
  std::shared_ptr<Object> getObjectByID(uint32_t ID);
  std::vector<std::shared_ptr<Model>> *getAllModel();
  std::vector<std::shared_ptr<Object>> *getAllObject();
  std::vector<std::shared_ptr<uint32_t>> getAllPlayersID();

  std::shared_ptr<uint32_t> getPlayersID(uint32_t index);
  std::shared_ptr<uint32_t> getCamerasID(uint32_t index);
  std::shared_ptr<uint32_t> getObjectsID(uint32_t index);
  std::shared_ptr<uint32_t> getLightsID(uint32_t index);
  std::shared_ptr<uint32_t> getnullID(uint32_t index);
};

extern KP::UTILS::ObjectsManager OBJECT_objectsManager;


} // namespace UTILS
} // namespace KP

#endif // KROPIPE_OBJECT_H
