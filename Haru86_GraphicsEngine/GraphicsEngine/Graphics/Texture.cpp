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
	int channels = 0;

	//unsigned char* image = SOIL_load_image(fileName.c_str(),
	//	&mWidth, &mHeight, &channels, SOIL_LOAD_AUTO);

	/*if (image == nullptr)
	{
		//SDL_Log("SOIL failed to load image %s: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}*/

	/*int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	glGenTextures(1, &mTextureID);
	glActiveTexture(GraphicsMain::GetInstance()->texSlots.find(mTextureID)->second);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format,
		GL_UNSIGNED_BYTE, image);*/

	//SOIL_free_image_data(image);

	// Enable linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

void Texture::SetActive()
{
	glActiveTexture(GraphicsMain::GetInstance()->texSlots.find(mTextureID)->second);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::SetEactive()
{
	glActiveTexture(GraphicsMain::GetInstance()->texSlots.find(mTextureID)->second);
	glDisable(GL_TEXTURE_2D);
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}