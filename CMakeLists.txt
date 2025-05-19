cmake_minimum_required(VERSION 3.10)
project(nial)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# Caminho do ImGui
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/include/imgui-master")

# Opções do GLFW (opcional)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only" FORCE)
list(APPEND SOURCES src/include/imageSupport/stb_image.cpp)

# Seus arquivos do projeto
file(GLOB SOURCES 
    "src/*.cpp" 
    "src/Vulkan_Engine/*.cpp"
    "src/Vulkan_Engine/instance/*.cpp"
    "src/Vulkan_Engine/window/*.cpp"
    "src/Vulkan_Engine/debug/*.cpp"
    "src/Vulkan_Engine/device/*.cpp"
    "src/Vulkan_Engine/swapchain/*.cpp"
    "src/Vulkan_Engine/queue/*.cpp"
    "src/Vulkan_Engine/buffers/*.cpp"
    "src/Vulkan_Engine/depth/*.cpp"
    "src/Vulkan_Engine/image/*.cpp"
    "src/Vulkan_Engine/pipeline/*.cpp"
)
file(GLOB HEADERS 
    "src/*.hpp"
    "src/Vulkan_Engine/*.hpp"
    "src/Vulkan_Engine/instance/*.hpp"
    "src/Vulkan_Engine/window/*.hpp"
    "src/Vulkan_Engine/debug/*.hpp"
    "src/Vulkan_Engine/device/*.hpp"
    "src/Vulkan_Engine/swapchain/*.hpp"
    "src/Vulkan_Engine/queue/*.hpp"
    "src/Vulkan_Engine/buffers/*.hpp"
    "src/Vulkan_Engine/depth/*.hpp"
    "src/Vulkan_Engine/image/*.hpp"
    "src/Vulkan_Engine/pipeline/*.hpp"
)

# Bibliotecas do ImGui
add_library(imgui
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
)

target_include_directories(
    imgui PUBLIC ${IMGUI_DIR}
    )
    

add_library(imgui_backend
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

target_include_directories(imgui_backend PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Executável principal
add_executable(nial ${SOURCES} ${HEADERS})

# Include paths (além do Vulkan)
target_include_directories(nial PRIVATE ${Vulkan_INCLUDE_DIRS})

# Linkar tudo
target_link_libraries(nial
    PRIVATE
        Vulkan::Vulkan
        glfw
        assimp
        imgui
        imgui_backend
)
