#include "../../Vulkan_Engine/pipeline/kroPipe_pipeline.hpp"
#include "../../Vulkan_Engine/device/kroPipe_device.hpp"
#include "../../Vulkan_Engine/debug/kroPipe_debug.hpp"
#include "kroPipe_object.hpp"
#include "kroPipe_light.hpp"

namespace KP {
namespace UTILS {

void ObjectsManager::addObject(std::shared_ptr<Object> Object) {
  lastID++;
  ID.push_back(lastID);
  allObject.push_back(Object);
  for (uint32_t i = 0; i < Object->getData().object_type.size(); i++) {
    if (Object->getData().object_type[i] == "Player") {
      std::cerr << "player ID:" << lastID << "\n";
      playersID.push_back(&lastID);
    } else if (Object->getData().object_type[i] == "Camera") {
      std::cerr << "camera ID:" << lastID << "\n";
      camerasID.push_back(&lastID);
    } else if (Object->getData().object_type[i] == "Object") {
      std::cerr << "object ID:" << lastID << "\n";
      objectsID.push_back(&lastID);
    } 
    else if (Object->getData().object_type[i] == "Light") {
      std::cerr << "light ID:" << lastID << "\n";
      lightsID.push_back(&lastID);
    }else {
      std::cerr << "null ID:" << lastID << "\n";
      nullID.push_back(&lastID);
    }
  }
}

uint32_t *ObjectsManager::getPlayersID(uint32_t index) {
  return playersID[index];
}
uint32_t *ObjectsManager::getCamerasID(uint32_t index) {
  return camerasID[index];
}
uint32_t *ObjectsManager::getObjectsID(uint32_t index) {
  return objectsID[index];
}
uint32_t *ObjectsManager::getLightsID(uint32_t index) {
  return lightsID[index];
}

uint32_t *ObjectsManager::getnullID(uint32_t index) { return nullID[index]; }

int ObjectsManager::getLastId() { return lastID; }

std::shared_ptr<Model> ObjectsManager::getModelByID(uint32_t ID) {
  if (ID >= allModel.size()) {
    throw std::runtime_error("allModel ID out of range");
  }
  return allModel[ID];
}

std::shared_ptr<Object> ObjectsManager::getObjectByID(uint32_t ID) {
  if (ID >= allObject.size()) {
    throw std::runtime_error("allObject ID out of range");
  }
  return allObject[ID];
}

std::vector<std::shared_ptr<Model>> *ObjectsManager::getAllModel() {
  return &allModel;
}

std::vector<std::shared_ptr<Object>> *ObjectsManager::getAllObject() {
  return &allObject;
}

void ObjectsManager::logID() {

  ImGui::Begin("LOG ID");
  if (ImGui::CollapsingHeader("IDs")) {
    for (uint32_t i = 0; i < KP::UTILS::OBJECT_objectsManager.getAllObject()->size(); i++) {
      std::string text = "ID: " + std::to_string(i);
      ImGui::Text(text.c_str());
      text = "Path: " + KP::UTILS::OBJECT_objectsManager.getObjectByID(i)->getData().modelPath;
      ImGui::Text(text.c_str());
    }
  }
  ImGui::End();
}

float Object::calculateRaio(ObjectData &object) {
  float raio = 0.0f;
  for (uint16_t i = 0; i < object.vao->meshes.size(); i++) {
    if (object.vao->meshes[i].vertices.size() == 0)
      return 0.0f;

    float cx = 0.0f, cy = 0.0f, cz = 0.0f;

    for (size_t i = 0; i < object.vao->meshes[i].vertices.size() * 3; i += 3) {
      cx += object.vao->meshes[i].vertices[i].Position.x;
      cy += object.vao->meshes[i].vertices[i + 1].Position.y;
      cz += object.vao->meshes[i].vertices[i + 2].Position.z;
    }
    cx /= object.vao->meshes[i].vertices.size();
    cy /= object.vao->meshes[i].vertices.size();
    cz /= object.vao->meshes[i].vertices.size();

    for (size_t i = 0; i < object.vao->meshes[i].vertices.size() * 3; i += 3) {
      float dx = object.vao->meshes[i].vertices[i].Position.x - cx;
      float dy = object.vao->meshes[i].vertices[i + 1].Position.y - cy;
      float dz = object.vao->meshes[i].vertices[i + 2].Position.z - cz;
      float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
      raio = std::max(raio, distance);
    }
  }
  return raio;
}

void Object::calculateAABB(ObjectData &object) {
  try {
    for (uint16_t i = 0; i < object.vao->meshes.size(); i++) {
      if (object.vao->meshes[i].vertices.size() == 0) {
        throw std::invalid_argument("Invalid Vertices: ");
      }
      float minX = FLT_MAX, maxX = -FLT_MAX;
      float minY = FLT_MAX, maxY = -FLT_MAX;
      float minZ = FLT_MAX, maxZ = -FLT_MAX;
      for (size_t i = 0; i < object.vao->meshes[i].vertices.size() * 8;
           i += 8) {
        float x = object.vao->meshes[i].vertices[i].Position.x;
        float y = object.vao->meshes[i].vertices[i + 1].Position.y;
        float z = object.vao->meshes[i].vertices[i + 2].Position.z;
        minX = std::min(minX, x);
        maxX = std::max(maxX, x);
        minY = std::min(minY, y);
        maxY = std::max(maxY, y);
        minZ = std::min(minZ, z);
        maxZ = std::max(maxZ, z);
      }
      object.width = maxX - minX;
      object.height = maxY - minY;
      object.depth = maxZ - minZ;
    }
  } catch (const std::exception &e) {
    std::cerr << "Failed to calculate the AABB " << e.what() << std::endl;
  }
}

Object::Object(createInfo_object &Info) {

  data.Position = Info.position;
  data.floorPos = Info.floorPos;
  data.floorPoslowest = Info.floorPos;
  data.object_type = Info.object_type;
  data.velocity = glm::vec3(0.0f);
  data.Scale = glm::vec3(1.0f);

  KP::UTILS::OBJECT_objectsManager.addObject((std::shared_ptr<Object>)this);

  data.ID = Info.ptr_ObjectsManager->getLastId();

  for (uint32_t i = 0; i < Info.object_type.size(); i++) {
    if (Info.object_type[i] == "Camera" && Info.modelPath != "") {
      KP::ENGINE::warnMessage(
          "Tried to load a 3d model on a camera object. Object ID:" +
          std::to_string(data.ID));
    }
  }

  createInfo_model modelInfo;
  modelInfo.modelPath = Info.modelPath;
  modelInfo.ObjectID = &data.ID;

  std::shared_ptr<KP::UTILS::Model> model = std::make_shared<KP::UTILS::Model>(
      modelInfo,
      *KP::UTILS::OBJECT_objectsManager
           .getAllModel()); // std::shared_ptr<std::shared_ptr<KP::UTILS::Model>>(modelInfo,
                            // *KP::UTILS::OBJECT_objectsManager.getAllModel());

  model->UBO.createDescriptorLayout();
  model->UBO.create();
  model->loadModel();

  KP::UTILS::OBJECT_objectsManager.getAllModel()->push_back(model);

  data.model = model;
  data.vao = std::make_shared<KP::UTILS::VAO>(model->vao);

  for (uint32_t i = 0; i < data.object_type.size(); i++) {
    if (data.object_type[i] == "Player") {
      createInfo_player playerInfo;
      playerInfo.ObjectID = &data.ID;
      std::shared_ptr<KP::UTILS::player> model =
          std::make_shared<KP::UTILS::player>(playerInfo);
    }
    if (data.object_type[i] == "Light") {
      KP::UTILS::createInfo_light dataLight;
      dataLight.intensity = 1.0f;

    }
  }
  // (Opcional) Calcular bounding box e raio, se quiser:
  // data.raio = calculateRaio(data);
  // calculateAABB(data);
}

void Object::clean() {
  data.model->UBO.cleanUp();
  data.model->cleanupVao();
}

ObjectData &Object::getData() { return data; };

void Object::DrawTransformUI(std::string &headerName) {
  if (ImGui::CollapsingHeader(headerName.c_str())) {
    ImGui::DragFloat("float Position X", &data.Position.x, 0.01f);
    ImGui::DragFloat("float Position Y", &data.Position.y, 0.01f);
    ImGui::DragFloat("float Position Z", &data.Position.z, 0.01f);

    ImGui::DragFloat("float Rotate X", &data.Rotation.x, 0.01f);
    ImGui::DragFloat("float Rotate Y", &data.Rotation.y, 0.01f);
    ImGui::DragFloat("float Rotate Z", &data.Rotation.z, 0.01f);

    ImGui::DragFloat("float Scale X", &data.Scale.x, 0.01f);
    ImGui::DragFloat("float Scale Y", &data.Scale.y, 0.01f);
    ImGui::DragFloat("float Scale Z", &data.Scale.z, 0.01f);
  }
}

void Object::draw(VkCommandBuffer &commandBuffer) {
  if (data.model) {
    data.model->draw(commandBuffer);
  }
}

}; // namespace UTILS
} // namespace KP

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
    vkCmdBindIndexBuffer(commandBuffer, vao.indexBuffers[i], 0,VK_INDEX_TYPE_UINT16);
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

KP::UTILS::createInfo_player *KP::UTILS::player::getData() { return &data; }

KP::UTILS::player::player(KP::UTILS::createInfo_player &info) {}

namespace KP {
namespace UTILS {

ObjectsManager OBJECT_objectsManager;
} // namespace UTILS
} // namespace KP
