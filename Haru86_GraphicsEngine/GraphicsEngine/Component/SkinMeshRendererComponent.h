#pragma once
#include "ARendererComponent.h"
#include <glm/glm.hpp>

class Mesh;

namespace animation {
	class AnimatorComponent;
}

class SkinMeshRendererComponent :
	public ARendererComponent
{
	std::shared_ptr<animation::AnimatorComponent> m_Animator; // Updateはルートノードでしか呼ばないのでAddComponentしない
	std::string m_NodeName;
	int m_MeshIndex;
public:
	SkinMeshRendererComponent(Object* o, std::string objPath,const std::shared_ptr<Mesh>& MeshData,const std::shared_ptr<animation::AnimatorComponent>& Animator,
		int MeshIndex,const std::string& NodeName,std::string vertexShaderName= "./Assets/Shader/MToon.vert", std::string fragmentShaderName= "./Assets/Shader/MToon.frag");
	~SkinMeshRendererComponent();

	virtual void Update() override;
	virtual void Draw() override;
	void Draw(const std::vector<glm::mat4>& ModelMatrixTree);
	virtual void ProcessInput(const SDL_Event& e) override;
};