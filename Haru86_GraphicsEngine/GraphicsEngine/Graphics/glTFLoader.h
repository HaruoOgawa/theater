#pragma once
#include <string>
#include <memory>
#include "glTFData.h"

class Object;
class Mesh;

namespace gltf {
	static class glTFLoader
	{
	public:
		static std::shared_ptr<glTFData> Load(Object* obj,std::string gltfDataPath);
	};
}

