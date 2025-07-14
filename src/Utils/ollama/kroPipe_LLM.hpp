#ifndef KROPIPE_LLM_HPP
#define KROPIPE_LLM_HPP


#include "../../kroPipe_depedence.hpp"
#include "ollama.hpp"

namespace KP {
namespace UTILS {

class IA {
private:
    struct Fragment {
        ImVec4 color;
        std::string text;
    };

    Ollama client;
    ollama::messages history;

    std::string LLM;
    std::string lastWindowTitle = "IA";
    std::vector<std::vector<Fragment>> commandHistory;

    std::mutex historyMutex;

    bool scrollToBottom = false;
    bool running = false;

    void conversation(const std::string& input);// TODO
    void Terminalsystem(const std::string& input);// TODO

public:

    IA(std::string LLM);// TODO
    void DrawTransformUI();// TODO

};

extern KP::UTILS::IA OBJECT_LLM;

} // namespace UTILS
} // namespace KP

#endif // KROPIPE_LLM_HPP
