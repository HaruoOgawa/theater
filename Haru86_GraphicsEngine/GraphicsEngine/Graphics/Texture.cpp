#include "Texture.h"
#include <glew.h>
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"

Texture::Texture()
	:mTextureID(-1)
	, mWidth(0)
	, mHeight(0)
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &mTextureID);
}

bool Texture::Load(const std::string& fileName)
{
	return true;
}

void Texture::CreateForRendering(int width, int height, GLint internalformat, GLint format, GLenum type)
{
	mWidth = width;
	mHeight = height;
	
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, mWidth, mHeight, 0, format,type, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::CreateForCubemap(int width, int height, GLint internalformat, GLint format, GLenum type)
{
	mWidth = width;
	mHeight = height;
	
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mTextureID);
	
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internalformat, mWidth, mHeight, 0, format,type, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internalformat, mWidth, mHeight, 0, format,type, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internalformat, mWidth, mHeight, 0, format,type, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internalformat, mWidth, mHeight, 0, format,type, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internalformat, mWidth, mHeight, 0, format,type, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internalformat, mWidth, mHeight, 0, format,type, 0);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Texture::SetActive(GLenum slot, GLenum texType)
{
	glActiveTexture(slot);
	glBindTexture(texType, mTextureID);
}

void Texture::SetEnactive(GLenum slot, GLenum texType) {
	glActiveTexture(slot);
	glBindTexture(texType, 0);
}