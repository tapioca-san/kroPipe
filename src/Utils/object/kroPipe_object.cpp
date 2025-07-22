#include "../../Vulkan_Engine/pipeline/kroPipe_pipeline.hpp"
#include "../../Vulkan_Engine/device/kroPipe_device.hpp"
#include "../../Vulkan_Engine/debug/kroPipe_debug.hpp"
#include "kroPipe_object.hpp"
#include "kroPipe_player.hpp"
#include "kroPipe_light.hpp"

namespace KP {
namespace UTILS {

void ObjectsManager::addObject(std::shared_ptr<Object> Object) {
  (*lastID)++;
  ID.push_back(*lastID);
  allObject.push_back(Object);
  for (uint32_t i = 0; i < Object->getData().object_type.size(); i++) {
    if (Object->getData().object_type[i] == "Player") {
      std::cerr << "player ID: " << *lastID << "\n";
      playersID.push_back(lastID);
    } else if (Object->getData().object_type[i] == "Camera") {
      std::cerr << "camera ID: " << *lastID << "\n";
      camerasID.push_back(lastID);
    } else if (Object->getData().object_type[i] == "Object") {
      std::cerr << "object ID: " << *lastID << "\n";
      objectsID.push_back(lastID);
    } else if (Object->getData().object_type[i] == "Light") {
      std::cerr << "light ID: " << *lastID << "\n";
      lightsID.push_back(lastID);
    } else {
      std::cerr << "null ID: " << *lastID << "\n";
      nullID.push_back(lastID);
    }
  }
}

std::shared_ptr<uint32_t> ObjectsManager::getPlayersID(uint32_t index) {
  return playersID[index];
}
std::shared_ptr<uint32_t> ObjectsManager::getCamerasID(uint32_t index) {
  return camerasID[index];
}
std::shared_ptr<uint32_t> ObjectsManager::getObjectsID(uint32_t index) {
  return objectsID[index];
}
std::shared_ptr<uint32_t> ObjectsManager::getLightsID(uint32_t index) {
  return lightsID[index];
}

std::shared_ptr<uint32_t> ObjectsManager::getnullID(uint32_t index) { 
  return nullID[index]; 
}

uint32_t ObjectsManager::getLastId() { 
  return *lastID; 
}

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
  for (uint16_t m = 0; m < object.vao->meshes.size(); m++) {
    const auto &mesh = object.vao->meshes[m];
    if (mesh.vertices.empty())
      return 0.0f;

    float cx = 0.0f, cy = 0.0f, cz = 0.0f;

    for (size_t v = 0; v < mesh.vertices.size(); v++) {
      cx += mesh.vertices[v].Position.x;
      cy += mesh.vertices[v].Position.y;
      cz += mesh.vertices[v].Position.z;
    }

    size_t vertexCount = mesh.vertices.size();
    cx /= vertexCount;
    cy /= vertexCount;
    cz /= vertexCount;

    for (size_t v = 0; v < vertexCount; v++) {
      float dx = mesh.vertices[v].Position.x - cx;
      float dy = mesh.vertices[v].Position.y - cy;
      float dz = mesh.vertices[v].Position.z - cz;
      float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
      raio = std::max(raio, distance);
    }
  }
  return raio;
}

void Object::calculateAABB(ObjectData &object) {
  try {
    for (uint16_t m = 0; m < object.vao->meshes.size(); m++) {
      const auto &mesh = object.vao->meshes[m];
      if (mesh.vertices.empty()) {
        throw std::invalid_argument("Invalid Vertices");
      }

      float minX = FLT_MAX, maxX = -FLT_MAX;
      float minY = FLT_MAX, maxY = -FLT_MAX;
      float minZ = FLT_MAX, maxZ = -FLT_MAX;

      for (size_t v = 0; v < mesh.vertices.size(); v++) {
        float x = mesh.vertices[v].Position.x;
        float y = mesh.vertices[v].Position.y;
        float z = mesh.vertices[v].Position.z;

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
    std::cerr << "Failed to calculate the AABB: " << e.what() << std::endl;
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

  std::shared_ptr<KP::UTILS::Model> model = std::make_shared<KP::UTILS::Model>(modelInfo, *KP::UTILS::OBJECT_objectsManager.getAllModel()); // std::shared_ptr<std::shared_ptr<KP::UTILS::Model>>(modelInfo,
                            // *KP::UTILS::OBJECT_objectsManager.getAllModel());

  model->UBO.createDescriptorLayout();
  model->UBO.create();
  model->loadModel();

  //if(data.object_type[0] == "Object"){
  //  data.model->bone.createUniformBuffers();
  //}

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
  data.raio = calculateRaio(data);
  calculateAABB(data);
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
    ImGui::Checkbox("Gravity mode", &data.gravityMode);
  }
}

void Object::draw(VkCommandBuffer &commandBuffer) {
  if (data.model) {
    data.model->draw(commandBuffer);
  }
}

}; // namespace UTILS
} // namespace KP

namespace KP {
namespace UTILS {

ObjectsManager OBJECT_objectsManager;

} // namespace UTILS
} // namespace KP
