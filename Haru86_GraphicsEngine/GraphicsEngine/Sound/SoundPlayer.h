#pragma once

namespace sound 
{
	class SoundPlayer
	{
	public:
		SoundPlayer()=default;
		virtual ~SoundPlayer();
		bool Play();
	};
}