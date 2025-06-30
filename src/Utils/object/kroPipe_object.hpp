#ifndef KROPIPE_OBJECT_H
#define KROPIPE_OBJECT_H

#include "../../Vulkan_Engine/pipeline/kroPipe_vertex_data.hpp"
#include "../../Vulkan_Engine/buffers/kroPipe_buffer.hpp"

#define MAX_BONE_INFLUENCE 4

namespace KP {
namespace UTILS {

struct ObjectsManager; // redefinition below

struct Mesh {
    std::vector<KP::ENGINE::VertexVulkan> vertices;
    std::vector<uint16_t> indices;
    
    VkImage diffuseTextureImage = VK_NULL_HANDLE;
    VkDeviceMemory diffuseTextureMemory = VK_NULL_HANDLE;
    VkImageView diffuseTextureView = VK_NULL_HANDLE;
    
    Mesh(std::vector<KP::ENGINE::VertexVulkan> v, std::vector<uint16_t> i) 
    : vertices(std::move(v)), indices(std::move(i)) {}
};


struct VAO {
    std::vector<VkBuffer> vertexBuffers;
    std::vector<VkDeviceMemory> vertexBufferMemorys;
    
    std::vector<VkBuffer> indexBuffers;
    std::vector<VkDeviceMemory> indexBufferMemorys;
    
    std::vector<Mesh> meshes;
    
};

struct createInfo_model{

    std::string modelPath;
    uint16_t* ObjectID;
};

class Model {
 
public:
    std::string modelPath;
    std::string directory;
    
    KP::UTILS::VAO vao;
    KP::ENGINE::UboStorage UBO;
    
    std::vector<Model*> *allModel; // it's a pointer that grab from somewhere
    
    uint16_t* objectID;
    
//void renderModel(Vertex &InfoModel, VertexVulkan handle)


void loadModel();

void renderToBuffer();

void draw(VkCommandBuffer &commandBuffer);

void cleanupVao();

Model(createInfo_model& info, std::vector<Model*> &allModel);

private:
    
void UboShader(uint32_t currentImage);
void createVertexBuffer(const std::vector<KP::ENGINE::VertexVulkan> &vertices, VAO &vao);
void createIndexBuffer(const std::vector<uint16_t> &indices, VAO &vao);
Mesh processMesh(aiMesh* mesh, const aiScene* scene);
void processNode(aiNode* node, const aiScene* scene);

};


    
struct ObjectData {
    glm::vec3 Position{};
    glm::vec3 Rotation{};
    glm::vec3 Scale{};
    glm::vec3 velocity{};
    
    KP::UTILS::VAO *vao;
    std::string modelPath;
    KP::UTILS::Model* model = nullptr; 

    uint32_t playerID = -1;

    float raio = 0.0f;
    float width = 0.0f, height = 0.0f, depth = 0.0f;
    float* vertices = nullptr;
    size_t numVertices = 0;
    float floorPos = 0.0f;
    float floorPoslowest = 0.0f;
    bool is_touchingX = false;
    bool is_touchingY = false;
    bool is_touchingZ = false;
    bool is_onCollition = false;
    bool is_onObject = false;
    bool is_camera = false;
    bool is_player = false;
    bool is_object = false;
    bool is_OnAir = true;
    uint16_t ID = 0;
};

struct createInfo_object{

    glm::vec3 position;
    float floorPos;
    bool is_myself; 
    std::string modelPath; 
    ObjectsManager* ptr_ObjectsManager;
};

class Object {
private:

    float calculateRaio(ObjectData& object);
    
    void calculateAABB(ObjectData& object);
    
    ObjectData data;

public:

    ObjectData& getData();
    Object(createInfo_object &Info);
    void DrawTransformUI(std::string &headerName);
    
    void draw(VkCommandBuffer& commandBuffer);

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

struct ObjectsManager{
    private:
    
    std::vector<Model*> allModel;
    std::vector<Object*> allObject;
    
    public:


    ~ObjectsManager();

    std::vector<uint16_t> playersID;
    std::vector<uint16_t> camerasID;
    std::vector<uint16_t> objectsID;
    std::vector<uint16_t> ID;
    
    uint16_t lastID = -1;
    void addObject(Object* ObjectData);
    int getLastId();
    
    Model* callModel(uint32_t ID);
    Object* callObject(uint32_t ID);
    std::vector<Model*>* getAllModel();
    std::vector<Object*>* getAllObject();
    std::vector<uint16_t*>* getAllPlayersID();


};

extern KP::UTILS::ObjectsManager OBJECT_objectsManager;

} // namespace Utils
} // namespace KP


#endif //KROPIPE_OBJECT_H