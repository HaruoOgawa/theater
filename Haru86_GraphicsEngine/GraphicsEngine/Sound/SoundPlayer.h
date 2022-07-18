#pragma once
#include <vector>
#include <string>

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

	private:
		std::string GetExeDir(char path[]);
	};
}