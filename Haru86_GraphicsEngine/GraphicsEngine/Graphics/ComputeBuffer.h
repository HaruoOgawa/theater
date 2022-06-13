#pragma once
#include <vector>
#include <glew.h>
#include <vector>

class ComputeBuffer
{
public:
	ComputeBuffer(GLsizeiptr size);
	~ComputeBuffer();
	template<typename T>
	void SetData(T data) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, data.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	template<typename T>
	void GetBufferData(T *data,int offset,int num) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
		glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(T)*num,data);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
	void DeleteBuffer();

	friend class ComputeShader;
	friend class Material;

private:
	void CreateBuffer(GLsizeiptr size);
	GLuint ssbo;
	GLsizeiptr bufferSize;
};

