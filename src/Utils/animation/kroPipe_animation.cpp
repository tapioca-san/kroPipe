
#include "kroPipe_animation.hpp"




namespace KP {
namespace UTILS {

bool Mesh::readSkeleton(Bone& boneOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfoTable) {

	if (boneInfoTable.find(node->mName.C_Str()) != boneInfoTable.end()) { // if node is actually a bone
		boneOutput.name = node->mName.C_Str();
		boneOutput.id = boneInfoTable[boneOutput.name].first;
		boneOutput.offset = boneInfoTable[boneOutput.name].second;

		for (int i = 0; i < node->mNumChildren; i++) {
			Bone child;
			readSkeleton(child, node->mChildren[i], boneInfoTable);
			boneOutput.children.push_back(child);
		}
		return true;
	}
	else { // find bones in children
		for (int i = 0; i < node->mNumChildren; i++) {
			if (readSkeleton(boneOutput, node->mChildren[i], boneInfoTable)) {
				return true;
			}

		}
	}
	return false;
}

void Mesh::loadAnimation(){

	std::unordered_map<std::string, std::pair<int, glm::mat4>> boneInfo = {};
	std::vector<uint> boneCounts;
	boneCounts.resize(verticesOutput.size(), 0);
	 nBoneCount = mesh->mNumBones;
     
    for (uint i = 0; i < nBoneCount; i++) {
		aiBone* bone = mesh->mBones[i];
		glm::mat4 m = assimpToGlmMatrix(bone->mOffsetMatrix);
		boneInfo[bone->mName.C_Str()] = { i, m };

		//loop through each vertex that have that bone
		for (int j = 0; j < bone->mNumWeights; j++) {
			uint id = bone->mWeights[j].mVertexId;
			float weight = bone->mWeights[j].mWeight;
			boneCounts[id]++;
			switch (boneCounts[id]) {
			case 1:
				verticesOutput[id].boneIds.x = i;
				verticesOutput[id].boneWeights.x = weight;
				break;
			case 2:
				verticesOutput[id].boneIds.y = i;
				verticesOutput[id].boneWeights.y = weight;
				break;
			case 3:
				verticesOutput[id].boneIds.z = i;
				verticesOutput[id].boneWeights.z = weight;
				break;
			case 4:
				verticesOutput[id].boneIds.w = i;
				verticesOutput[id].boneWeights.w = weight;
				break;
			default:
				//std::cout << "err: unable to allocate bone to vertex" << std::endl;
				break;

			}
		}
	}

}

} // namespace UTILS
} // namespace KP