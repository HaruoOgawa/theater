#include "SoundPlayer.h"
#include <Windows.h>
#include <mmsystem.h>
#include <array>
#pragma comment(lib, "Winmm.lib")

namespace sound 
{
	SoundPlayer::SoundPlayer() {
		Initialize();
	}

	SoundPlayer::~SoundPlayer() {
		Release();
	}

	bool SoundPlayer::Initialize() {
		std::array<char, MAXERRORLENGTH> errorString;
		mciGetErrorStringA(
			mciSendStringA(
				"open \" E:\\CppDev\\theater\\Haru86_GraphicsEngine\\Assets\\Sound\\stroke.mp3\" type mpegvideo alias mp3",
				nullptr,
				0,
				nullptr),
			errorString.data(),
			MAXERRORLENGTH);
		//std::printf("%s\n", errorString.data());

		return true;
	}

	bool SoundPlayer::Play() 
	{
		std::array<char, MAXERRORLENGTH> errorString;
		mciGetErrorStringA(
			mciSendStringA("play mp3", nullptr, 0, nullptr),
			errorString.data(),
			MAXERRORLENGTH);
		//std::printf("%s\n", errorString.data());

		return true;
	}

	bool SoundPlayer::Pause() {
		mciSendStringA("pause mp3", NULL, 0, NULL);

		return true;
	}

	void SoundPlayer::Release() {
		mciSendStringA("close mp3", NULL, 0, NULL);
	}
}
