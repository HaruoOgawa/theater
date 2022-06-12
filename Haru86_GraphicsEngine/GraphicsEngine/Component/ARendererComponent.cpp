#include "ARendererComponent.h"
#include "GraphicsEngine/Object/Object.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "./TimelineComponent.h"

ARendererComponent::ARendererComponent(Object* o) :
	Component(o), mesh(nullptr), material(nullptr), myowner(o), useZTest(true)
{
}

ARendererComponent::~ARendererComponent() {

}

void ARendererComponent::Update() {

}

void ARendererComponent::Draw() {

}

void ARendererComponent::ProcessInput(const SDL_Event& e) {

}

const std::shared_ptr<Mesh>& ARendererComponent::GetMesh()const {
	return mesh;
}

const std::shared_ptr<class Material>& ARendererComponent::GetMaterial()const {
	return material;
}

void ARendererComponent::SetUseZTest(bool use) {
	useZTest = use;
}

bool ARendererComponent::GetUseZTest()const {
	return useZTest;
}

void ARendererComponent::DrawBoard() {
}