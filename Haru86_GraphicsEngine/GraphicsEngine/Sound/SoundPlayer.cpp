#include "SoundPlayer.h"
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

namespace sound 
{
	SoundPlayer::~SoundPlayer() {
		system("pause");
	}

	bool SoundPlayer::Play() 
	{
		if (!PlaySound(TEXT("stroke.wav"), NULL, SND_SYNC))return false;
		return true;
	}
}
