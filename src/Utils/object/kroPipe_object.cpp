#include "../../Vulkan_Engine/pipeline/kroPipe_pipeline.hpp"
#include "../../Vulkan_Engine/device/kroPipe_device.hpp"
#include "kroPipe_object.hpp"

namespace KP {
namespace UTILS {

void ObjectsManager::addObject(ObjectData& ObjectData){
    lastID++;
    ID.push_back(lastID);

    if(ObjectData.is_player == true){
        playersID.push_back(lastID);
    }
    else if(ObjectData.is_camera == true){
        camerasID.push_back(lastID);
    }
    else if(ObjectData.is_object == true){
        objectsID.push_back(lastID);
    }
}
int ObjectsManager::getLastId(){
    return lastID;
}

Model* ObjectsManager::callModel(uint32_t ID){ // this should be callObject, not callModel
    return allModel[ID];
}

std::vector<Model*>* ObjectsManager::getAllModel(){
    return &allModel;
}

float Object::calculateRaio(ObjectData& object) {
    float raio = 0.0f;
    for(uint16_t i = 0; i < object.vao->meshes.size(); i++){
        if (object.vao->meshes[i].vertices.size() == 0) return 0.0f;
        
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

void Object::calculateAABB(ObjectData& object) {
    try {
        for(uint16_t i = 0; i < object.vao->meshes.size(); i++){
        if (object.vao->meshes[i].vertices.size() == 0) {
            throw std::invalid_argument("Invalid Vertices: ");
        }
        float minX = FLT_MAX, maxX = -FLT_MAX;
        float minY = FLT_MAX, maxY = -FLT_MAX;
        float minZ = FLT_MAX, maxZ = -FLT_MAX;
        for (size_t i = 0; i < object.vao->meshes[i].vertices.size() * 8; i += 8) {
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
    } catch (const std::exception& e) {
        std::cerr << "Failed to calculate the AABB " << e.what() << std::endl;
    }
}

ObjectData data;

Object::Object(createInfo_object &Info) {
    data.Position = Info.position;
    data.floorPos = Info.floorPos;
    data.floorPoslowest = Info.floorPos;
    data.is_camera = false;
    data.is_player = Info.is_myself;
    data.is_object = !Info.is_myself;
    data.velocity = glm::vec3(0.0f);
    data.Scale = glm::vec3(1.0f);

    // Criação e carregamento do modelo
   

   

    Info.ObjectsManager.addObject(data);
    data.ID = Info.ObjectsManager.getLastId();
    
    createInfo_model modelInfo;
    modelInfo.modelPath = Info.model->modelPath;
    modelInfo.ObjectID = &data.ID;

    KP::UTILS::Model* model = new KP::UTILS::Model(modelInfo, *KP::UTILS::OBJECT_objectsManager.getAllModel());
    model->UBO.createDescriptorLayout();
    model->UBO.create();
    model->loadModel();
    KP::UTILS::OBJECT_objectsManager.getAllModel()->push_back(model);

    data.model = model;
    data.vao = &model->vao;

    if (data.vao) {
        data.raio = calculateRaio(data);
        calculateAABB(data);
    }
}

ObjectData Object::getData(){
    return data;
};

void Object::DrawTransformUI(std::string &headerName){
    if(ImGui::CollapsingHeader(headerName.c_str())){
        ImGui::DragFloat("float Position X", &data.Position.x, 0.1f);
        ImGui::DragFloat("float Position Y", &data.Position.y, 0.1f);
        ImGui::DragFloat("float Position Z", &data.Position.z, 0.1f);
        
        ImGui::DragFloat("float Rotate X", &data.Rotation.x, 0.1f);
        ImGui::DragFloat("float Rotate Y", &data.Rotation.y, 0.1f);
        ImGui::DragFloat("float Rotate Z", &data.Rotation.z, 0.1f);
        
        ImGui::DragFloat("float Scale X", &data.Scale.x, 0.1f);
        ImGui::DragFloat("float Scale Y", &data.Scale.y, 0.1f);
        ImGui::DragFloat("float Scale Z", &data.Scale.z, 0.1f);
    }
}


void Object::draw(VkCommandBuffer& commandBuffer) {
    if (data.model) {
        data.model->draw(commandBuffer);
    }
}


}; // namespace UTILS
} // namespace kroPipe




void KP::UTILS::Model::loadModel() {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "failed to load model: " << modelPath << "\nErro Assimp: " << importer.GetErrorString() << std::endl;
        return;
    }

    directory = modelPath.substr(0, modelPath.find_last_of('/'));

    processNode(scene->mRootNode, scene);
    
    renderToBuffer();
    
}

void KP::UTILS::Model::renderToBuffer(){
    for(uint16_t i = 0; i < vao.meshes.size(); i++){
        createVertexBuffer(vao.meshes[i].vertices, vao);
        createIndexBuffer(vao.meshes[i].indices, vao);
    }
}

void KP::UTILS::Model::draw(VkCommandBuffer &commandBuffer){
    
    VkDeviceSize offsets[] = {0};
    for(uint16_t i = 0; i < vao.meshes.size(); i++){
        VkBuffer vertexBuffers[] = {vao.vertexBuffers[i]};
        
        //UboShader(currentFrame);

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, vao.indexBuffers[i], 0, VK_INDEX_TYPE_UINT16);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, KP::ENGINE::pipelineLayout, 0, 1, &UBO.uniformBuffers.descriptorSets[KP::ENGINE::currentFrame], 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(vao.meshes[i].indices.size()), 1, 0, 0, 0);
    }
}

void KP::UTILS::Model::cleanupVao(){
    /*
    vkDestroyBuffer(device, vao.indexBuffer, nullptr);
    vkFreeMemory(device, vao.indexBufferMemory, nullptr);
    vkDestroyBuffer(device, vao.vertexBuffer, nullptr);
    vkFreeMemory(device, vao.vertexBufferMemory, nullptr);
    */
    
    for(VkBuffer indexBuffer : vao.indexBuffers){
        vkDestroyBuffer(KP::ENGINE::OBJECT_device.getDevice(),indexBuffer, KP::ENGINE::VK_Allocator);
    }
    for(VkBuffer vertexBuffer : vao.vertexBuffers){
        vkDestroyBuffer(KP::ENGINE::OBJECT_device.getDevice(), vertexBuffer, KP::ENGINE::VK_Allocator);
    }
    for(VkDeviceMemory indexBufferMemory : vao.indexBufferMemorys){
        vkFreeMemory(KP::ENGINE::OBJECT_device.getDevice(), indexBufferMemory, KP::ENGINE::VK_Allocator);
    }
    for(VkDeviceMemory vertexBufferMemory : vao.vertexBufferMemorys){
        vkFreeMemory(KP::ENGINE::OBJECT_device.getDevice(), vertexBufferMemory, KP::ENGINE::VK_Allocator);
    }
}

KP::UTILS::Model::Model(createInfo_model& info, std::vector<Model*> &allModel) : objectID(info.ObjectID), UBO(*info.ObjectID){
    
    this->modelPath = modelPath;
    this->allModel = &allModel;

}



void KP::UTILS::Model::createVertexBuffer(const std::vector<KP::ENGINE::VertexVulkan> &vertices, VAO &vao) {
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    KP::ENGINE::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                 stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(KP::ENGINE::OBJECT_device.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(KP::ENGINE::OBJECT_device.getDevice(), stagingBufferMemory);

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    KP::ENGINE::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                 vertexBuffer, vertexBufferMemory);

    KP::ENGINE::copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vao.vertexBuffers.push_back(vertexBuffer);
    vao.vertexBufferMemorys.push_back(vertexBufferMemory);

    vkDestroyBuffer(KP::ENGINE::OBJECT_device.getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(KP::ENGINE::OBJECT_device.getDevice(), stagingBufferMemory, nullptr);
}

void KP::UTILS::Model::createIndexBuffer(const std::vector<uint16_t> &indices, VAO &vao) {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    KP::ENGINE::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                 stagingBuffer, stagingBufferMemory);

    void* data;
    KP::ENGINE::err = vkMapMemory(KP::ENGINE::OBJECT_device.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
    KP::ENGINE::check_vk_result(KP::ENGINE::err);
    
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(KP::ENGINE::OBJECT_device.getDevice(), stagingBufferMemory);

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    KP::ENGINE::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                 indexBuffer, indexBufferMemory);

    KP::ENGINE::copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vao.indexBuffers.push_back(indexBuffer);
    vao.indexBufferMemorys.push_back(indexBufferMemory);

    vkDestroyBuffer(KP::ENGINE::OBJECT_device.getDevice(), stagingBuffer, nullptr);
    vkFreeMemory(KP::ENGINE::OBJECT_device.getDevice(), stagingBufferMemory, nullptr);
}

KP::UTILS::Mesh KP::UTILS::Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<KP::ENGINE::VertexVulkan> vertices;
    std::vector<uint16_t> indices;

    KP::ENGINE::VertexVulkan vertex{};
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            glm::vec3 vector;
            
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            if(mesh->mTextureCoords[0]) 
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                ////tangent
                //vector.x = mesh->mTangents[i].x;
                //vector.y = mesh->mTangents[i].y;
                //vector.z = mesh->mTangents[i].z;
                //vertex.Tangent = vector;              Não funcionando e não sei porquê. da segmant fault. 
                //// bitangent
                //vector.x = mesh->mBitangents[i].x;
                //vector.y = mesh->mBitangents[i].y;
                //vector.z = mesh->mBitangents[i].z;
                //vertex.Bitangent = vector;
            }
            else{
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }
            
            vertices.push_back(vertex);
        }
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);        
        }

    return Mesh(vertices, indices);
}

void KP::UTILS::Model::processNode(aiNode* node, const aiScene* scene) {
    for (uint16_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        vao.meshes.push_back(processMesh(mesh, scene));
    }

    for (uint16_t i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}




namespace KP {
namespace UTILS {


ObjectsManager OBJECT_objectsManager;
}//UTILS
}//KP
