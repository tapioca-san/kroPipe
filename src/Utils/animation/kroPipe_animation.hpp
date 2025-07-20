#ifndef KROPIPE_ANIMATION_H
#define KROPIPE_ANIMATION_H

#include "../../Vulkan_Engine/pipeline/kroPipe_vertex_data.hpp"
#include "../../kroPipe_depedence.hpp"

#define MAX_BONE_INFLUENCE 4

namespace KP {
namespace UTILS {

glm::mat4 assimpToGlmMatrix(aiMatrix4x4 mat);

glm::vec3 assimpToGlmVec3(aiVector3D vec);

glm::quat assimpToGlmQuat(aiQuaternion quat);
	
struct BoneTransformTrack {
	std::vector<float> positionTimestamps = {};
	std::vector<float> rotationTimestamps = {};
	std::vector<float> scaleTimestamps = {};

	std::vector<glm::vec3> positions = {};
	std::vector<glm::quat> rotations = {};
	std::vector<glm::vec3> scales = {};
};

struct AnimationData {
	float duration = 0.0f;
	float ticksPerSecond = 1.0f;
	std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {};
};

struct Bone {
	int id = 0; 
	std::string name = "";
	glm::mat4 offset = glm::mat4(1.0f);
	std::vector<Bone> children = {};
};

class Animation{

    private:

	std::unordered_map<std::string, std::pair<int, glm::mat4>> boneInfo = {};
	std::vector<uint> boneCounts;
	std::vector<KP::ENGINE::VertexVulkan> verticesOutput;
	uint32_t numbersBoneCount;

    public:


        bool readSkeleton(Bone& boneOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfoTable);

        void loadAnimation(aiMesh* mesh, std::vector<KP::ENGINE::VertexVulkan> &vertices);
        
        void getAnimation();


};


} // namespace UTILS
} // namespace KP

#endif//KROPIPE_ANIMATION_H