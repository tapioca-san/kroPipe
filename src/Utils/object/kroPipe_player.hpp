#ifndef KROPIPE_PLAYER_H
#define KROPIPE_PLAYER_H

#include "../camera/kroPipe_camera.hpp"
#include "kroPipe_object.hpp"

namespace KP {
namespace UTILS {

struct createInfo_player{

    uint32_t* ObjectID;
    KP::UTILS::Camera camera;

};


class player{


    public:

    player(KP::UTILS::createInfo_player& info);
    ~player();

    KP::UTILS::createInfo_player* getData();

    private:

    createInfo_player data;


};

} //UTILS
} //KP

#endif //PLAYER_H
