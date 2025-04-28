#ifndef MODEL_H
#define MODEL_H

#include "../VKconfiguration/kroPipe_buffer.hpp"
#include "../kroPipe_include.hpp"
#include "kroPipe_Log.hpp"
#include "kroPipe_struct.hpp"


#define MAX_BONE_INFLUENCE 4
namespace KP {
namespace OBJECT {



class Model {
 
public:
    std::string modelPath;
    std::string directory;
    
    VAO vao;
    KP::BUFFER::UboStorage UBO;


    std::vector<VkDeviceMemory> uniformBufferMemory;
    
//void renderModel(Vertex &InfoModel, VertexVulkan handle)


void loadModel() {
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

void renderToBuffer(){
    for(uint16_t i = 0; i < vao.meshes.size(); i++){
        createVertexBuffer(vao.meshes[i].vertices, vao);
        createIndexBuffer(vao.meshes[i].indices, vao);
    }

}

void Draw(VkCommandBuffer commandBuffer){
        
    VkDeviceSize offsets[] = {0};
    for(uint16_t i = 0; i < vao.meshes.size(); i++){
        VkBuffer vertexBuffers[] = {vao.vertexBuffers[i]};
        
        //UboShader(currentFrame);

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, vao.indexBuffers[i], 0, VK_INDEX_TYPE_UINT16);
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &UBO.uniformBuffers.descriptorSets[currentFrame], 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(vao.meshes[i].indices.size()), 1, 0, 0, 0);
    }
}

void cleanupVao(){
    /*
    vkDestroyBuffer(device, vao.indexBuffer, nullptr);
    vkFreeMemory(device, vao.indexBufferMemory, nullptr);
    vkDestroyBuffer(device, vao.vertexBuffer, nullptr);
    vkFreeMemory(device, vao.vertexBufferMemory, nullptr);
    */
    
    for(VkBuffer indexBuffer : vao.indexBuffers){
        vkDestroyBuffer(g_Device,indexBuffer, Allocator);
    }
    for(VkBuffer vertexBuffer : vao.vertexBuffers){
        vkDestroyBuffer(g_Device, vertexBuffer, Allocator);
    }
    for(VkDeviceMemory indexBufferMemory : vao.indexBufferMemorys){
        vkFreeMemory(g_Device, indexBufferMemory, Allocator);
    }
    for(VkDeviceMemory vertexBufferMemory : vao.vertexBufferMemorys){
        vkFreeMemory(g_Device, vertexBufferMemory, Allocator);
    }
}

Model(std::string modelPath){
    
    this->modelPath = modelPath;

}

private:
    
    void UboShader(uint32_t currentImage) {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

            
        memcpy(UBO.uniformBuffers.uniformBuffersMapped[currentImage], &vao.UBO, sizeof(vao.UBO));
    }

    void createVertexBuffer(const std::vector<KP::STRUCT::VertexVulkan> &vertices, VAO &vao) {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        KP::BUFFER::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                     stagingBuffer, stagingBufferMemory);
    
        void* data;
        vkMapMemory(g_Device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(g_Device, stagingBufferMemory);
    
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        KP::BUFFER::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                     vertexBuffer, vertexBufferMemory);
    
        KP::BUFFER::copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
    
        vao.vertexBuffers.push_back(vertexBuffer);
        vao.vertexBufferMemorys.push_back(vertexBufferMemory);
    
        vkDestroyBuffer(g_Device, stagingBuffer, nullptr);
        vkFreeMemory(g_Device, stagingBufferMemory, nullptr);
    }
    
    void createIndexBuffer(const std::vector<uint16_t> &indices, VAO &vao) {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
    
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        KP::BUFFER::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                     stagingBuffer, stagingBufferMemory);
    
        void* data;
        err = vkMapMemory(g_Device, stagingBufferMemory, 0, bufferSize, 0, &data);
        check_vk_result(err);
        
        memcpy(data, indices.data(), (size_t) bufferSize);
        vkUnmapMemory(g_Device, stagingBufferMemory);
    
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        KP::BUFFER::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
                     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                     indexBuffer, indexBufferMemory);
    
        KP::BUFFER::copyBuffer(stagingBuffer, indexBuffer, bufferSize);
    
        vao.indexBuffers.push_back(indexBuffer);
        vao.indexBufferMemorys.push_back(indexBufferMemory);
    
        vkDestroyBuffer(g_Device, stagingBuffer, nullptr);
        vkFreeMemory(g_Device, stagingBufferMemory, nullptr);
    }

    KP::STRUCT::Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<KP::STRUCT::VertexVulkan> vertices;
        std::vector<uint16_t> indices;
    
        KP::STRUCT::VertexVulkan vertex{};
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
                // positions
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
                // texture coordinates
                if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    glm::vec2 vec;
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    vec.x = mesh->mTextureCoords[0][i].x; 
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoords = vec;
                    //tangent
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.Tangent = vector;
                    // bitangent
                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.Bitangent = vector;
                }
                else
                    vertex.TexCoords = glm::vec2(0.0f, 0.0f);
    
                vertices.push_back(vertex);
            }

            // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);        
            }
    
        return KP::STRUCT::Mesh(vertices, indices);
    }
    
    void processNode(aiNode* node, const aiScene* scene) {
        for (uint16_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            vao.meshes.push_back(processMesh(mesh, scene));
        }
    
        for (uint16_t i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

};

// VARIABLES ------------------------------------------------------
    inline std::vector<Model*> allModel; // memoria apagada na Vkconfiguration/kroPipe_instance.hpp, função ~instance em cleanPointers
//
    
inline Model* createModel(std::string modelPath){ 
    Model* model = new Model(modelPath);
    allModel.push_back(model);
    return model;
}

inline void loadAllModels(){
    for(auto model : allModel){
        model->loadModel();
    }
}
}
}    
inline KP::OBJECT::Model* glock = KP::OBJECT::createModel("/home/pipebomb/Downloads/model3D/m4a1.obj");
inline KP::OBJECT::Model* minhasCabeca = KP::OBJECT::createModel("/home/pipebomb/Downloads/model3D/project_-_cirno_fumo_3d_scan.glb");


#endif // MODEL_H
