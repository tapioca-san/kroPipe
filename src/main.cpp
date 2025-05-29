#include "Vulkan_Engine/kroPipe_Aplication.hpp"

int main() {
    try {
        // Usando o objeto OBJECT_aplication (declarado extern em app.hpp e definido em app.cpp)
        OBJECT_aplication.init();
        // OBJECT_aplication.run(); // Implemente seu loop principal aqui
        // OBJECT_aplication.cleanup(); // Implemente a limpeza aqui

    } catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    OBJECT_aplication.clean();

    return EXIT_SUCCESS;
}