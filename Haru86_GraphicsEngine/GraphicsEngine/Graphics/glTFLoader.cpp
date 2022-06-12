#include "glTFLoader.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Object/Object.h"

namespace gltf {
	std::shared_ptr<glTFData> glTFLoader::Load(Object* obj, std::string gltfDataPath) {

		std::shared_ptr<glTFData> data = std::make_shared<glTFData>();
		if (!data->Load(obj,gltfDataPath))return nullptr;

		return data;
	}
}