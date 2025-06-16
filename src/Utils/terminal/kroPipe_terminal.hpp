#include "../../kroPipe_depedence.hpp"

#ifndef KROPIPE_TERMINAL_HPP
#define KROPIPE_TERMINAL_HPP

namespace KP {
namespace UTILS {


struct AnsiFragment {
    ImVec4 color;
    std::string text;
};

class Terminal {
public:
    void Terminalsystem(std::string &command);
    void showTerminal();
    void restartTerminal();
private:
    ImVec4 ansiColor(int code);
    std::vector<AnsiFragment> parseAnsiLine(const std::string& line);
    void handleAnsiSequences(const std::string& rawLine);

    std::string inputBuffer;
    std::vector<std::string> commandHistory;
    std::vector<std::string> rawOutputBuffer;
    std::vector<std::string> titleHistory;
    std::string lastWindowTitle;

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
