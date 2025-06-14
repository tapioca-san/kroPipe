#include "kroPipe_model.hpp"


void KP::ENGINE::Model::loadModel() {
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

void KP::ENGINE::Model::renderToBuffer(){
    for(uint16_t i = 0; i < vao.meshes.size(); i++){
        createVertexBuffer(vao.meshes[i].vertices, vao);
        createIndexBuffer(vao.meshes[i].indices, vao);
    }
}

void KP::ENGINE::Model::draw(VkCommandBuffer &commandBuffer){
        
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

void KP::ENGINE::Model::cleanupVao(){
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

KP::ENGINE::Model::Model(std::string modelPath){
    
    this->modelPath = modelPath;

}

void KP::ENGINE::Model::UboShader(uint32_t currentImage) {
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        
    memcpy(UBO.uniformBuffers.uniformBuffersMapped[currentImage], &vao.UBO, sizeof(vao.UBO));
}

void KP::ENGINE::Model::createVertexBuffer(const std::vector<VertexVulkan> &vertices, VAO &vao) {
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

void KP::ENGINE::Model::createIndexBuffer(const std::vector<uint16_t> &indices, VAO &vao) {
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

KP::ENGINE::Mesh KP::ENGINE::Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<VertexVulkan> vertices;
    std::vector<uint16_t> indices;

    VertexVulkan vertex{};
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
                //vertex.Tangent = vector;              Não funcionando e nã sei porquê. da segmant fault. 
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

void KP::ENGINE::Model::processNode(aiNode* node, const aiScene* scene) {
    for (uint16_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        vao.meshes.push_back(processMesh(mesh, scene));
    }

    for (uint16_t i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

KP::ENGINE::Model* KP::ENGINE::createModel(std::string modelPath){ 
    KP::ENGINE::Model* model = new KP::ENGINE::Model(modelPath);
    KP::ENGINE::allModel.push_back(model);
    return model;
}

void KP::ENGINE::loadAllModels(){
    for(auto model : allModel){
        model->loadModel();
    }
}


namespace KP {
namespace ENGINE {

std::vector<Model*> allModel; 
KP::ENGINE::Model* glock = KP::ENGINE::createModel("/home/pipebomb/Downloads/base.fbx");

}//ENGINE
}//KP
