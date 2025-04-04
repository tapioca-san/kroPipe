#ifndef ENTITY_H
#define ENTITY_H

#include "../kroPipe_include.hpp"
#include "../kroPipe_depedence.hpp"
#include "kroPipe_gravity.hpp"
#include "kroPipe_object.hpp"
#include "kroPipe_AABB.hpp"

//glm::vec3 Position, float floorPos, float* vertices = {}, size_t vertexCount = {}
void createEntity(glm::vec3 position, float floorPos, bool is_myself){
    kroPipe::Object player(position, floorPos, is_myself, lastID);
    allObjects.push_back(player);
    //size_t numVertices = sizeof(vertices) / sizeof(vertices[0]) / 8;
    //calculateAABB(player, vertices, numVertices);
}


std::vector<int> entityLoad(std::vector<kroPipe::Object> &allObjects){
    if(allObjects.empty()){
        throw std::runtime_error("entityLoad error: no objects available");
    }

    // sorted[1] = main character (yourself/ você mesmo)
    // sorted[2 or more] = objects
    
    int i = 0;
    
    kroPipe::Object mainPlayer = allObjects[0];
    std::vector<int> sortedID;
    std::vector<int> objectsID;

    for(const kroPipe::Object object : allObjects){
        if (object.is_myself && i == 0){
            mainPlayer = object;
            i++;
        }
        else if (object.is_myself && i == 1) {
            if(debug){
                std::cerr << "main player fetched first: " << mainPlayer.ID << "\n";
                std::cerr << "second player fetched: " << object.ID << "\n";
            }
            std::runtime_error("failed to load the objects: cant exist 2 main players");
        }
        else {
            objectsID.push_back(object.ID);
        }

    }

    sortedID.push_back(mainPlayer.ID);
    sortedID.insert(sortedID.end(), objectsID.begin(), objectsID.end());

    return sortedID;
}

void loadObjects(){

    sortedID = entityLoad(allObjects);
    
    if(sortedID.size() < 2){
        return;
    }

    for (size_t i = 1; i < sortedID.size(); i++) {
        if (sortedID[i] < static_cast<int>(allObjects.size())) {
            kroPipe::detectCollision3D(allObjects[sortedID[1]], allObjects[sortedID[i]]);
            kroPipe::gravityForce(allObjects[sortedID[i]], deltaTime);
        }
    }
}

#endif
