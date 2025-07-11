#ifndef KROPIPE_OS_HPP
#define KROPIPE_OS_HPP

#include "../../kroPipe_depedence.hpp"

namespace KP{
namespace ENGINE{

    class OperationSystem{
        
        public:

        bool isWayland();
        bool isX11();
        bool isWindow();
        bool isLinux();
    };

extern KP::ENGINE::OperationSystem Object_operationSystem;
}//ENGINE
}//KP



#endif//KROPIPE_OS_HPP