#ifndef KROPIPE_OBJECT_H
#define KROPIPE_OBJECT_H

#include "../../Vulkan_Engine/buffers/kroPipe_buffer.hpp"
#include "../../Vulkan_Engine/pipeline/kroPipe_vertex_data.hpp"
#include "../camera/kroPipe_camera.hpp"

#define MAX_BONE_INFLUENCE 4

namespace KP {
namespace UTILS {

struct ObjectsManager; // redefinition below
class player;          // redefinition below
class light;

struct Mesh {
  std::vector<KP::ENGINE::VertexVulkan> vertices;
  std::vector<uint32_t> indices;

  VkImage diffuseTextureImage = VK_NULL_HANDLE;
  VkDeviceMemory diffuseTextureMemory = VK_NULL_HANDLE;
  VkImageView diffuseTextureView = VK_NULL_HANDLE;

  Mesh(std::vector<KP::ENGINE::VertexVulkan> v, std::vector<uint32_t> i)
      : vertices(std::move(v)), indices(std::move(i)) {}
};

struct VAO {
  std::vector<VkBuffer> vertexBuffers;
  std::vector<VkDeviceMemory> vertexBufferMemorys;

  std::vector<VkBuffer> indexBuffers;
  std::vector<VkDeviceMemory> indexBufferMemorys;

  std::vector<Mesh> meshes;
};

struct createInfo_model {

  std::string modelPath;
  uint32_t *ObjectID;
};

class Model {

public:
  std::string modelPath;
  std::string directory;

  KP::UTILS::VAO vao;
  KP::ENGINE::UboStorage UBO;

  std::shared_ptr<std::vector<std::shared_ptr<Model>>>
      allModel; // it's a pointer that grab from somewhere

  uint32_t objectID;

  // void renderModel(Vertex &InfoModel, VertexVulkan handle)

  void loadModel();

  void renderToBuffer();

  void draw(VkCommandBuffer &commandBuffer);

  void cleanupVao();

  Model(createInfo_model &info, std::vector<std::shared_ptr<Model>> allModel);

private:
  void UboShader(uint32_t currentImage);
  void createVertexBuffer(const std::vector<KP::ENGINE::VertexVulkan> &vertices,
                          VAO &vao);
  void createIndexBuffer(const std::vector<uint32_t> &indices, VAO &vao);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  void processNode(aiNode *node, const aiScene *scene);
};

struct ObjectData {
  glm::vec3 Position{};
  glm::vec3 Rotation{};
  glm::vec3 Scale{};
  glm::vec3 velocity{};

  std::shared_ptr<KP::UTILS::VAO> vao;
  std::string modelPath;
  std::shared_ptr<KP::UTILS::Model> model = nullptr;

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

  std::vector<std::string> object_type;
  
  std::shared_ptr<KP::UTILS::light> light;

  bool is_OnAir = false;
  uint32_t ID = 0;
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
  std::vector<uint32_t *> playersID;
  std::vector<uint32_t *> camerasID;
  std::vector<uint32_t *> objectsID;
  std::vector<uint32_t *> lightsID;
  std::vector<uint32_t *> nullID;
  std::vector<uint32_t> ID;

  uint32_t lastID = -1;
  void addObject(std::shared_ptr<Object> ObjectData);
  int getLastId();
  void logID();

  std::shared_ptr<Model> getModelByID(uint32_t ID);
  std::shared_ptr<Object> getObjectByID(uint32_t ID);
  std::vector<std::shared_ptr<Model>> *getAllModel();
  std::vector<std::shared_ptr<Object>> *getAllObject();
  std::vector<std::shared_ptr<uint32_t>> getAllPlayersID();

  uint32_t *getPlayersID(uint32_t index);
  uint32_t *getCamerasID(uint32_t index);
  uint32_t *getObjectsID(uint32_t index);
  uint32_t *getLightsID(uint32_t index);
  uint32_t *getnullID(uint32_t index);
};

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




extern KP::UTILS::ObjectsManager OBJECT_objectsManager;
} // namespace UTILS
} // namespace KP

#endif // KROPIPE_OBJECT_H
