cmake_minimum_required(VERSION 3.10)
project(nial)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only" FORCE)

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "src/*.hpp")

add_executable(nial ${SOURCES} ${HEADERS})

target_include_directories(nial PRIVATE ${Vulkan_INCLUDE_DIRS})

target_link_libraries(nial PRIVATE Vulkan::Vulkan glfw assimp)
