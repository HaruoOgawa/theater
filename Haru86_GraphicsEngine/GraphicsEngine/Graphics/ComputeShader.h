#pragma once
#include <string>
#include <glew.h>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

enum CorrectionType {
	COMPUTEBUFFER,
	ARRAY
};

class ComputeShader
{
public:
	ComputeShader(std::string shaderPath);
	~ComputeShader();
	void SetBuffer(std::shared_ptr<class ComputeBuffer> buffer,int bufferindex,std::shared_ptr<class Material> material);
	void SetMatrixUniform(std::string uniformName, glm::mat4 matrix);
	void SetFloatUniform(std::string uniformName, float val);
	void SetIntUniform(std::string uniformName, int val);
	void SetVec2Uniform(std::string uniformName, glm::vec2 val);
	void SetVec3Uniform(std::string uniformName, glm::vec3 val);
	void SetTexUniform(std::string uniformName, unsigned int val);
	void Dispatch(int xGroupNum, int yGroupNum, int zGroupNum);
	void LoadComputeShader(std::string computeShaderPath);
	bool CompileShader(std::string shadername, GLuint& outShader);
	void BindComputeBuffer(CorrectionType correctionType);
	void DisBindComputeBuffer(CorrectionType correctionType);
	void SetActive();
	void DeleteShader();
private:
	GLuint shaderPrg;
	GLuint computeShaderData;
	std::vector<std::shared_ptr<ComputeBuffer>> m_buffers;
};

