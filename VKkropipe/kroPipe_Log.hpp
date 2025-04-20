#ifndef LOG_H
#define LOG_H

#include "../kroPipe_include.hpp"

// this file is for debug mode, so, you would use it if you were trying make a new update around debug.
// esse arquivo é de debug, então, se tu precisa usar isso, é para criar debug



std::vector<std::string> logError;

inline void defaultErrorNoText(std::string type, std::string mensage){
    if(mensage == ""){
        throw std::runtime_error("failed to" + type + " a mensage without text");
    }
}

inline void defaultMensage(std::string type, std::string mensage...){
    defaultErrorNoText(type, mensage);
    logError.push_back(mensage);
    std::cerr << "[ " << type << " ] : " << mensage << "\n";
}


template <class t>
void warnMensage(t *mensage...){
    defaultMensage("WARN", mensage);
}

template <class t>
void errorMensage(t mensage...){
    defaultMensage("ERROR", mensage);
}

template <class t>
std::string fatalMensage(t mensage...){
    return mensage;
}

template <class t>
void infoMensage(t mensage...){
    defaultMensage("INFO", mensage);
}
template <class t>
void debugMensage(t mensage...){
    if(debug)
        defaultMensage("DEBUG", mensage);
}

template <class t>
void deviceMensage(t mensage...){
    if(debug)
        defaultMensage("DEVICE", mensage);
}

template <class t>
void validationLayersMensage(t mensage...){
    if(debug)
        defaultMensage("VALIDATION LAYER", mensage);
}

template <class t>
void modelMensage(t mensage...){
    if(debug)
        defaultMensage("MODEL", mensage);
}

static void check_vk_result(VkResult err, std::string informationError = ""){ // this is for checking if creating 
    if (err == VK_SUCCESS)
        return;
    fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
    if(informationError != "")
        std::cerr << informationError;
    if (err < 0)
        abort();
}


#endif//LOG_H