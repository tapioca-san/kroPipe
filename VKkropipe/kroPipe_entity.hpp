#ifndef ENTITY_H
#define ENTITY_H

#include "../kroPipe_include.hpp"
#include "kroPipe_object.hpp"

struct entityInfo{

    glm::vec3 position; 
    float floorPos; 
    bool is_myself;

};

// glm::vec3 position, float floorPos
inline int createEntity(glm::vec3 position, float floorPos, bool is_myself) {
    KP::OBJECT::Object* player = new KP::OBJECT::Object(position, floorPos, is_myself);
    allObjects.push_back(player);
    // size_t numVertices = sizeof(vertices) / sizeof(vertices[0]) /2 8;
    // calculateAABB(player, vertices, numVertices);
    return lastID; 
}

inline std::vector<int> entityLoad(std::vector<KP::OBJECT::Object*> allObjects) {
    if (allObjects.empty()) {
        throw std::runtime_error("entityLoad error: no objects available");
    }

    // sorted[0] = main character (yourself / você mesmo)
    // sorted[1+] = other objects

    int mainPlayerCount = 0;
    KP::OBJECT::Object *mainPlayer = allObjects[0];

    std::vector<int> sortedIDs;
    std::vector<int> otherIDs;

    for (KP::OBJECT::Object *object : allObjects) {
        if (object->data.is_myself && mainPlayerCount == 0) {
            mainPlayer = object;
            mainPlayerCount++;
        }
        else if (object->data.is_myself && mainPlayerCount == 1) {
            if (debug) {
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

inline void loadObjects(std::vector<int> &IDs) {
    IDs = entityLoad(allObjects);

    if (IDs.size() < 2) {
        return;
    }

    
}

#endif // ENTITY_H
