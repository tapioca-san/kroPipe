#include "kroPipe_gravity.hpp"

namespace KP {
namespace UTILS {



void gravityForce(std::shared_ptr<KP::UTILS::Object> object, float deltaTime){
        object->getData().velocity.y -= GRAVIDADE * deltaTime;
        object->getData().Position += object->getData().velocity * deltaTime;
        
        if(object->getData().floorPos < object->getData().floorPoslowest)
            object->getData().floorPoslowest = object->getData().floorPos;
        
        if(!object->getData().is_onObject)
            object->getData().floorPos = object->getData().floorPoslowest;

        if(object->getData().Position.y - object->getData().raio > object->getData().floorPos)
            object->getData().is_OnAir = true;
        
    if(object->getData().Position.y - object->getData().raio < object->getData().floorPos){
        object->getData().Position.y = object->getData().floorPos + object->getData().raio;
        object->getData().velocity.y = 0.0f;
        object->getData().is_OnAir = false;
    }
}

void useGravity(std::vector<std::shared_ptr<KP::UTILS::Object>> objects, float deltaTime){

    for(std::shared_ptr<KP::UTILS::Object> object : objects){
        gravityForce(object, deltaTime);
    }
}

} // namespace UTILS
} // namespace KP
