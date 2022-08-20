#include "SoundPlayer.h"
#include <Windows.h>
#include <mmsystem.h>
#include <array>
#include "GraphicsEngine/Message/Console.h"
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
		char ExePath[256];
		GetModuleFileName(NULL, ExePath, 256);
		std::string ExeDir = GetExeDir(ExePath);

		std::string AudioPath ="\"" + ExeDir + "\\" + "stroke_edit.mp3" + "\"";
		std::string cmd = "open " + AudioPath + " type mpegvideo alias mp3";
		
		std::array<char, MAXERRORLENGTH> errorString;
		mciGetErrorStringA(
			mciSendStringA(
				cmd.c_str(),
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

	std::string SoundPlayer::GetExeDir(char path[]) {
		std::string path_str(path);
		int pathBlockOrder=path_str.rfind("\\");
		std::string ExeDir = path_str.erase(pathBlockOrder);
		return ExeDir;
	}
}
