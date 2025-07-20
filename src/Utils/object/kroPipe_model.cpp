

#include "../../Vulkan_Engine/pipeline/kroPipe_pipeline.hpp"
#include "../../Vulkan_Engine/device/kroPipe_device.hpp"
#include "../../Vulkan_Engine/debug/kroPipe_debug.hpp"
#include "kroPipe_model.hpp"


namespace KP {
namespace UTILS {

void KP::UTILS::Model::loadModel() {
  Assimp::Importer importer;

  if (modelPath == "") {
    return;
  }

  const aiScene *scene =
      importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    KP::ENGINE::warnMessage("failed to load model: " + modelPath + "\nErro Assimp: " + importer.GetErrorString() + "\n");
    return;
  }

  directory = modelPath.substr(0, modelPath.find_last_of('/'));

  processNode(scene->mRootNode, scene);

  renderToBuffer();
}

void KP::UTILS::Model::renderToBuffer() {
  for (uint32_t i = 0; i < vao.meshes.size(); i++) {
    createVertexBuffer(vao.meshes[i].vertices, vao);
    createIndexBuffer(vao.meshes[i].indices, vao);
  }
}

void KP::UTILS::Model::draw(VkCommandBuffer &commandBuffer) {

  VkDeviceSize offsets[] = {0};
  for (uint32_t i = 0; i < vao.meshes.size(); i++) {
    VkBuffer vertexBuffers[] = {vao.vertexBuffers[i]};

    // UboShader(currentFrame);

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, vao.indexBuffers[i], 0,VK_INDEX_TYPE_UINT32);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,KP::ENGINE::pipelineLayout, 0, 1,&UBO.uniformBuffers.descriptorSets[KP::ENGINE::currentFrame], 0,nullptr);
    vkCmdDrawIndexed(commandBuffer,static_cast<uint32_t>(vao.meshes[i].indices.size()), 1, 0,0, 0);
  }
}

void KP::UTILS::Model::cleanupVao() {

  for (VkBuffer indexBuffer : vao.indexBuffers) {
    vkDestroyBuffer(*KP::ENGINE::OBJECT_device.getPointerDevice(), indexBuffer,
                    KP::ENGINE::VK_Allocator);
  }
  for (VkBuffer vertexBuffer : vao.vertexBuffers) {
    vkDestroyBuffer(*KP::ENGINE::OBJECT_device.getPointerDevice(), vertexBuffer,
                    KP::ENGINE::VK_Allocator);
  }
  for (VkDeviceMemory indexBufferMemory : vao.indexBufferMemorys) {
    vkFreeMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(),
                 indexBufferMemory, KP::ENGINE::VK_Allocator);
  }
  for (VkDeviceMemory vertexBufferMemory : vao.vertexBufferMemorys) {
    vkFreeMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(),
                 vertexBufferMemory, KP::ENGINE::VK_Allocator);
  }
}

KP::UTILS::Model::Model(createInfo_model &info,
                        std::vector<std::shared_ptr<Model>> allModel)
    : objectID(*info.ObjectID), UBO(*info.ObjectID) {

  this->modelPath = info.modelPath;
  this->allModel =
      std::make_shared<std::vector<std::shared_ptr<Model>>>(allModel);
}

void KP::UTILS::Model::createVertexBuffer(
    const std::vector<KP::ENGINE::VertexVulkan> &vertices, VAO &vao) {
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  KP::ENGINE::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                               VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                           stagingBuffer, stagingBufferMemory);

  void *data;
  vkMapMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(),
              stagingBufferMemory, 0, bufferSize, 0, &data);
  memcpy(data, vertices.data(), (size_t)bufferSize);
  vkUnmapMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(),
                stagingBufferMemory);

  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;
  KP::ENGINE::createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

  KP::ENGINE::copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

  vao.vertexBuffers.push_back(vertexBuffer);
  vao.vertexBufferMemorys.push_back(vertexBufferMemory);

  vkDestroyBuffer(*KP::ENGINE::OBJECT_device.getPointerDevice(), stagingBuffer,
                  nullptr);
  vkFreeMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(),
               stagingBufferMemory, nullptr);
}

