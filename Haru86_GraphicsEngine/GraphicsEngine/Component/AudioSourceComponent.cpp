#include "AudioSourceComponent.h"
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <vector>
#include "./SDL.h"
#include <iostream>
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"

AudioSourceComponent::AudioSourceComponent(Object* o)
	: Component(o)
{
	m_audioSourcePath = "./Assets/Resources/MusicData/lug00ber_they_are_feeding.mp3";
}

AudioSourceComponent::AudioSourceComponent(Object* o, std::string audioSourcePath)
	: Component(o)
{
	m_audioSourcePath = audioSourcePath;
}

AudioSourceComponent::~AudioSourceComponent() {
	Shutdown();
}

bool AudioSourceComponent::Initialize() {
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR,
		FMOD_DEBUG_MODE_TTY
	);

	FMOD_RESULT result;
	result = FMOD::Studio::System::create(&mSystem);
	if (result != FMOD_OK) {
		//SDL_Log("FMOD‚Ìì¬‚ÉŽ¸”s‚µ‚Ü‚µ‚½: %s",FMOD_ErrorString(result));
		return false;
	}
	
	result = mSystem->initialize(
		512,
		FMOD_STUDIO_INIT_NORMAL,
		FMOD_INIT_NORMAL,
		nullptr
	);

	if (result!=FMOD_OK) {
		//SDL_Log("FMOD‰Šú‰»‚ÉŽ¸”s‚µ‚Ü‚µ‚½: %s", FMOD_ErrorString(result));
		return false;
	}

	mSystem->getLowLevelSystem(&lowLevelSystem);

	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	if (!CreateSound(m_audioSourcePath)) {
		return false;
	}

	return true;
}

void AudioSourceComponent::LoadBank(std::string fileName) {
	if (banksMap.find(fileName) != banksMap.end()) {
		return;
	}

	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = mSystem->loadBankFile(
		fileName.c_str(),
		FMOD_STUDIO_LOAD_BANK_NORMAL,
		&bank
	);

	const int maxPathLength = 512;

	if (result==FMOD_OK) {
		banksMap.emplace(fileName, bank);
		bank->loadSampleData();
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0) {
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);

			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++) {
				FMOD::Studio::EventDescription* e = events[i];
				e->getPath(eventName, maxPathLength, nullptr);
				eventMap.emplace(eventName, e);
			}
		}
	}
}
void AudioSourceComponent::UnLoadBask(std::string fileName) {
	auto item = banksMap.find(fileName);
	if (item == banksMap.end()) {
		return;
	}

	FMOD::Studio::Bank* bank = item->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);
	if (numEvents > 0) {
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];
		for (int i = 0; i < numEvents; i++) {
			FMOD::Studio::EventDescription* e = events[i];
			e->getPath(eventName, 512, nullptr);
			auto eventi = eventMap.find(eventName);
			if (eventi != eventMap.end()) {
				eventMap.erase(eventi);
			}
		}
	}

	bank->unloadSampleData();
	bank->unload();
	banksMap.erase(item);
}

void AudioSourceComponent::UnLoadAllBank() {
	for (auto& item : eventMap) {
		item.second->unloadSampleData();
	}
	
	for (auto& item : banksMap) {
		item.second->unloadSampleData();
		item.second->unload();
	}

	banksMap.clear();
	eventMap.clear();
}

bool AudioSourceComponent::CreateSound(std::string name) {
	FMOD_RESULT result;
	result = lowLevelSystem->createStream(name.c_str(), FMOD_DEFAULT, 0, &sound);
	
	if (result == FMOD_OK) {
		//channel->setVolume(0.1f);
		return true;
	}
	else {
		char a[] = "a";
		//SDL_Log("Cannnot Load Sound: (%d) %s",result,FMOD_ErrorString(result));
		delete sound;
		return false;
	}
}

void AudioSourceComponent::Play() {
	FMOD_RESULT result = lowLevelSystem->playSound(sound, nullptr, false, &channel);
	if (result != FMOD_OK) {
		//SDL_Log("Cannnot Play Sound: (%d) %s", result, FMOD_ErrorString(result));
		delete sound;
		delete channel;
	}
	else {
		channel->setCallback(AudioSourceComponent::EndOfSong);
	}
}

void AudioSourceComponent::Update() {
	
}

int AudioSourceComponent::GetAudioLength() {
	unsigned int len = 0;
	sound->getLength(&len, FMOD_TIMEUNIT_MS);

	unsigned int pos;
	channel->getPosition(&pos, FMOD_TIMEUNIT_MS);
	
	return static_cast<int>(len);
}

void AudioSourceComponent::SkipAudio(float startTime) {
	if (channel!=nullptr) {
		if (!StopAudioCheck(startTime)) {
			channel->setPosition(static_cast<unsigned int>(startTime), FMOD_TIMEUNIT_MS);
		}
		else {
			channel->setPosition(static_cast<unsigned int>(GetAudioLength() - 2000), FMOD_TIMEUNIT_MS);
		}
	}
}

bool AudioSourceComponent::StopAudioCheck(float startTime) {
	if (channel!=nullptr) {
		unsigned int pos;
		channel->getPosition(&pos, FMOD_TIMEUNIT_MS);
		if (startTime >GetAudioLength()-2000) {
			return true;
		}
		else {
			return false;
		}
		
	}
}

FMOD_RESULT F_CALLBACK AudioSourceComponent::EndOfSong(FMOD_CHANNELCONTROL* channelControl, FMOD_CHANNELCONTROL_TYPE controlType,
	FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType, void* commanData1, void* commanData2)
{
	if (controlType == FMOD_CHANNELCONTROL_CHANNEL && callbackType == FMOD_CHANNELCONTROL_CALLBACK_END) {
		GraphicsMain::GetInstance()->Restart();
	}

	return FMOD_OK;
}

void AudioSourceComponent::UnLoadSound() {
	if (sound != nullptr) {
		delete sound;
	}

	if (channel!=nullptr) {
		delete channel;
	}
}

void AudioSourceComponent::Shutdown() {
	UnLoadAllBank();
	mSystem->release();
}

void AudioSourceComponent::ProcessInput(const SDL_Event& e) {

}
