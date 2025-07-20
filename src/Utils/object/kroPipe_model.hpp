#ifndef KROPIPE_MODEL_H
#define KROPIPE_MODEL_H

#include "../../Vulkan_Engine/pipeline/kroPipe_vertex_data.hpp"
#include "../../Vulkan_Engine/buffers/kroPipe_buffer.hpp"
#include "../animation/kroPipe_animation.hpp"
#include "../../kroPipe_depedence.hpp"


namespace KP {
namespace UTILS {

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

  std::shared_ptr<std::vector<std::shared_ptr<Model>>> allModel; 

  KP::UTILS::Animation animation;

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

} // namespace UTILS
} // namespace KP


#endif//MROPIPE_MODEL_H

