#include "ShaderLib.h"
#include <iostream>
namespace shaderlib {
	const char* ShaderLib::Standard_vert = {
		#include "GraphicsEngine/App/BuiltinShader/standard.vert"
	};
	
	const char* ShaderLib::Standard_frag = {
		#include "GraphicsEngine/App/BuiltinShader/standard.frag"
	};

}