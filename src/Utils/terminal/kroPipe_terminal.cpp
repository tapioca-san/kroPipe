#include "kroPipe_terminal.hpp"

namespace KP {
namespace UTILS {

Terminal OBJECT_terminal;

void Terminal::Start() {
    if (running) return;

    running = true;

    shell_pid = forkpty(&master_fd, nullptr, nullptr, nullptr);
    if (shell_pid == 0) {
        const char* home = getenv("HOME");
        if (home) chdir(home);
        execlp("/bin/bash", "bash", "--login", nullptr);
        _exit(1);
    }

    readerThread = std::thread(&Terminal::ReaderThread, this);
}

void Terminal::ReaderThread() {
    char buffer[512];
    while (running) {
        pollfd pfd = { master_fd, POLLIN, 0 };
        if (poll(&pfd, 1, 100) > 0) {
            ssize_t n = read(master_fd, buffer, sizeof(buffer) - 1);
            if (n > 0) {
                buffer[n] = '\0';
                std::lock_guard<std::mutex> lock(historyMutex);
                commandHistory.emplace_back(buffer);
                scrollToBottom = true;
            }
        }
    }
}

void Terminal::Shutdown() {
    running = false;
    if (readerThread.joinable()) readerThread.join();
    if (master_fd != -1) close(master_fd);
    if (shell_pid > 0) kill(shell_pid, SIGKILL);
}

void Terminal::ShowTerminal() {
    if (!running) Start();

    ImGui::Begin("Terminal");

    ImGui::BeginChild("ScrollRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_HorizontalScrollbar);
    {
        std::lock_guard<std::mutex> lock(historyMutex);
        for (const auto& line : commandHistory) {
            ImGui::TextUnformatted(line.c_str());
        }
    }
    if (scrollToBottom) {
        ImGui::SetScrollHereY(1.0f);
        scrollToBottom = false;
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
    if (ImGui::InputText("Comando", inputBuffer.data(), 256, flags)) {
        std::string command = inputBuffer + "\n";
        write(master_fd, command.c_str(), command.size());
        inputBuffer.clear();
    }

    ImGui::End();
}

} // namespace UTILS
} // namespace KP