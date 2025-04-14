#ifndef DEPENDENCE_H
#define DEPENDENCE_H

    #include <vulkan/vulkan_core.h>
    #include <vulkan/vulkan.h>
    #define GLFW_INCLUDE_VULKAN
    #include <GLFW/glfw3.h>

    #define GLM_FORCE_RADIANS
    #define GLM_FORCE_DEPTH_ZERO_TO_ONE
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>

    #include "include/imgui-master/backends/imgui_impl_vulkan.h"
    #include "include/imgui-master/backends/imgui_impl_glfw.h"
    #include "include/imgui-master/imgui.h"

    #include "include/imageSupport/stb_image.h"

    #include "assimp/scene.h"
    #include "assimp/postprocess.h"
    #include "assimp/Importer.hpp"

    #include <filesystem>
    #include <iostream>
    #include <stdexcept>
    #include <vector>
    #include <cstring>
    #include <cstdlib>
    #include <optional>
    #include <set>
    #include <limits>
    #include <algorithm>
    #include <fstream>
    #include <array>
    #include <chrono>
    #include <memory>

#endif//DEPENDENCE_H