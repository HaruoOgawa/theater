#pragma once
#include <string>

#include <glew.h>

class Texture
{
public:
	Texture();
	~Texture();
	void CreateForRendering(int width, int height,GLint internalformat,GLint format,GLenum type= GL_UNSIGNED_BYTE);
	bool Load(const std::string& fileName);
	unsigned int GetTextureID() const { return mTextureID; }
	void SetActive(GLenum slot);
	void SetEnactive(GLenum slot);
	void Unload();
	friend class Material;
private:
	unsigned int mTextureID;
	int mWidth;
	int mHeight;
	std::string texUniformName;
};

