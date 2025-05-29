#ifndef MODEL_H
#define MODEL_H

#include "../buffers/kroPipe_buffer.hpp"
#include "../pipeline/kroPipe_pipeline.hpp"



#define MAX_BONE_INFLUENCE 4
namespace KP {
namespace ENGINE {


struct Mesh {
    std::vector<VertexVulkan> vertices;
    std::vector<uint16_t> indices;
    
    VkImage diffuseTextureImage = VK_NULL_HANDLE;
    VkDeviceMemory diffuseTextureMemory = VK_NULL_HANDLE;
    VkImageView diffuseTextureView = VK_NULL_HANDLE;
    
    Mesh(std::vector<VertexVulkan> v, std::vector<uint16_t> i) 
    : vertices(std::move(v)), indices(std::move(i)) {}
};



struct VAO {
    std::vector<VkBuffer> vertexBuffers;
    std::vector<VkDeviceMemory> vertexBufferMemorys;
    
    std::vector<VkBuffer> indexBuffers;
    std::vector<VkDeviceMemory> indexBufferMemorys;
    
    std::vector<Mesh> meshes;
    
    
    UniformBufferObject UBO;
    
};

class Model {
 
public:
    std::string modelPath;
    std::string directory;
    
    VAO vao;
    KP::ENGINE::UboStorage UBO;
    
//void renderModel(Vertex &InfoModel, VertexVulkan handle)


void loadModel();

void renderToBuffer();

void draw(VkCommandBuffer commandBuffer);

void cleanupVao();

Model(std::string modelPath);

private:
    
void UboShader(uint32_t currentImage);
void createVertexBuffer(const std::vector<VertexVulkan> &vertices, VAO &vao);
void createIndexBuffer(const std::vector<uint16_t> &indices, VAO &vao);
Mesh processMesh(aiMesh* mesh, const aiScene* scene);
void processNode(aiNode* node, const aiScene* scene);

};

    
Model* createModel(std::string modelPath);

void loadAllModels();

extern KP::ENGINE::Model* glock;
extern std::vector<Model*> allModel; 

}//ENGINE
}//KP


#endif // MODEL_H
