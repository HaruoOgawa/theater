#pragma once
#include "Component.h"
#include "../Object/Object.h"
#include <string>
#include <unordered_map>
#include <memory>

class AudioSourceComponent :
    public Component
{
public:
    AudioSourceComponent(class Object* o);
    AudioSourceComponent(class Object* o, std::string audioSourcePath);
    ~AudioSourceComponent();
    bool Initialize();
	void Update() override;
	void ProcessInput(const SDL_Event& e) override;
	
};

