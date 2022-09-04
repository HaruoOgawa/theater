#include "SoundPlayer.h"
#include <Windows.h>
#include <mmsystem.h>
#include <array>
#include "GraphicsEngine/Message/Console.h"
#include <sstream>
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
		/*std::array<char, MAXERRORLENGTH> errorString;
		mciGetErrorStringA(
			mciSendStringA("play mp3", nullptr, 0, nullptr),
			errorString.data(),
			MAXERRORLENGTH);*/
		//std::printf("%s\n", errorString.data());

		return true;
	}

	bool SoundPlayer::Pause() {
		mciSendStringA("pause mp3", NULL, 0, NULL);

		return true;
	}

	void SoundPlayer::Skip(float SkipOffset)
	{
		//
		float Offset = (SkipOffset * 1000.0f); // ミリ秒に直す

		std::ostringstream ss;
		ss << Offset; 
		std::string SkipOffset_str(ss.str());
		std::string cmd = "seek mp3 to " + SkipOffset_str;
		
		std::array<char, MAXERRORLENGTH> errorString;
		mciGetErrorStringA(
			mciSendStringA(cmd.c_str(), NULL, 0, NULL),
			errorString.data(),
			MAXERRORLENGTH);

		// スキップした後は、もう一度Playしないと音が鳴らない
		Play();

		//Console::Log("cmd: %s\n", cmd.c_str());
		//std::printf("%s\n", errorString.data());
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
