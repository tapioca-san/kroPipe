#ifndef ENTITY_H
#define ENTITY_H

#include "../kroPipe_include.hpp"
#include "../kroPipe_depedence.hpp"
#include "kroPipe_gravity.hpp"
#include "kroPipe_object.hpp"
#include "kroPipe_AABB.hpp"

// glm::vec3 position, float floorPos
int createEntity(glm::vec3 position, float floorPos, bool is_myself) {
    KP::OBJECT::Object player(position, floorPos, is_myself, lastID);
    allObjects.push_back(player);
    // size_t numVertices = sizeof(vertices) / sizeof(vertices[0]) / 8;
    // calculateAABB(player, vertices, numVertices);
    return lastID; 
}

std::vector<int> entityLoad(std::vector<KP::OBJECT::Object>& allObjects) {
    if (allObjects.empty()) {
        throw std::runtime_error("entityLoad error: no objects available");
    }

    // sorted[0] = main character (yourself / você mesmo)
    // sorted[1+] = other objects

    int mainPlayerCount = 0;
    KP::OBJECT::Object mainPlayer = allObjects[0];

    std::vector<int> sortedIDs;
    std::vector<int> otherIDs;

    for (const KP::OBJECT::Object& object : allObjects) {
        if (object.data.is_myself && mainPlayerCount == 0) {
            mainPlayer = object;
            mainPlayerCount++;
        }
        else if (object.data.is_myself && mainPlayerCount == 1) {
            if (debug) {
                std::cerr << "main player fetched first: " << mainPlayer.data.ID << "\n";
                std::cerr << "second player fetched: " << object.data.ID << "\n";
            }
            throw std::runtime_error("entityLoad error: can't have 2 main players");
        }
        else {
            otherIDs.push_back(object.data.ID);
        }
    }

    sortedIDs.push_back(mainPlayer.data.ID);
    sortedIDs.insert(sortedIDs.end(), otherIDs.begin(), otherIDs.end());

    return sortedIDs;
}

void loadObjects() {
    sortedID = entityLoad(allObjects);

    if (sortedID.size() < 2) {
        return;
    }

    for (size_t i = 1; i < sortedID.size(); i++) {
        if (sortedID[i] >= 0 && sortedID[i] < static_cast<int>(allObjects.size())) {
            KP::OBJECT::detectCollision3D(allObjects[sortedID[0]], allObjects[sortedID[i]]);
            kroPipe::gravityForce(allObjects[sortedID[i]], deltaTime);
        }
    }
}

#endif // ENTITY_H
