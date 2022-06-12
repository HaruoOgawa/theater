#include "KuraGPGPU.h"
#include <string>
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Graphics.h"
#include "GraphicsEngine/Object/Object.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "../KuraArtApp.h"
#include "GraphicsEngine/Graphics/ComputeBuffer.h"
#include "GraphicsEngine/Graphics/ComputeShader.h"
#include <vector>
#include "GraphicsEngine/Graphics/glTFLoader.h"
#include "GraphicsEngine/Graphics/glTFData.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Object/CameraObject.h"
#include "GraphicsEngine/Component/CameraComponent.h"
#include "GraphicsEngine/Object/CNode.h"

KuraGPGPU::KuraGPGPU(Object* o)
	: Component(o),
	num(1024)
{
	std::shared_ptr<Object> obj = std::make_shared<Object>();
	std::shared_ptr<gltf::glTFData> data = gltf::glTFLoader::Load(obj.get(),"./Assets/Resources/glTFData/PhoenixWithVAT/Phoenix.gltf");
	m_mesh = data->GetMesh();
		
	m_material = std::make_shared<Material>(
		"./Assets/Shader/KuraShader/BoidsResult.vert",
		"./Assets/Shader/KuraShader/BoidsResult.geom",
		"./Assets/Shader/KuraShader/BoidsResult.frag");

	Start();

}

KuraGPGPU::~KuraGPGPU() {
}

void KuraGPGPU::Start() {
	//force=0 data=1
	//force buffer
	m_computeBuffer_BoidsForce = std::make_shared<ComputeBuffer>(sizeof(BoidsForce) * num);
	std::vector<BoidsForce> forceData;
	
	//data buffer
	m_computeBuffer_BoidsData = std::make_shared<ComputeBuffer>(sizeof(BoidsData) * num);
	std::vector<BoidsData> resultData;
	
	for (int i = 0; i < num;i++) {
		float size = 100.0;
		float pos[3] = {
			(rand(glm::vec2(i,0.951))*2.0-1.0)* size,
			(rand(glm::vec2(i,0.456))*2.0-1.0)* size,
			(rand(glm::vec2(i,0.753))*2.0-1.0)* size
		};
		
		glm::vec3 v = glm::normalize(glm::vec3(
			(rand(glm::vec2(0.321, i)) * 2.0 - 1.0),
			(rand(glm::vec2(0.654, i)) * 2.0 - 1.0),
			(rand(glm::vec2(0.987, i)) * 2.0 - 1.0)
		));
		float vel[3] = {
			v.x,
			v.y,
			v.z
		};
		
		//force
		forceData.push_back(BoidsForce(vel));
		
		//data
		resultData.push_back(BoidsData(pos, vel));
	}

	//force attach
	m_computeBuffer_BoidsForce->SetData<std::vector<BoidsForce>>(forceData);

	//data attach
	m_computeBuffer_BoidsData->SetData<std::vector<BoidsData>>(resultData);
	
	//SetBuffer//////////////
	//force compute
	m_computeShader_force = std::make_shared<ComputeShader>("./Assets/Shader/KuraShader/BoidsForce.comp");
	m_computeShader_force->SetBuffer(m_computeBuffer_BoidsForce, 0, m_material);
	m_computeShader_force->SetBuffer(m_computeBuffer_BoidsData, 1, m_material);

	//data compute
	m_computeShader_data = std::make_shared<ComputeShader>("./Assets/Shader/KuraShader/BoidsData.comp");
	m_computeShader_data->SetBuffer(m_computeBuffer_BoidsForce,0,m_material);
	m_computeShader_data->SetBuffer(m_computeBuffer_BoidsData,1,m_material);
	
	//set buffer to material
	m_material->SetBuffer(m_computeBuffer_BoidsForce, 0);
	m_material->SetBuffer(m_computeBuffer_BoidsData, 1);

	//clear data
	forceData.clear();
	resultData.clear();

}

