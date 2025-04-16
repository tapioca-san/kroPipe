#ifndef OBJECT_H
#define OBJECT_H

#include "../kroPipe_depedence.hpp"
#include "kroPipe_struct.hpp"

static int lastID = 0;

namespace KP {
namespace OBJECT {

    struct VAO {

        std::vector<VkBuffer> vertexBuffers;
        std::vector<VkDeviceMemory> vertexBufferMemorys;
        
        std::vector<VkBuffer> indexBuffers;
        std::vector<VkDeviceMemory> indexBufferMemorys;
        
        std::vector<KP::STRUCT::Mesh> meshes;
        
        
        KP::STRUCT::UniformBufferObject UBO;
        
    };

    struct ObjectData {
        glm::vec3 Position{};
        glm::vec3 Rotation{};
        glm::vec3 Scale{};
        glm::vec3 actualPositionForUseForLastPosition{};
        glm::vec3 lastPosition{};
        glm::vec3 velocity{};
        
        VAO *vao;

        std::string modelPath;

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
        bool is_myself = false;
        bool is_OnAir = true;

        int ID = -1;
    };

    class Object {
    private:
        float calculateRaio(ObjectData& object) {
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
        
        inline void calculateAABB(ObjectData& object) {
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

    public:
        ObjectData data;

        Object(glm::vec3 position, float floorPos, bool is_myself, VAO *vao = nullptr) {
            data.Position = position;
            data.floorPos = floorPos;
            data.floorPoslowest = floorPos;
            data.is_myself = is_myself;
            data.velocity = glm::vec3(0.0f);
            data.ID = lastID++;
            data.vao = vao;
            data.Scale = glm::vec3(1);

            if(vao){
                calculateRaio(data);
                calculateAABB(data);
            }
        }

        ~Object() {
        }
    };

    

}
} // namespace kroPipe

std::vector<KP::OBJECT::Object*> allObjects;
std::vector<int> sortedID;

#endif // OBJECT_H
