#ifndef SCRIPT_H
#define SCRIPT_H

#include "../kroPipe_depedence.hpp"
#include "../kroPipe_include.hpp"

void chooseLocalPath(){
    //std::cout << "put in here the whole local path on your application: " << std::endl;
    //std::cin >> directoryFileManually;
    if(directoryFileManually == ""){
        directoryFileManually = "/home/pipebomb/dev/cpp/vulkan/teste/src/";
    }
}


#endif//SCRIPT_H