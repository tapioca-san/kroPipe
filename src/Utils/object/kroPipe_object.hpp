#ifndef KROPIPE_OBJECT_H
#define KROPIPE_OBJECT_H

#include "../../Vulkan_Engine/buffers/kroPipe_buffer.hpp"
#include "../../kroPipe_depedence.hpp"
#include "../load/kroPipe_model.hpp"

namespace KP {
namespace UTILS {


    struct ObjectData {
        glm::vec3 Position{};
        glm::vec3 lastPosition{};
        glm::vec3 Rotation{};
        glm::vec3 Scale{};
        glm::vec3 velocity{};
        
        KP::UTILS::VAO *vao;

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
        float calculateRaio(ObjectData& object);
        
        void calculateAABB(ObjectData& object);

    public:
        ObjectData data;

        Object(glm::vec3 position, float floorPos, bool is_myself, KP::UTILS::VAO *vao = nullptr);

        void DrawTransformUI(std::string &headerName);

        
        ~Object() {
        }
    };

    void loadObjects(std::vector<int> &IDs);    
    std::vector<int> entityLoad(std::vector<KP::UTILS::Object*> allObjects);
    int createEntity(glm::vec3 position, float floorPos, bool is_myself);
    
    extern uint16_t lastID;
    extern std::vector<KP::UTILS::Object*> allObjects;
    extern std::vector<int> sortedID;

    

} // namespace engine
} // namespace KP


#endif //KROPIPE_OBJECT_H
