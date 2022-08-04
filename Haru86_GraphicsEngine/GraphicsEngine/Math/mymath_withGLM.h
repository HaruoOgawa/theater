#pragma once
#include <glm/glm.hpp>
#include <vector>

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
	
	static glm::vec2 hemCircleRand(float maxRadius, glm::vec2 radSeed, glm::vec2 angSeed)
	{
		float r = rand(radSeed) * maxRadius;
		float a = rand(angSeed) * 3.14159265f * 2.0f;
		
		return glm::vec2(-1.0f*glm::abs(r * glm::cos(a)), r * glm::sin(a));
	}

	static std::vector<float> CastVec3ToLine_float(const std::vector<glm::vec3>& BlockVector) {
		std::vector<float> result;
		for (const auto& block : BlockVector) {
			result.push_back(block.x);
			result.push_back(block.y);
			result.push_back(block.z);
		}

		return result;
	}
}