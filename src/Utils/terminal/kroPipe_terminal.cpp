
#include "kroPipe_terminal.hpp"

namespace KP {
namespace UTILS {



// Retorna cor ImGui para código ANSI
ImVec4 Terminal::ansiColor(int code) {
    switch (code) {
        case 30: return ImVec4(0.0f, 0.0f, 0.0f, 1.0f);    // preto
        case 31: return ImVec4(1.0f, 0.0f, 0.0f, 1.0f);    // vermelho
        case 32: return ImVec4(0.0f, 1.0f, 0.0f, 1.0f);    // verde
        case 33: return ImVec4(1.0f, 1.0f, 0.0f, 1.0f);    // amarelo
        case 34: return ImVec4(0.0f, 0.0f, 1.0f, 1.0f);    // azul
        case 35: return ImVec4(1.0f, 0.0f, 1.0f, 1.0f);    // magenta
        case 36: return ImVec4(0.0f, 1.0f, 1.0f, 1.0f);    // ciano
        case 37: return ImVec4(1.0f, 1.0f, 1.0f, 1.0f);    // branco
        default: return ImGui::GetStyle().Colors[ImGuiCol_Text];
    }
}

// Parseia uma linha ANSI, retorna fragmentos coloridos para ImGui
std::vector<AnsiFragment> Terminal::parseAnsiLine(const std::string& line) {
    std::vector<AnsiFragment> fragments;
    size_t i = 0;
    ImVec4 currentColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
    std::string currentText;

    while (i < line.size()) {
        if (line[i] == '\033') {
            if (i + 1 < line.size()) {
                if (line[i + 1] == '[') {
                    if (!currentText.empty()) {
                        fragments.push_back({currentColor, currentText});
                        currentText.clear();
                    }
                    size_t m = line.find('m', i);
                    if (m == std::string::npos) break;
                    std::string code = line.substr(i + 2, m - (i + 2));
                    i = m + 1;
                    std::istringstream ss(code);
                    std::string token;
                    while (std::getline(ss, token, ';')) {
                        int n = 0;
                        try { n = std::stoi(token); } catch (...) { n = 0; }
                        if (n == 0) {
                            currentColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
                        } else if (n >= 30 && n <= 37) {
                            currentColor = ansiColor(n);
                        }
                    }
                    continue;
                } else if (line[i + 1] == ']') {
                    break; // Para tratar fora
                }
            }
        }
        currentText += line[i++];
    }

    if (!currentText.empty()) {
        fragments.push_back({currentColor, currentText});
    }

    return fragments;
}

// Trata sequências ANSI especiais como \033]0;title\007 para título da janela
void Terminal::handleAnsiSequences(const std::string& rawLine) {
    size_t pos = 0;
    while ((pos = rawLine.find("\033]0;", pos)) != std::string::npos) {
        size_t start = pos + 4;
        size_t end = rawLine.find('\007', start);
        if (end == std::string::npos) break;
        std::string title = rawLine.substr(start, end - start);

        {
            std::lock_guard<std::mutex> lock(historyMutex);
            titleHistory.push_back(title);
            lastWindowTitle = title;
        }
        pos = end + 1;
    }
}

void Terminal::Terminalsystem(std::string command) {
    if (master_fd < 0) return;

    std::string cmd = command + "\n";
    if(command == "load"){ // todo: está dando break em entrar em memoria invalidade
    }
    if(command == "restart"){
        restartTerminal();
    }

    else{
        
        write(master_fd, cmd.c_str(), cmd.size());

        {
            std::lock_guard<std::mutex> lock(historyMutex);
            commandHistory.push_back("$ " + command);
        }

        scrollToBottom = true;

    }
}

// Fecha terminal, mata processo e reinicia o shell, já rodando 'cd ~' no começo
void Terminal::restartTerminal() {
    if (running && shell_pid > 0) {
        kill(shell_pid, SIGKILL);
        shell_pid = -1;
    }

    if (master_fd >= 0) {
        close(master_fd);
        master_fd = -1;
    }

    running = false;

    // Limpa buffers
    {
        std::lock_guard<std::mutex> lock(historyMutex);
        commandHistory.clear();
        rawOutputBuffer.clear();
        titleHistory.clear();
        lastWindowTitle.clear();
    }

    // Inicializa term_settings a partir do terminal atual para evitar warnings
    struct termios term_settings;
    if (tcgetattr(STDIN_FILENO, &term_settings) == -1) {
        perror("tcgetattr");
    }
    cfmakeraw(&term_settings);

    shell_pid = forkpty(&master_fd, nullptr, &term_settings, nullptr);
    if (shell_pid == 0) {
        setenv("TERM", "xterm-256color", 1); // Define variável TERM para o shell
        execlp("bash", "bash", nullptr);
        perror("execlp");
        _exit(1);
    }
    running = true;

    // Manda cd ~ com \n para shell iniciar na home
    const char* cdCommand = "cd ~\n";
    write(master_fd, cdCommand, strlen(cdCommand));

    // Thread de leitura da saída do terminal
    readerThread = std::thread([this]() {
        char buffer[512];
        std::string leftover;

        while (true) {
            ssize_t count = read(master_fd, buffer, sizeof(buffer));
            if (count <= 0) break;

            std::string rawChunk(buffer, count);
            std::string combined = leftover + rawChunk;

            // Quebra por linhas
            size_t pos = 0;
            while (true) {
                size_t newline = combined.find('\n', pos);
                if (newline == std::string::npos) break;

                std::string line = combined.substr(pos, newline - pos);
                handleAnsiSequences(line);

                {
                    std::lock_guard<std::mutex> lock(historyMutex);
                    commandHistory.push_back(line);
                    rawOutputBuffer.push_back(line);
                    scrollToBottom = true;
                }
                pos = newline + 1;
            }
            // Salva o restante (linha incompleta) para próxima leitura
            leftover = combined.substr(pos);
        }
    });
    readerThread.detach();
}

std::vector<std::string> Terminal::getCommandHistory(){
    
    
    return commandHistory;
}

void Terminal::showTerminal() {
    if (!running) {
        restartTerminal();
    }

    
    ImGui::Begin(("Terminal - " + lastWindowTitle).c_str());

    ImVec2 size = ImGui::GetIO().DisplaySize;
    float height = size.y * 0.5f;
    ImGui::SetWindowSize(ImVec2(size.x * 0.8f, height), ImGuiCond_Once);

    ImGui::BeginChild("ScrollRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true, ImGuiWindowFlags_HorizontalScrollbar);
    {
        std::lock_guard<std::mutex> lock(historyMutex);
        for (const auto& line : commandHistory) {
            auto fragments = parseAnsiLine(line);
            for (const auto& frag : fragments) {
                ImGui::SameLine(0.0f, 0.0f);
                ImGui::TextColored(frag.color, "%s", frag.text.c_str());
            }
            ImGui::NewLine();
        }
    }
    if (scrollToBottom) {
        ImGui::SetScrollHereY(1.0f);
        scrollToBottom = false;
    }
    ImGui::EndChild();

    static char buffer[256] = "";
    static bool keepFocus = true;

    // Se a janela estiver focada e keepFocus = true, mantém foco no input
    if (ImGui::IsWindowFocused() && keepFocus) {
        ImGui::SetKeyboardFocusHere();
    }

    // InputText com flags EnterReturnsTrue para capturar Enter
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue;
    if (ImGui::InputText("Comando", buffer, sizeof(buffer), flags)) {
        std::string cmd = buffer;
        Terminalsystem(cmd);
        buffer[0] = '\0';
        keepFocus = true;  // Mantém foco após enviar comando
    }

    // Se o usuário clicar fora do input (fora do widget), perde o foco
    if (ImGui::IsWindowFocused() && !ImGui::IsItemActive() && ImGui::IsMouseClicked(0)) {
        keepFocus = false;
    }

    // Se clicar na janela do terminal, recupera o foco
    if (ImGui::IsWindowFocused() && ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {
        keepFocus = true;
    }

    ImGui::End();
}


KP::UTILS::Terminal OBJECT_terminal;

} // namespace UTILS
} // namespace KP