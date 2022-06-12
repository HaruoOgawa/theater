#include "Component.h"
#include "../GraphicsMain/GraphicsMain.h"

Component::Component(Object* o)
	: owner(o),game(GraphicsMain::GetInstance())
{
	Start();
}

Component::~Component() {
}

void Component::Start() {
}

void Component::Update() {

}

void Component::Dtaw() {
}

