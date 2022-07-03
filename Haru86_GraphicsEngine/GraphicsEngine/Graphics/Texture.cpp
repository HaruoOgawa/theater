#include "Texture.h"
#include <glew.h>
//#include <SOIL.h>
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include <map>

Texture::Texture()
	//:mTextureID(0)
	:mTextureID(-1)
	, mWidth(0)
	, mHeight(0)
{

}

Texture::~Texture()
{
	Unload();
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
	
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, mWidth, mHeight, 0, format,
		type, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetActive(GLenum slot)
{
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::SetEnactive(GLenum slot) {
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}