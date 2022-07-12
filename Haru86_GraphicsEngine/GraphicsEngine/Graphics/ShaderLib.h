#pragma once
#include <string>

namespace shaderlib {
	class ShaderLib { // ビルトインシェーダーをShaderLibで定義する(カスタムシェーダはApp::Startから直接渡す)
	public:
		static const std::string Standard_vert;
		static const std::string Standard_frag;
		static const std::string DepthColor_frag;
		static const std::string NormalMapColor_frag;
		static const std::string GridPlane_frag;
		static const std::string StandardRenderBoard_vert;
		static const std::string StandardRenderBoard_frag;
		static const std::string BloomBlur_frag;
		static const std::string BloomDemo_frag;
		static const std::string BloomIlluminance_frag;
		static const std::string BloomResult_frag;
		static const std::string FontText_vert;
		static const std::string FontText_frag;
		static const std::string LatePostProcess_frag;
		static const std::string PolygonPostProcess_frag;
		static const std::string PolygonRaymarchingMixer_vert;
		static const std::string PolygonRaymarchingMixer_frag;
		static const std::string RaymarchingObject_vert;
		static const std::string UIStandard_vert;
		static const std::string UIStandard_frag;
	};
}