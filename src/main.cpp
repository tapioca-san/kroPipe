#include "Vulkan_Engine/kroPipe_Aplication.hpp"

int main() {
  try {
    OBJECT_aplication.init();
    OBJECT_aplication.run();

  } catch (const std::exception &e) {
    std::cerr << "Fatal Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  OBJECT_aplication.clean();

  return EXIT_SUCCESS;
}
