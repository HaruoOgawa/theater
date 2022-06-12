#pragma once
#include <vector>

class glTFMesh
{
public:
	glTFMesh();
	~glTFMesh();
	std::vector<float> vertex;
	std::vector<unsigned char> subDataVertex;

	std::vector<float> normal;
	std::vector<unsigned char> subDataNormal;

	std::vector<float> texcoord;
	std::vector<unsigned char> subDataTexcoord;

	std::vector<float> weights;
	std::vector<unsigned char> subDataWeights;

	std::vector<int> joints;
	std::vector<unsigned char> subDataJoints;

	std::vector<unsigned int> indices;
	std::vector<unsigned char> subDataIndices;
	std::vector<unsigned short> mainDataIndices;
};

