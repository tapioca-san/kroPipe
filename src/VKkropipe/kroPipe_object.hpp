#ifndef OBJECT_H
#define OBJECT_H

#include "../kroPipe_depedence.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cmath>

namespace KP {
namespace OBJECT {


    struct ObjectData {
        glm::vec3 Position{};
        glm::vec3 actualPositionForUseForLastPosition{};
        glm::vec3 lastPosition{};
        glm::vec3 velocity{};

        float raio = 0.0f;
        float width = 0.0f, height = 0.0f, depth = 0.0f;
        float* vertices = nullptr;
        size_t numVertices = 0;

        float floorPos = 0.0f;
        float floorPoslowest = 0.0f;

        bool is_touchingX = false;
        bool is_touchingY = false;
        bool is_touchingZ = false;
        bool is_onCollition = false;
        bool is_onObject = false;
        bool is_myself = false;
        bool is_OnAir = true;

        int ID = -1;
    };

    class Object {
    private:
        float calculateRaio(const float* vertices, size_t numVertices) {
            if (!vertices || numVertices == 0) return 0.0f;

            float cx = 0.0f, cy = 0.0f, cz = 0.0f;

            for (size_t i = 0; i < numVertices * 3; i += 3) {
                cx += vertices[i];
                cy += vertices[i + 1];
                cz += vertices[i + 2];
            }
            cx /= numVertices;
            cy /= numVertices;
            cz /= numVertices;

            float raio = 0.0f;
            for (size_t i = 0; i < numVertices * 3; i += 3) {
                float dx = vertices[i] - cx;
                float dy = vertices[i + 1] - cy;
                float dz = vertices[i + 2] - cz;
                float distancia = std::sqrt(dx * dx + dy * dy + dz * dz);
                raio = std::max(raio, distancia);
            }

            return raio;
        }

    public:
        ObjectData data;

        Object(glm::vec3 position, float floorPos, bool is_myself, int& lastID, float* vertices = nullptr, size_t vertexCount = 0) {
            data.Position = position;
            data.floorPos = floorPos;
            data.floorPoslowest = floorPos;
            data.is_myself = is_myself;
            data.velocity = glm::vec3(0.0f);
            data.ID = lastID++;
            data.vertices = vertices;
            data.numVertices = vertexCount;

            if (vertices && vertexCount > 0) {
                data.raio = calculateRaio(vertices, vertexCount);
                //calculateAABB(data, vertices, vertexCount);
            }
        }

        ~Object() {
        }
    };

    void calculateAABB(ObjectData& obj, const float* vertices, size_t numVertices) {
        try {
            if (!vertices || numVertices == 0) {
                throw std::invalid_argument("Os dados de vértices são inválidos.");
            }

            float minX = FLT_MAX, maxX = -FLT_MAX;
            float minY = FLT_MAX, maxY = -FLT_MAX;
            float minZ = FLT_MAX, maxZ = -FLT_MAX;

            for (size_t i = 0; i < numVertices * 8; i += 8) {
                float x = vertices[i];
                float y = vertices[i + 1];
                float z = vertices[i + 2];

                minX = std::min(minX, x);
                maxX = std::max(maxX, x);
                minY = std::min(minY, y);
                maxY = std::max(maxY, y);
                minZ = std::min(minZ, z);
                maxZ = std::max(maxZ, z);
            }

            obj.width = maxX - minX;
            obj.height = maxY - minY;
            obj.depth = maxZ - minZ;
        } catch (const std::exception& e) {
            std::cerr << "Erro no cálculo do AABB: " << e.what() << std::endl;
        }
    }

}
} // namespace kroPipe

#endif // OBJECT_H
