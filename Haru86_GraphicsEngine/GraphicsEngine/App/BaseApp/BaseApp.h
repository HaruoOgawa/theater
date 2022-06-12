#pragma once
#include <memory>

class BaseApp
{
public:
	BaseApp();
	virtual ~BaseApp();
private:
	friend class GraphicsMain;
	friend class GraphicsRenderer;
	virtual void Start()  = 0;
	virtual void Update()  = 0;
	virtual void Draw()  = 0;
	virtual void Timeline(class TimelineObject* timelineObj)  = 0;
};

