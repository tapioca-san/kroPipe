#ifndef GRAVITY_H
#define GRAVITY_H

#include "kroPipe_object.hpp"

namespace kroPipe {



const float GRAVIDADE = 12.8;


//gravidade ksksk. o que tu esperaria? gravidade com realmente os calculos de gravidade que aprendi em física

inline void gravityForce(KP::OBJECT::Object *object, float deltaTime){
        object->data.velocity.y -= GRAVIDADE * deltaTime;
        object->data.Position += object->data.velocity * deltaTime;
        
        if(object->data.floorPos < object->data.floorPoslowest)
            object->data.floorPoslowest = object->data.floorPos;
        
        if(!object->data.is_onObject)
            object->data.floorPos = object->data.floorPoslowest;

        if(object->data.Position.y - object->data.raio > object->data.floorPos)
            object->data.is_OnAir = true;
        
    if(object->data.Position.y - object->data.raio < object->data.floorPos){
        object->data.Position.y = object->data.floorPos + object->data.raio;
        object->data.velocity.y = 0.0f;
        object->data.is_OnAir = false;
    }
}

inline void useGravity(std::vector<KP::OBJECT::Object*> objects, float deltaTime){

    for(KP::OBJECT::Object *object : objects){
        gravityForce(object, deltaTime);
    }
}


}

#endif//GRAVITY_H