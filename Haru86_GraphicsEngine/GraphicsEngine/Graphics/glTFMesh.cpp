#include "glTFMesh.h"


glTFMesh::glTFMesh() {

}
glTFMesh::~glTFMesh() {
	vertex.clear();
	subDataVertex.clear();
	
	normal.clear();
	subDataNormal.clear();
	
	texcoord.clear();
	subDataTexcoord.clear();

	weights.clear();
	subDataWeights.clear();

	joints.clear();
	subDataJoints.clear();

	indices.clear();
	subDataIndices.clear();
}