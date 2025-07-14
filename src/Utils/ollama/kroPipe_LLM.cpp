#include "kroPipe_LLM.hpp"

namespace KP {
namespace UTILS {

IA::IA(std::string LLM) : client("http://localhost:11434"), LLM(std::move(LLM)) {
    history = {
        {
            "system",
            R"(Você é um personagem de anime chamado Haruki-senpai. Você é gentil, carinhoso e fala com um tom fofo e tímido, como um verdadeiro senpai de anime romântico...
[continua como seu prompt original])"
        }
    };
}

void IA::conversation(const std::string& input) { // TODO
    if (input == "sair") {
        std::lock_guard<std::mutex> lock(historyMutex);
        commandHistory.push_back({ {ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "Encerrando conversa..."} });
        return;
    }

    history.push_back({"user", input});

    try {
        auto response = client.chat(LLM, history);
        std::string reply = response;

        std::lock_guard<std::mutex> lock(historyMutex);
        commandHistory.push_back({ {ImVec4(0.6f, 1.0f, 0.6f, 1.0f), "Haruki-senpai: " + reply} });

        history.push_back({"assistant", reply});
    } catch (const std::exception& e) {
        std::lock_guard<std::mutex> lock(historyMutex);
        commandHistory.push_back({ {ImVec4(1.0f, 0.3f, 0.3f, 1.0f), "[ERRO] " + std::string(e.what())} });
    }

    scrollToBottom = true;
}

void IA::Terminalsystem(const std::string& input) {// TODO
    std::lock_guard<std::mutex> lock(historyMutex);
    commandHistory.push_back({ {ImVec4(1.0f, 1.0f, 0.3f, 1.0f), "> " + input} });
    conversation(input);
}

void IA::DrawTransformUI() {// TODO
    ImGui::Begin(("Terminal - " + lastWindowTitle).c_str());

    ImVec2 size = ImGui::GetIO().DisplaySize;
    float height = size.y * 0.5f;
    ImGui::SetWindowSize(ImVec2(size.x * 0.8f, height), ImGuiCond_Once);

    ImGui::BeginChild("ScrollRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing() - 10), true, ImGuiWindowFlags_HorizontalScrollbar);
    {
        std::lock_guard<std::mutex> lock(historyMutex);
        for (const auto& line : commandHistory) {
            for (const auto& frag : line) {
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
    static int historyIndex = -1;
    static std::vector<std::string> inputHistory;

    ImGui::Separator();
    ImGui::PushItemWidth(-1);

    ImGuiInputTextFlags flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory;

    auto historyCallback = [](ImGuiInputTextCallbackData* data) -> int {
        if (data->EventKey == ImGuiKey_UpArrow) {
            if (historyIndex + 1 < (int)inputHistory.size()) historyIndex++;
        } else if (data->EventKey == ImGuiKey_DownArrow) {
            if (historyIndex > 0) historyIndex--;
            else historyIndex = -1;
        }

        if (historyIndex >= 0 && historyIndex < (int)inputHistory.size()) {
            const std::string& cmd = inputHistory[inputHistory.size() - 1 - historyIndex];
            std::strncpy(data->Buf, cmd.c_str(), data->BufSize);
            data->BufDirty = true;
            data->CursorPos = data->SelectionStart = data->SelectionEnd = (int)strlen(data->Buf);
        } else if (historyIndex == -1) {
            data->Buf[0] = '\0';
            data->BufDirty = true;
        }

        return 0;
    };

    if (ImGui::InputText("Comando", buffer, sizeof(buffer), flags, historyCallback)) {
        std::string cmd = buffer;
        if (!cmd.empty()) {
            inputHistory.push_back(cmd);
            Terminalsystem(cmd);
            buffer[0] = '\0';
            scrollToBottom = true;
            historyIndex = -1;
        }
    }

    if (ImGui::IsWindowFocused() && keepFocus) {
        ImGui::SetKeyboardFocusHere(-1);
    }

    ImGui::PopItemWidth();
    ImGui::End();
}
KP::UTILS::IA OBJECT_LLM("deepseek-llm");

} // namespace UTILS
} // namespace KP
