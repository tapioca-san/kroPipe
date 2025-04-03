#ifndef GRAVITY_H
#define GRAVITY_H

#include "../kroPipe_include.hpp"
#include "kroPipe_object.hpp"

namespace kroPipe {



const float GRAVIDADE = 12.8;


//gravidade ksksk. o que tu esperaria? gravidade com realmente os calculos de gravidade que aprendi em física

void gravityForce(kroPipe::Object& object, float deltaTime){
        object.velocity.y -= GRAVIDADE * deltaTime;
        object.Position += object.velocity * deltaTime;
        
        if(object.floorPos < object.floorPoslowest)
            object.floorPoslowest = object.floorPos;
        
        if(!object.is_onObject)
            object.floorPos = object.floorPoslowest;

        if(object.Position.y - object.raio > object.floorPos)
            object.is_OnAir = true;
        
    if(object.Position.y - object.raio < object.floorPos){
        object.Position.y = object.floorPos + object.raio;
        object.velocity.y = 0.0f;
        object.is_OnAir = false;
    }
}

void useGravity(std::vector<kroPipe::Object>& objects, float deltaTime){

    for(kroPipe::Object& object : objects){
        gravityForce(object, deltaTime);
    }
}


}

#endif//GRAVITY_H