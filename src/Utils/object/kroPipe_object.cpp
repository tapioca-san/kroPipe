#include "kroPipe_object.hpp"


namespace KP {
namespace ENGINE {

std::vector<KP::ENGINE::Object*> allObjects;
std::vector<int> sortedID;
uint16_t lastID = 0;

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
Object::Object(glm::vec3 position, float floorPos, bool is_myself, VAO *vao) {
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

// glm::vec3 position, float floorPos
int createEntity(glm::vec3 position, float floorPos, bool is_myself) {
    KP::ENGINE::Object* player = new KP::ENGINE::Object(position, floorPos, is_myself);
    allObjects.push_back(player);
    // size_t numVertices = sizeof(vertices) / sizeof(vertices[0]) /2 8;
    // calculateAABB(player, vertices, numVertices);
    return lastID; 
}

std::vector<int> entityLoad(std::vector<KP::ENGINE::Object*> allObjects) {
    if (allObjects.empty()) {
        throw std::runtime_error("entityLoad error: no objects available");
    }

    // sorted[0] = main character (yourself / você mesmo)
    // sorted[1+] = other objects

    int mainPlayerCount = 0;
    KP::ENGINE::Object *mainPlayer = allObjects[0];

    std::vector<int> sortedIDs;
    std::vector<int> otherIDs;

    for (KP::ENGINE::Object *object : allObjects) {
        if (object->data.is_myself && mainPlayerCount == 0) {
            mainPlayer = object;
            mainPlayerCount++;
        }
        else if (object->data.is_myself && mainPlayerCount == 1) {
            if (false) {
                std::cerr << "main player fetched first: " << mainPlayer->data.ID << "\n";
                std::cerr << "second player fetched: " << object->data.ID << "\n";
            }
            throw std::runtime_error("entityLoad error: can't have 2 main players");
        }
        else {
            otherIDs.push_back(object->data.ID);
        }
    }

    sortedIDs.push_back(mainPlayer->data.ID);
    sortedIDs.insert(sortedIDs.end(), otherIDs.begin(), otherIDs.end());

    return sortedIDs;
}


void loadObjects(std::vector<int> &IDs) {
    IDs = entityLoad(allObjects);

    if (IDs.size() < 2) {
        return;
    }    
}


}; // namespace engine
} // namespace kroPipe

