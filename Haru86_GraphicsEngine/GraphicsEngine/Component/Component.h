#pragma once

#include "../GraphicsMain/GraphicsMain.h"
#include <string>
#include <memory>

class Component
{
public:
	Component(class GameObject* o);
	virtual ~Component();
	virtual void Start();
	virtual void Update();
	virtual void Dtaw();
	friend GameObject;
protected:
	class GameObject* owner;
	class GraphicsMain* game;
	
};

