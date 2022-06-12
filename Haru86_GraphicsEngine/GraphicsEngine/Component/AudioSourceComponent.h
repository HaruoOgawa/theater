#pragma once
#include "Component.h"
#include "../Object/Object.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <fmod_common.h>

namespace FMOD
{
	class System;
	class Channel;
	class Event;
	class Sound;
	namespace Studio
	{
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	};
};


class AudioSourceComponent :
    public Component
{
public:
    AudioSourceComponent(class Object* o);
    AudioSourceComponent(class Object* o, std::string audioSourcePath);
    ~AudioSourceComponent();
    bool Initialize();
	void Play();
	void Update() override;
	void ProcessInput(const SDL_Event& e) override;
	int GetAudioLength();
	void SkipAudio(float startTime);
	bool StopAudioCheck(float startTime);
private :
    void Shutdown();
	void LoadBank(std::string fileName);
	void UnLoadBask(std::string fileName);
	void UnLoadAllBank();
	bool CreateSound(std::string name);
	void UnLoadSound();
	static FMOD_RESULT __stdcall EndOfSong(FMOD_CHANNELCONTROL* channelControl, FMOD_CHANNELCONTROL_TYPE controlType,
		FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType, void* commanData1, void* commanData2);
	FMOD::Studio::System* mSystem;
	FMOD::System* lowLevelSystem;
	std::unordered_map<std::string, FMOD::Studio::Bank*> banksMap;
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> eventMap;
	FMOD::Channel* channel;
	std::string m_audioSourcePath;
	FMOD::Sound* sound;
};

