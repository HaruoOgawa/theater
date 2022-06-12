#pragma once
#include "Object.h"
#include <string>
#include <memory>

class AudioObject :
    public Object
{
public:
    AudioObject();
    AudioObject(std::string audioSourcePath);
    ~AudioObject();
    bool Initialize();
    void PlayAudio();
    friend class GraphicsMain;
    friend class BaseApp;
    friend class UIComponent;
    std::shared_ptr<class AudioSourceComponent> audioSource;
};

