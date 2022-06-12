#pragma once
#include <string>
#include <SDL.h>
#include <GL/glew.h>

class Texture
{
public:
	Texture();
	~Texture();
	void CreateForRendering(int width, int height,GLint internalformat,GLint format,GLenum type= GL_UNSIGNED_BYTE);
	void CreateTrueTypeFontTexture(SDL_Surface* surf);
	bool Load(const std::string& fileName);
	unsigned int GetTextureID() const { return mTextureID; }
	void SetActive();
	void SetEactive();
	void Unload();
	friend class Material;
private:
	unsigned int mTextureID;
	int mWidth;
	int mHeight;
	std::string texUniformName;
};

