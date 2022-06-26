#pragma once

namespace sound {
	class SoundShaderPlayer
	{
	public:
		SoundShaderPlayer();
		virtual ~SoundShaderPlayer() = default;

		bool Initialize();
		bool Update();
	private:
		void DrawSound();
	};
}

