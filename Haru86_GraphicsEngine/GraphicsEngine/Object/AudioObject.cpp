#include "../Object/AudioObject.h"
#include "../Component/AudioSourceComponent.h"
#include <iostream>
#include "GraphicsEngine/Object/CNode.h"


AudioObject::AudioObject() 
	: Object()
{
	audioSource =  std::make_shared<AudioSourceComponent>(this);
	GetRootNode()->AddComponent<AudioSourceComponent>(audioSource);
}

AudioObject::AudioObject(std::string audioSourcePath)
	: Object()
{
	audioSource = std::make_shared<AudioSourceComponent>(this, audioSourcePath);
	GetRootNode()->AddComponent<AudioSourceComponent>(audioSource);
}

AudioObject::~AudioObject() {
}

bool AudioObject::Initialize() {
	if (!audioSource->Initialize()) {
		return false;
	}

	return true;
}

void AudioObject::PlayAudio() {
	audioSource->Play();
}