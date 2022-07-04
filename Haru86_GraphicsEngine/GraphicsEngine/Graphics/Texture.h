#pragma once
#include <string>

#include <glew.h>

class Texture
{
public:
	Texture();
	~Texture();
	void CreateForRendering(int width, int height,GLint internalformat,GLint format,GLenum type= GL_UNSIGNED_BYTE);
	void CreateForCubemap(int width, int height,GLint internalformat,GLint format,GLenum type= GL_UNSIGNED_BYTE);
	bool Load(const std::string& fileName);
	unsigned int GetTextureID() const { return mTextureID; }
	int GetWidth()const { return mWidth; }
	int GetHeight()const { return mHeight; }
	void SetActive(GLenum slot,GLenum texType=GL_TEXTURE_2D);
	void SetEnactive(GLenum slot, GLenum texType = GL_TEXTURE_2D);
	friend class Material;
private:
	unsigned int mTextureID;
	int mWidth;
	int mHeight;
	std::string texUniformName;
};

