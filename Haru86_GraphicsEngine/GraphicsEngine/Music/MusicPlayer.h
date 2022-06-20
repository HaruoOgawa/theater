#pragma once
class MusicPlayer
{
public:
	MusicPlayer();
	~MusicPlayer();

	bool Initialize();
	bool Update();
private:
	static void PlayMusic();
};

