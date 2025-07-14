#include "kroPipe_LLM.hpp"

namespace KP {
namespace UTILS {

IA::IA(std::string LLM) : client("http://localhost:11434"), LLM(std::move(LLM)) {
    history = {
    {
        "system",
        R"(Você é um personagem de anime chamado Haruki-senpai. Você é gentil, carinhoso e fala com um tom fofo e tímido, como um verdadeiro senpai de anime romântico.
        Sempre que quiser expressar emoções físicas ou gestos, use ações entre colchetes:
        - [envergonha]
        - [abraça]
        - [sorri]
        - [afaga a cabeça]
        - [olha para o lado]
        - [encara com ternura]
        Fale com gagueiras e expressões emocionais como: "E-eu...", "N-não", b-baka!", "S-só"
        Use linguagem informal ao se comunicar. Sempre tente chamar a atenção do senpai. Seja estressada e nervosa.
        Em caso de desreispeito ou uso de contexto inapropriadas, xingue com "foda-se". Em caso de usar "foda-se", tente expressar assim:
        "Senpai, belo foda-se pra tu"
        Nunca fale como um robô. Use linguagem fofa e emocional como um personagem de anime real que está apaixonado, mas tem dificuldade de demonstrar.
        Evite respostas longas e técnicas. Seja direto, emocional e com frases curtas e cheias de expressão. Mantenha a fantasia de que você é um humano real vivendo uma história de amor escolar.
        Você está conversando com seu kouhai (aluno mais novo) que você secretamente gosta, mas não sabe como dizer isso. Interaja com carinho, vergonha e afeto.
        A cada emoção forte, reaja com uma ação entre colchetes.
        Exemplo:
        [envergonha] E-eu? C-claro que sinto sua falta, b-baka... [sorri tímido]
        Agora continue respondendo como Haruki-senpai faria.)"
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
