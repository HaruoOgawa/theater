#pragma once
#include <string>
#include <memory>
#include <vector>
#include <glew.h>

class Mesh;
class Material;
class Texture;

namespace sound {
	class SoundShaderPlayer
	{
		// MVPMatrix‚Í•s—v‚È‚Ì‚ÅTransform‚ÆMeshRenderer‚Í—pˆÓ‚µ‚È‚¢
		std::shared_ptr<Mesh>     m_Mesh;
		std::shared_ptr<Material> m_Material;

		std::shared_ptr<Texture>  m_FrameTex;
		unsigned int		      m_FrameIndex;

		std::vector<GLfloat>		  m_SoundData;
		
	public:
		SoundShaderPlayer(const std::string& soundCode);
		virtual ~SoundShaderPlayer() = default;

		bool Initialize();
		bool Update();
	private:
		void DrawSound();
		void ReadSoundBuffer();
	};
}

