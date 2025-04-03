#ifndef AABB_H
#define AABB_H

//#include "../kroPipe_include.hpp"
#include "kroPipe_object.hpp"


namespace kroPipe {


bool detectCollision3D(const Object& obj1, const Object& obj2) {
    return (obj1.Position.x < obj2.Position.x + obj2.width  &&
            obj1.Position.x + obj1.width > obj2.Position.x  &&
            obj1.Position.y < obj2.Position.y + obj2.height &&
            obj1.Position.y + obj1.height > obj2.Position.y &&
            obj1.Position.z < obj2.Position.z + obj2.depth  &&
            obj1.Position.z + obj1.depth > obj2.Position.z);
}

bool detectCollisionX3D(const Object& obj1, const Object& obj2) {
    return (obj1.Position.x < obj2.Position.x + obj2.width &&
            obj1.Position.x + obj1.width > obj2.Position.x);
}
bool detectCollisionY3D(const Object& obj1, const Object& obj2) {
    return (obj1.Position.y < obj2.Position.y + obj2.height &&
            obj1.Position.y + obj1.height > obj2.Position.y);
}
bool detectCollisionZ3D(const Object& obj1, const Object& obj2) {
    return (obj1.Position.z < obj2.Position.z + obj2.depth &&
            obj1.Position.z + obj1.depth > obj2.Position.z);
}

void checkCollisions3D(Object& mySelf, Object& obj, float step = 0.01f) {
    if(detectCollision3D(mySelf, obj)){
        if(detectCollisionY3D(mySelf, obj)){
            std::cerr << "floorPos\n";
            mySelf.floorPos = obj.Position.y + obj.height;
            mySelf.is_onObject = true;
        }
    }
     
    if(!detectCollisionY3D(mySelf, obj) && mySelf.is_onObject){
       mySelf.is_onObject = false;
    }
}


}





#endif//AABB_H
