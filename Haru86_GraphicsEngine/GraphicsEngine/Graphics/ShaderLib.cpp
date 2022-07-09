#include "ShaderLib.h"
#include <iostream>
namespace shaderlib { // プリプロセッサでどれをリンクするか選ぶのもありかも？？ => 例えばBloomを使わないのにBloomShaderをリンクするのは無駄
	const std::string ShaderLib::Standard_vert = {
		#include "GraphicsEngine/Graphics/Shader/standard.vert"
	};
	
	const std::string ShaderLib::Standard_frag = {
		#include "GraphicsEngine/Graphics/Shader/standard.frag"
	};
	
	const std::string ShaderLib::DepthColor_frag = {
		#include "GraphicsEngine/Graphics/Shader/DepthColor.frag"
	};
	
	const std::string ShaderLib::NormalMapColor_frag = {
		#include "GraphicsEngine/Graphics/Shader/NormalMapColor.frag"
	};
	
	const std::string ShaderLib::GridPlane_frag = {
		#include "GraphicsEngine/Graphics/Shader/GridPlane.frag"
	};
	
	const std::string ShaderLib::StandardRenderBoard_vert = {
		#include "GraphicsEngine/Graphics/Shader/StandardRenderBoard.vert"
	};
	
	const std::string ShaderLib::StandardRenderBoard_frag = {
		#include "GraphicsEngine/Graphics/Shader/StandardRenderBoard.frag"
	};
	
#ifdef UseBloom

#endif // UseBloom


	const std::string ShaderLib::BloomBlur_frag = {
		#include "GraphicsEngine/Graphics/Shader/BloomBlur.frag"
	};
	
	const std::string ShaderLib::BloomDemo_frag = {
		#include "GraphicsEngine/Graphics/Shader/BloomDemo.frag"
	};
	
	const std::string ShaderLib::BloomIlluminance_frag = {
		#include "GraphicsEngine/Graphics/Shader/BloomIlluminance.frag"
	};
	
	const std::string ShaderLib::BloomResult_frag = {
		#include "GraphicsEngine/Graphics/Shader/BloomResult.frag"
	};
	

#ifdef UseText
	const std::string ShaderLib::FontText_vert = {
		#include "GraphicsEngine/Graphics/Shader/FontText.vert"
	};

	const std::string ShaderLib::FontText_frag = {
		#include "GraphicsEngine/Graphics/Shader/FontText.frag"
	};
#endif // UseText
	const std::string ShaderLib::LatePostProcess_frag = {
		#include "GraphicsEngine/Graphics/Shader/LatePostProcess.frag"
	};
	
	const std::string ShaderLib::PolygonPostProcess_frag = {
		#include "GraphicsEngine/Graphics/Shader/PolygonPostProcess.frag"
	};
	
	const std::string ShaderLib::PolygonRaymarchingMixer_vert = {
		#include "GraphicsEngine/Graphics/Shader/PolygonRaymarchingMixer.vert"
	};
	
	const std::string ShaderLib::PolygonRaymarchingMixer_frag = {
		#include "GraphicsEngine/Graphics/Shader/PolygonRaymarchingMixer.frag"
	};
	
	const std::string ShaderLib::RaymarchingObject_vert = {
		#include "GraphicsEngine/Graphics/Shader/RaymarchingObject.vert"
	};
	
#ifdef DEBUG
	const std::string ShaderLib::UIStandard_vert = {
		#include "GraphicsEngine/Graphics/Shader/UIStandard.vert"
};

	const std::string ShaderLib::UIStandard_frag = {
		#include "GraphicsEngine/Graphics/Shader/UIStandard.frag"
	};
#endif // DEBUG
}