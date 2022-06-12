#pragma once
#include "GraphicsEngine/Component/Component.h"

class Object;
class Mesh;
class Material;
class TimelineAnimationClip;
class ARendererComponent :
	public Component
{
public:
	ARendererComponent(Object* o);
	~ARendererComponent();

	virtual void Update() override = 0;
	virtual void Draw() = 0;
	virtual void ProcessInput(const SDL_Event& e) override = 0;

	const std::shared_ptr<Mesh>& GetMesh()const;
	const std::shared_ptr<Material>& GetMaterial()const;
	void SetUseZTest(bool use);
	bool GetUseZTest()const;

	virtual void DrawBoard();

	std::vector<TimelineAnimationClip*> animationClips;
protected:
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
	class Object* myowner;
	bool useZTest;
};