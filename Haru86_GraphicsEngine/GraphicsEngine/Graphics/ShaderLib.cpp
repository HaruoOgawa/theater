#include "ShaderLib.h"
#include <iostream>
namespace shaderlib {
	const std::string ShaderLib::Standard_vert = {
		#include "GraphicsEngine/Graphics/Shader/standard.vert"
	};
	
	const std::string ShaderLib::Standard_frag = {
		#include "GraphicsEngine/Graphics/Shader/standard.frag"
	};
	
	const std::string ShaderLib::DepthColor_frag = {
		#include "GraphicsEngine/Graphics/Shader/DepthColor.frag"
	};
	
	const std::string ShaderLib::GridPlane_frag = {
		#include "GraphicsEngine/Graphics/Shader/GridPlane.frag"
	};

}