#include "ComputeShader.h"
#include <glew.h>

#include <fstream>
#include <sstream>
#include "GraphicsEngine/Graphics/ComputeBuffer.h"
#include "GraphicsEngine/Graphics/Material.h"

ComputeShader::ComputeShader(std::string shaderCode) {
	LoadComputeShader(shaderCode);
}

ComputeShader::~ComputeShader() {
	m_buffers.clear();
	DeleteShader();
}

void ComputeShader::SetBuffer(std::shared_ptr<ComputeBuffer> buffer, int bufferindex, std::shared_ptr<Material> material) {
	m_buffers.push_back(buffer);
	SetActive();
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bufferindex, buffer->ssbo);
}


void ComputeShader::SetMatrixUniform(std::string uniformName, glm::mat4 matrix) {
	GLuint location = glGetUniformLocation(shaderPrg, uniformName.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, reinterpret_cast<const float*>(&matrix[0][0]));
}

void ComputeShader::SetFloatUniform(std::string uniformName, float val) {
	GLuint location = glGetUniformLocation(shaderPrg, uniformName.c_str());
	glUniform1f(location, val);
}

void ComputeShader::SetIntUniform(std::string uniformName, int val) {
	GLuint location = glGetUniformLocation(shaderPrg, uniformName.c_str());
	glUniform1i(location, val);
}

void ComputeShader::SetVec2Uniform(std::string uniformName, glm::vec2 val) {
	GLuint location = glGetUniformLocation(shaderPrg, uniformName.c_str());
	glUniform2fv(location, 1, reinterpret_cast<GLfloat*>(&val));
}

void ComputeShader::SetVec3Uniform(std::string uniformName, glm::vec3 val) {
	GLuint location = glGetUniformLocation(shaderPrg, uniformName.c_str());
	glUniform3fv(location, 1, reinterpret_cast<GLfloat*>(&val));
}

void ComputeShader::SetTexUniform(std::string uniformName, unsigned int val) {
	GLuint location = glGetUniformLocation(shaderPrg, uniformName.c_str());
	glUniform1i(location, static_cast<GLuint>(val - 1));
}

void ComputeShader::Dispatch(int xGroupNum, int yGroupNum, int zGroupNum) {
	glDispatchCompute(xGroupNum, yGroupNum, zGroupNum);
}


void ComputeShader::LoadComputeShader(std::string computeShaderPath) {
	if (CompileShader(computeShaderPath, computeShaderData)) {

		shaderPrg = glCreateProgram();
		glAttachShader(shaderPrg, computeShaderData);
		glLinkProgram(shaderPrg);

		GLint status;
		glGetProgramiv(shaderPrg, GL_LINK_STATUS, &status);
		if (status != GL_TRUE) {
			//SDL_Log("Cannot Load Program Data(ComputeShader)");
			char buffer[512];
			memset(buffer, 0, 512);
			glGetProgramInfoLog(shaderPrg, 511, nullptr, buffer);
			//SDL_Log("GLSL Link Status(ComputeShader):\n%s", buffer);
		}
	}
}

bool ComputeShader::CompileShader(std::string shadername, GLuint& outShader) {

	std::ifstream shaderFile(shadername);
	if (shaderFile.is_open()) {

		std::stringstream sstream;
		sstream << shaderFile.rdbuf();
		std::string contents = sstream.str();
		const char* contentsChar = contents.c_str();

		outShader = glCreateShader(GL_COMPUTE_SHADER);
		glShaderSource(outShader, 1, &(contentsChar), nullptr);
		glCompileShader(outShader);
	}
	else {
		//SDL_Log("Failure!! cannot open shader file...");
	}

	GLint status;
	glGetShaderiv(outShader, GL_COMPILE_STATUS, &status);
	if (status != GL_TRUE) {
		//SDL_Log("Cannnot Load Shader: %s", shadername.c_str());

		char buffer[512];
		memset(buffer, 0, 512);
		glGetShaderInfoLog(outShader, 511, nullptr, buffer);
		//SDL_Log("GLSL Compile Failed: %s", buffer);
		return false;
	}

	return true;
}

void ComputeShader::BindComputeBuffer(CorrectionType correctionType) {
	if (correctionType == CorrectionType::COMPUTEBUFFER) {
		for (int i = 0; i < m_buffers.size(); i++) {
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i, m_buffers[i]->ssbo);
		}
	}
	else if(correctionType == CorrectionType::ARRAY) {
		for (int i = 0; i < m_buffers.size(); i++) {
			glBindBuffer(GL_ARRAY_BUFFER, m_buffers[i]->ssbo);
		}
	}
}

void ComputeShader::DisBindComputeBuffer(CorrectionType correctionType) {
	if (correctionType == CorrectionType::COMPUTEBUFFER) {
		for (int i = 0; i < m_buffers.size(); i++) {
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i,0);
		}
	}
	else if(correctionType == CorrectionType::ARRAY) {
		for (int i = 0; i < m_buffers.size(); i++) {
			glBindBuffer(GL_ARRAY_BUFFER,0);
		}
	}
}

void ComputeShader::SetActive() {
	glUseProgram(shaderPrg);
}

void ComputeShader::DeleteShader() {
	glUseProgram(shaderPrg);
	glDeleteProgram(shaderPrg);
}
