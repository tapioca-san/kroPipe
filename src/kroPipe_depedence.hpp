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

// audio dependencias
#include <AL/al.h>
#include <AL/alc.h>

// Inclua apenas o header se o cpp for compilado separadamente
#include "include/imageSupport/stb_image.h"

#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"

#include <unordered_map>
#include <sys/wait.h>
#include <filesystem>
#include <stdexcept>
#include <algorithm>
#include <endian.h>
#include <iostream>
#include <optional>
#include <cstring>
#include <fstream>
#include <vector>
#include <limits>
#include <chrono>
#include <memory>
#include <thread>
#include <atomic>
#include <poll.h>
#include <mutex>
#include <pty.h>
#include <array>
#include <pwd.h>
#include <set>
#include <bit>

#endif//KROPIPE_DEPEDENCE_HPP