void KuraGPGPU::Update() {
	//Dispatch force///////////////////////////
	m_computeShader_force->SetActive();
	m_computeShader_force->SetFloatUniform("_time", GraphicsMain::GetInstance()->time);
	m_computeShader_force->SetFloatUniform("_deltaTime", GraphicsMain::GetInstance()->deltaTime);
	//boid uniform
	m_computeShader_force->SetFloatUniform("_centerPosPower", 1000000.0);
	m_computeShader_force->SetFloatUniform("_maxBoidsDist", 50);
	
	//ŽÀÛ‚ÉŽg‚Á‚Ä‚¢‚éuniform
	m_computeShader_force->SetIntUniform("_boids_count", num);
	m_computeShader_force->SetFloatUniform("_maxBoidsField", 200);
	m_computeShader_force->SetFloatUniform("_NoiseValue", 0.1);
	m_computeShader_force->SetFloatUniform("_joinPower", 100);
	m_computeShader_force->SetFloatUniform("_separatePower", 1000);
	m_computeShader_force->SetFloatUniform("_alignmentPower", 100);
	m_computeShader_force->SetFloatUniform("_joinDisMin", 0.1);  // pre10
	m_computeShader_force->SetFloatUniform("_separateDistMin", 1000); //pre10
	m_computeShader_force->SetFloatUniform("_alignmentDistMin", 0.1); //pre10
	m_computeShader_force->Dispatch(num / 256, 1, 1);

	//Dispatch Data///////////////
	m_computeShader_data->SetActive();
	m_computeShader_data->SetFloatUniform("_time", GraphicsMain::GetInstance()->time);
	m_computeShader_data->SetFloatUniform("_deltaTime", GraphicsMain::GetInstance()->deltaTime);
	//boid uniform
	m_computeShader_data->Dispatch(num/256,1,1);

	//Camera/////////////////////
	float initPos[3] = { 0,0,0 };
	float initvel[3] = { 0,0,0 };
	int targetIndex = num/4;
	
	std::vector<BoidsData> targerBoidsData = std::vector<BoidsData>(num, BoidsData(initPos, initvel));
	m_computeBuffer_BoidsData->GetBufferData(&targerBoidsData[0], 0, num);
	float radius = 4.0;
	float speed = 0.0005;

	//camera center
	GraphicsMain::GetInstance()->game_camera_instance->m_cameraComponent->cameraCenter = glm::vec3(
		targerBoidsData[targetIndex].position[0], targerBoidsData[targetIndex].position[1], targerBoidsData[targetIndex].position[2]
	);
	//camera postion
	GraphicsMain::GetInstance()->game_camera_instance->GetRootNode()->GetComponent<TransformComponent>()->SetPosition(
		GraphicsMain::GetInstance()->game_camera_instance->m_cameraComponent->cameraCenter
		+ glm::vec3(
			glm::cos(GraphicsMain::GetInstance()->time* speed),
			glm::sin(GraphicsMain::GetInstance()->time * speed),
			glm::sin(GraphicsMain::GetInstance()->time * speed))* radius);

	targerBoidsData.clear();

}

void KuraGPGPU::Draw() {
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

	owner->GetRootNode()->GetComponent<TransformComponent>()->CalMatrix();
	m_material->SetActive();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	
	m_material->SetMatrixUniform("MVPMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetMVPMatrix() * m_mesh->localMatrix); //
	m_material->SetMatrixUniform("MMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetMMatrix());
	m_material->SetMatrixUniform("VMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetVMatrix());
	m_material->SetMatrixUniform("PMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetPMatrix());
	m_material->SetFloatUniform("_time", GraphicsMain::GetInstance()->time);
	m_material->SetFloatUniform("_deltaTime", GraphicsMain::GetInstance()->deltaTime);
	m_material->SetIntUniform("_boids_count", num);
	Graphics::DrawInstancedWithMesh(m_mesh, num,m_material,GL_TRIANGLES);

}

void KuraGPGPU::ProcessInput(const SDL_Event& e) {

}

float KuraGPGPU::rand(glm::vec2 st) {
	return glm::fract(glm::sin(glm::dot(st, glm::vec2(12.9898, 78.233))) * 43758.5453);
}