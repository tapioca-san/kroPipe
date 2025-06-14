#ifndef KROPIPE_OBJECT_H
#define KROPIPE_OBJECT_H

#include "../../kroPipe_depedence.hpp"
#include "../../Vulkan_Engine/buffers/kroPipe_buffer.hpp"
#include "../../Vulkan_Engine/load/kroPipe_model.hpp"

namespace KP {
namespace ENGINE {

    uint16_t lastID = 0;

    struct ObjectData {
        glm::vec3 Position{};
        glm::vec3 lastPosition{};
        glm::vec3 Rotation{};
        glm::vec3 Scale{};
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
        float calculateRaio(ObjectData& object);
        
        void calculateAABB(ObjectData& object);

    public:
        ObjectData data;

        Object(glm::vec3 position, float floorPos, bool is_myself, VAO *vao = nullptr);

        void DrawTransformUI(std::string &headerName);

        void loadObjects(std::vector<int> &IDs);

        std::vector<int> entityLoad(std::vector<KP::ENGINE::Object*> allObjects);

        int createEntity(glm::vec3 position, float floorPos, bool is_myself);

        ~Object() {
        }
    };

    

}
} // namespace kroPipe

extern std::vector<KP::ENGINE::Object*> allObjects;
extern std::vector<int> sortedID;

#endif //KROPIPE_OBJECT_H
