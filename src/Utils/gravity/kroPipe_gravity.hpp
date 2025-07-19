#ifndef GRAVITY_H
#define GRAVITY_H

#include "../../kroPipe_depedence.hpp"
#include "../object/kroPipe_object.hpp"

namespace KP {
namespace UTILS {


const float GRAVIDADE = 9.8;


//gravidade ksksk. o que tu esperaria? gravidade com realmente os calculos de gravidade que aprendi em física

void gravityForce(std::shared_ptr<KP::UTILS::Object> object, float deltaTime);
void useGravity(std::vector<std::shared_ptr<KP::UTILS::Object>> objects, float deltaTime);



} // namespace UTILS
} // namespace KP


#endif//GRAVITY_H