#include "kroPipe_text.hpp"

namespace KP {
namespace ENGINE {


bool findWordsOnString(std::string mensageContent, std::string phraseToFind){ // auto explicativa. ela tenta achar a parte que tu quer dentro do std::string
    std::string lastMensage = mensageContent;
    size_t pos = lastMensage.find(phraseToFind);
    if (pos != std::string::npos) {
        return true;
    } else {
        return false;
    }
}

}//ENGINE
}//KP