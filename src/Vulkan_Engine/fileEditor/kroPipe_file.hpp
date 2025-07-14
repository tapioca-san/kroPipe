#ifndef KROPIPE_FILE_HPP
#define KROPIPE_FILE_HPP

#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace ENGINE {

class FileEditor {
public:
    template<typename BufferType = std::vector<char>>
    BufferType readFile(const std::string& filename);
};

extern FileEditor OBJECT_fileEditor;

} // namespace ENGINE
} // namespace KP

#include "kroPipe_file.tpp"

#endif // KROPIPE_FILE_HPP
