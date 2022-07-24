#pragma once
#include <glm/glm.hpp>

namespace mymath {
	static float rand(glm::vec2 st) 
	{
		return glm::fract(glm::sin(glm::dot(st, glm::vec2(12.9898, 78.233))) * 43758.5453);
	}

	static glm::vec2 circleRand(float maxRadius, glm::vec2 radSeed, glm::vec2 angSeed)
	{
		float r = rand(radSeed) * maxRadius;
		float a = rand(angSeed) * 3.14159265f * 2.0f;
		
		return glm::vec2(r * glm::cos(a), r * glm::sin(a));
	}
}