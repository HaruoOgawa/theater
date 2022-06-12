#pragma once
class Time
{
public:
	Time(float fpsVal);
	~Time();
	void UpdateTime();
	float GetTime();
	float fps;
	float deltaTime;
	friend class UIComponent;
private:
	float time;
};