void KP::UTILS::Model::createIndexBuffer(const std::vector<uint32_t> &indices,
                                         VAO &vao) {
  VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  KP::ENGINE::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                               VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                           stagingBuffer, stagingBufferMemory);

  void *data;
  KP::ENGINE::err = vkMapMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(),
                                stagingBufferMemory, 0, bufferSize, 0, &data);
  KP::ENGINE::check_vk_result(KP::ENGINE::err);

  memcpy(data, indices.data(), (size_t)bufferSize);
  vkUnmapMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(),
                stagingBufferMemory);

  VkBuffer indexBuffer;
  VkDeviceMemory indexBufferMemory;
  KP::ENGINE::createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

  KP::ENGINE::copyBuffer(stagingBuffer, indexBuffer, bufferSize);

  vao.indexBuffers.push_back(indexBuffer);
  vao.indexBufferMemorys.push_back(indexBufferMemory);

  vkDestroyBuffer(*KP::ENGINE::OBJECT_device.getPointerDevice(), stagingBuffer,
                  nullptr);
  vkFreeMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(),
               stagingBufferMemory, nullptr);
}

KP::UTILS::Mesh KP::UTILS::Model::processMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<KP::ENGINE::VertexVulkan> vertices;
  std::vector<uint32_t> indices;
  
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    KP::ENGINE::VertexVulkan vertex{};

    // posição
    vertex.Position = {
      mesh->mVertices[i].x,
      mesh->mVertices[i].y,
      mesh->mVertices[i].z
    };

    // normal
    if (mesh->HasNormals()) {
      vertex.Normal = {
        mesh->mNormals[i].x,
        mesh->mNormals[i].y,
        mesh->mNormals[i].z
      };
    }

    // coordenadas de textura
    if (mesh->mTextureCoords[0]) {
      vertex.TexCoords = {
        mesh->mTextureCoords[0][i].x,
        mesh->mTextureCoords[0][i].y
      };

      if (mesh->HasTangentsAndBitangents()) {
        vertex.Tangent = {
          mesh->mTangents[i].x,
          mesh->mTangents[i].y,
          mesh->mTangents[i].z
        };
        vertex.Bitangent = {
          mesh->mBitangents[i].x,
          mesh->mBitangents[i].y,
          mesh->mBitangents[i].z
        };
      } else {
        vertex.Tangent = glm::vec3(0.0f);
        vertex.Bitangent = glm::vec3(0.0f);
      }
    } else {
      vertex.TexCoords = glm::vec2(0.0f);
    }

    bool colorSet = false;

    if (mesh->HasVertexColors(0)) {
      vertex.color = {
        mesh->mColors[0][i].r,
        mesh->mColors[0][i].g,
        mesh->mColors[0][i].b
      };
      colorSet = true;
    }

    unsigned int materialIndex = mesh->mMaterialIndex;
    if (!colorSet && materialIndex < scene->mNumMaterials) {
      aiMaterial *material = scene->mMaterials[materialIndex];
      aiColor4D color;

      if (aiGetMaterialColor(material, AI_MATKEY_BASE_COLOR, &color) == AI_SUCCESS) {
        vertex.color = {color.r, color.g, color.b};
        colorSet = true;
      }

      if (!colorSet && aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &color) == AI_SUCCESS) {
        vertex.color = {color.r, color.g, color.b};
        colorSet = true;
      }
    }

    if (!colorSet) {
      vertex.color = glm::vec3(1.0f);
    }

    animation.loadAnimation(mesh, vertices);

    vertices.push_back(vertex);
  }

  // Índices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    const aiFace &face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  return Mesh(vertices, indices);
}


void KP::UTILS::Model::processNode(aiNode *node, const aiScene *scene) {
  for (uint16_t i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    vao.meshes.push_back(processMesh(mesh, scene));
  }

  for (uint16_t i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}


} // namespace UTILS
} // namespace KP