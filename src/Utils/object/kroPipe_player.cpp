#include "kroPipe_player.hpp"
#include "kroPipe_object.hpp"


namespace KP {
namespace UTILS {



KP::UTILS::createInfo_player* player::getData(){
    return &data;
}


player::player(KP::UTILS::createInfo_player& info) {
    KP::UTILS::OBJECT_objectsManager.callObject(*data.ObjectID)->getData().is_player = true; // necessary
}

} //UTILS
} //KP
