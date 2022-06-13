#pragma once
#include "./Texture.h"
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <memory>

class Font
{
public:
	Font();
	~Font();
	bool Load(std::string fontPass);
	void UnLoad();
private:
};

