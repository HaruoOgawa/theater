#include "Time.h"

Time::Time(float fpsVal){
	fps = fpsVal;
	time = 0.0f;	
	deltaTime = 1.0f / fps;
}

Time::~Time() {

}

void Time::UpdateTime() {
	time += deltaTime;
}

float Time::GetTime() {
	return time;
}