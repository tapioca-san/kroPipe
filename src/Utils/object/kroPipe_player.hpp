#ifndef KROPIPE_PLAYER_H
#define KROPIPE_PLAYER_H

#include "../camera/kroPipe_camera.hpp"
#include "kroPipe_object.hpp"

namespace KP {
namespace UTILS {

struct playerInfo{

    uint32_t* ObjectID;
    KP::UTILS::Camera camera;

};


class player{


    public:


    player(KP::UTILS::playerInfo& info);
    ~player();

    KP::UTILS::playerInfo* getData();



    private:

    playerInfo data;


};

} //UTILS
} //KP

#endif //PLAYER_H
