#include "kroPipe_os.hpp"
#include <GLFW/glfw3.h>

namespace KP{
namespace ENGINE{

bool OperationSystem::isWayland() {
    const char* session = std::getenv("XDG_SESSION_TYPE");
    if (session && std::string(session) == "wayland") {
        return true;
    }
    return false;
}

bool OperationSystem::isX11() {
    const char* session = std::getenv("XDG_SESSION_TYPE");
    if (session && std::string(session) == "tty") {
        return true;
    }
    return false;
}

bool OperationSystem::isWindow(){
    #ifdef  _WIN64
    return true;
    #endif
    return false;
}

bool OperationSystem::isLinux(){
    #ifdef __linux__
    return true;
    #endif
    return false;
}

KP::ENGINE::OperationSystem Object_operationSystem;

}//ENGINE
}//KP