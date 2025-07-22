#ifndef KROPIPE_ANIMATION_H
#define KROPIPE_ANIMATION_H

#include "../../Vulkan_Engine/pipeline/kroPipe_vertex_data.hpp"
#include "../../Vulkan_Engine/buffers/kroPipe_buffer.hpp"
#include "../../Vulkan_Engine/device/kroPipe_device.hpp"
#include "../../Vulkan_Engine/debug/kroPipe_debug.hpp"
#include "../../kroPipe_depedence.hpp"

namespace KP {
namespace UTILS {

// Conversão Assimp → GLM
inline glm::mat4 aiMat4ToGlm(const aiMatrix4x4& mat) {
    return glm::mat4(
        mat.a1, mat.b1, mat.c1, mat.d1,
        mat.a2, mat.b2, mat.c2, mat.d2,
        mat.a3, mat.b3, mat.c3, mat.d3,
        mat.a4, mat.b4, mat.c4, mat.d4
    );
}

struct VertexWeight {
    int boneID;
    float weight;
};

struct BoneInfo {
    std::string name;
    int id;
    glm::mat4 offsetMatrix; 
};

class Bone {

private:

VkDeviceMemory uniformBufferMemory;
void* uniformBufferMapped;  


VkBuffer uniformBuffer;

public:
    std::vector<std::vector<VertexWeight>> vertexWeightsPerVertex;

    std::unordered_map<std::string, BoneInfo> boneMap;
    std::vector<glm::mat4> boneMatrices;

    int boneCount = 0;

    void ExtractBonesFromMesh(aiMesh* mesh) {
        vertexWeightsPerVertex.resize(mesh->mNumVertices);

        for (unsigned int i = 0; i < mesh->mNumBones; ++i) {
            aiBone* bone = mesh->mBones[i];
            std::string boneName = bone->mName.C_Str();

            int boneID;
            if (boneMap.find(boneName) == boneMap.end()) {
                BoneInfo info;
                info.name = boneName;
                info.id = boneCount;
                info.offsetMatrix = aiMat4ToGlm(bone->mOffsetMatrix);
                boneMap[boneName] = info;

                boneID = boneCount;
                boneCount++;
            } else {
                boneID = boneMap[boneName].id;
            }

            for (unsigned int j = 0; j < bone->mNumWeights; ++j) {
                unsigned int vertexID = bone->mWeights[j].mVertexId;
                float weight = bone->mWeights[j].mWeight;

                vertexWeightsPerVertex[vertexID].push_back({ boneID, weight });
            }
        }

        boneMatrices.resize(boneCount, glm::mat4(1.0f));
    }

    void ProcessBones(const aiNode* node, const glm::mat4& parentTransform) {
        std::string nodeName = node->mName.C_Str();
        glm::mat4 nodeTransform = aiMat4ToGlm(node->mTransformation);

        glm::mat4 globalTransform = parentTransform * nodeTransform;

        auto it = boneMap.find(nodeName);
        if (it != boneMap.end()) {
            int boneIndex = it->second.id;
            glm::mat4 offsetMatrix = it->second.offsetMatrix;

            boneMatrices[boneIndex] = globalTransform * offsetMatrix;
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            ProcessBones(node->mChildren[i], globalTransform);
        }
    }

    const std::vector<glm::mat4>& GetFinalBoneMatrices() const {
        return boneMatrices;
    }

    void createUniformBuffers(){
        VkDeviceSize bufferSize = sizeof(std::vector<glm::mat4>);
        KP::ENGINE::createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer, uniformBufferMemory);
        KP::ENGINE::err = vkMapMemory(*KP::ENGINE::OBJECT_device.getPointerDevice(), uniformBufferMemory, 0, bufferSize, 0, &uniformBufferMapped);
        KP::ENGINE::check_vk_result(KP::ENGINE::err);
        
        memcpy(uniformBufferMapped, &GetFinalBoneMatrices(), sizeof(GetFinalBoneMatrices()));
    }

    void updateLightToShaders() {
        memcpy(uniformBufferMapped, &GetFinalBoneMatrices(), sizeof(GetFinalBoneMatrices()));
    }

};

} // namespace UTILS
} // namespace KP

#endif // KROPIPE_ANIMATION_H
