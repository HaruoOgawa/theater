#pragma once

#include "../Object/Object.h"
#include "../GraphicsMain/GraphicsMain.h"
#include <string>
#include <memory>

class Component
{
public:
	Component(class Object* o);
	virtual ~Component();
	virtual void Start();
	virtual void Update();
	virtual void Dtaw();
	friend Object;
protected:
	class Object* owner;
	class GraphicsMain* game;
	
};

