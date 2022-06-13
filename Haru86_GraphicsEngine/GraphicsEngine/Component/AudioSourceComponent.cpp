#include "AudioSourceComponent.h"

AudioSourceComponent::AudioSourceComponent(Object* o)
	: Component(o)
{
}

AudioSourceComponent::AudioSourceComponent(Object* o, std::string audioSourcePath)
	: Component(o)
{
}

AudioSourceComponent::~AudioSourceComponent() {
	
}

bool AudioSourceComponent::Initialize() {	
	return true;
}

void AudioSourceComponent::Update(){
}
void AudioSourceComponent::ProcessInput(const SDL_Event& e) {

}

