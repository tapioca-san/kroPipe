#ifndef OBJECT_H
#define OBJECT_H

#include "../kroPipe_depedence.hpp"


namespace KP {
namespace OBJECT {

    struct Object {

        private:

        float calculateRaio(const float* vertices, size_t numVertices) {
            float cx = 0.0f, cy = 0.0f, cz = 0.0f;

            for(size_t i = 0; i < numVertices * 3; i += 3) {
                cx += vertices[i];     // x
                cy += vertices[i + 1]; // y
                cz += vertices[i + 2]; // z
            }
            cx /= numVertices;
            cy /= numVertices;
            cz /= numVertices;

            float raio = 0.0f;
            for (size_t i = 0; i < numVertices * 3; i += 3) {
                float distancia = std::sqrt(
                    std::pow(vertices[i] - cx, 2) +        // x
                    std::pow(vertices[i + 1] - cy, 2) +    // y
                    std::pow(vertices[i + 2] - cz, 2)      // z
                );
                raio = std::max(raio, distancia);
            }

            return raio;
        }

        public:
        glm::vec3 Position;
        glm::vec3 actualPositionForUseForLastPosition; 
        glm::vec3 lastPosition;     
        glm::vec3 velocity;
        float raio;     
        float width, height, depth; 
        float* vertices;
        size_t numVertices =  numVertices = sizeof(vertices) / sizeof(vertices[0]) / 8;;
        float floorPos;
        float floorPoslowest;

        bool is_touchingX = false;
        bool is_touchingY = false;
        bool is_touchingZ = false;
        
        bool is_onCollition = false;
        bool is_onObject = false;
        bool is_myself = false;
        bool is_OnAir = true;

        int ID;

        Object(glm::vec3 Position, float floorPos, bool is_myself, int &lastID, float* vertices = {}, size_t vertexCount = {})
            : Position(Position), is_myself(is_myself), velocity(0.0f, 0.0f, 0.0f), ID(lastID++), floorPos(floorPos), floorPoslowest(floorPos), raio(0), width(0), height(0), depth(0), vertices(vertices) {
            
            lastID++;

            for (size_t i = 0; i < vertexCount; ++i) {
                this->vertices[i] = vertices[i];
                this->raio = calculateRaio(vertices, sizeof(vertices) * 3);
            }
        }

        ~Object() {
            delete[] vertices;
        }
    };

 void calculateAABB(Object& obj, const float* vertices, size_t numVertices) {
        
        
        try{
            if (vertices == nullptr || numVertices == 0) {
                throw std::invalid_argument("Os dados de vértices são inválidos.");
            }

            float minX = 9999.0f;
            float maxX = -9999.0f;
            float minY = 9999.0f;
            float maxY = -9999.0f;
            float minZ = 9999.0f;
            float maxZ = -9999.0f;

            for (size_t i = 0; i < numVertices * 8; i += 8) {  
                if (i + 2 >= numVertices * 8) {
                    throw std::out_of_range("Índice de posição fora do alcance dos vértices.");
                }

                // Posições
                float x = vertices[i];
                float y = vertices[i + 1];
                float z = vertices[i + 2];

                if(x < minX) minX = x;
                if(x > maxX) maxX = x;
                if(y < minY) minY = y;
                if(y > maxY) maxY = y;
                if(z < minZ) minZ = z;
                if(z > maxZ) maxZ = z;
            }

            obj.width = maxX - minX ;
            obj.height = maxY - minY;
            obj.depth = maxZ - minZ;

            
        } catch (const std::invalid_argument& e) {
            std::cerr << "Erro: " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Erro: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erro desconhecido: " << e.what() << std::endl;
        }
    }
   
    
}//OBJECT
}//KP



#endif//OBJECT_H