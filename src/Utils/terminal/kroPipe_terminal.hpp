#include "../../kroPipe_depedence.hpp"

#ifndef KROPIPE_TERMINAL_HPP
#define KROPIPE_TERMINAL_HPP

namespace KP {
namespace UTILS {

class Terminal {
public:
    void Start();                      // Inicia o shell
    void Shutdown();                   // Encerra o shell
    void ShowTerminal();              // Desenha o terminal no ImGui

private:
    void ReaderThread();              // Loop de leitura do PTY

    std::string inputBuffer;
    std::vector<std::string> commandHistory;
    std::mutex historyMutex;

    bool scrollToBottom = false;
    bool running = false;
    int master_fd = -1;
    pid_t shell_pid = -1;
    std::thread readerThread;
};

extern Terminal OBJECT_terminal;

} // namespace UTILS
} // namespace KP

#endif
