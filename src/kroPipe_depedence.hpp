#ifndef KROPIPE_DEPEDENCE_HPP
#define KROPIPE_DEPEDENCE_HPP

#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

// dependencias de imgui
#include "include/imgui-master/backends/imgui_impl_vulkan.h"
#include "include/imgui-master/backends/imgui_impl_glfw.h"
#include "include/imgui-master/imgui.h"

// Inclua apenas o header se o cpp for compilado separadamente
#include "include/imageSupport/stb_image.h"

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

#include <unordered_map>
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
#include <pty.h>
#include <sys/wait.h>
#include <poll.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <pwd.h>

#endif//KROPIPE_DEPEDENCE_HPP