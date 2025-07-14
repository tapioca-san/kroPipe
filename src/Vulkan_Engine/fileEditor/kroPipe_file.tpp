#ifndef KROPIPE_FILE_TPP
#define KROPIPE_FILE_TPP

#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace ENGINE {

template<typename BufferType>
BufferType FileEditor::readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file: " + filename);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    BufferType buffer(fileSize);
    file.seekg(0);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
    file.close();

    return buffer;
}

} // namespace ENGINE
} // namespace KP

#endif // KROPIPE_FILE_TPP
