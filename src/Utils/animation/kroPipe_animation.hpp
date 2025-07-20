#ifndef KROPIPE_ANIMATION_H
#define KROPIPE_ANIMATION_H

#include "../../kroPipe_depedence.hpp"


namespace KP {
namespace UTILS {

struct BoneTransformTrack {
	std::vector<float> positionTimestamps = {};
	std::vector<float> rotationTimestamps = {};
	std::vector<float> scaleTimestamps = {};

	std::vector<glm::vec3> positions = {};
	std::vector<glm::quat> rotations = {};
	std::vector<glm::vec3> scales = {};
};

struct Animation {
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

class Mesh{

    private:



    public:


        bool readSkeleton(Bone& boneOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfoTable);

        void loadAnimation();
        
        void getAnimation();

};


} // namespace UTILS
} // namespace KP

#endif//KROPIPE_ANIMATION_H