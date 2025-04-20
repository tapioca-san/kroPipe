#ifndef AABB_H
#define AABB_H

//#include "../kroPipe_include.hpp"
#include "kroPipe_object.hpp"


namespace KP {
namespace OBJECT {

bool detectCollision3D(const Object& obj1, const Object& obj2) {
    return (obj1.data.Position.x < obj2.data.Position.x + obj2.data.width  &&
            obj1.data.Position.x + obj1.data.width > obj2.data.Position.x  &&
            obj1.data.Position.y < obj2.data.Position.y + obj2.data.height &&
            obj1.data.Position.y + obj1.data.height > obj2.data.Position.y &&
            obj1.data.Position.z < obj2.data.Position.z + obj2.data.depth  &&
            obj1.data.Position.z + obj1.data.depth > obj2.data.Position.z);
}

bool detectCollisionX3D(const Object& obj1, const Object& obj2) {
    return (obj1.data.Position.x < obj2.data.Position.x + obj2.data.width &&
            obj1.data.Position.x + obj1.data.width > obj2.data.Position.x);
}
bool detectCollisionY3D(const Object& obj1, const Object& obj2) {
    return (obj1.data.Position.y < obj2.data.Position.y + obj2.data.height &&
            obj1.data.Position.y + obj1.data.height > obj2.data.Position.y);
}
bool detectCollisionZ3D(const Object& obj1, const Object& obj2) {
    return (obj1.data.Position.z < obj2.data.Position.z + obj2.data.depth &&
            obj1.data.Position.z + obj1.data.depth > obj2.data.Position.z);
}

void checkCollisions3D(Object& mySelf, Object& obj, float step = 0.01f) {
    if(detectCollision3D(mySelf, obj)){
        if(detectCollisionY3D(mySelf, obj)){
            std::cerr << "floorPos\n";
            mySelf.data.floorPos = obj.data.Position.y + obj.data.height;
            mySelf.data.is_onObject = true;
        }
    }
     
    if(!detectCollisionY3D(mySelf, obj) && mySelf.data.is_onObject){
       mySelf.data.is_onObject = false;
    }
}


}//OBJECT
}//KP





#endif//AABB_H
