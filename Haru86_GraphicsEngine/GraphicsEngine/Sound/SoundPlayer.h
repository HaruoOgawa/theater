#pragma once

namespace sound 
{
	class SoundPlayer
	{
	public:
		SoundPlayer();
		virtual ~SoundPlayer();
		bool Initialize();
		bool Play();
		bool Pause();
		void Release();
	};
}