#include "KuraTrail.h"
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
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "GraphicsEngine/Graphics/Texture.h"
#include "./Assets/App/KuraArtApp/Script/KuraGPGPU.h"
#include "GraphicsEngine/Object/CameraObject.h"
#include "GraphicsEngine/Component/CameraComponent.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Object/CNode.h"

KuraTrail::KuraTrail(Object* o, KuraGPGPU* kuraGPGPU)
	: Component(o),
	kuraGPGPU_instance(kuraGPGPU),
	trailNum(kuraGPGPU->num),
	nodeSegment(512)
{
	nodeNum = trailNum * nodeSegment;
	
	m_material = std::make_shared<Material>(
		"./Assets/Shader/KuraShader/GPUTrail_Render.vert",
		"./Assets/Shader/KuraShader/GPUTrail_Render.geom",
		"./Assets/Shader/KuraShader/GPUTrail_Render.frag"		
	);

	m_texture_trail = std::make_shared<Texture>();
	if (!m_texture_trail->Load("./Assets/Resources/TextureData/brush_pow2._WithWing.png")) {
		printf("Cannot Load Texture\n");
	}

	m_mesh = std::make_shared<Mesh>(PrimitiveType::POINT);
	
	//compute shader
	m_computeShader_NextInputPos = std::make_shared<ComputeShader>("./Assets/Shader/KuraShader/GPUTrail_NextInputPos.comp");
	m_computeShader_NodeInfo = std::make_shared<ComputeShader>("./Assets/Shader/KuraShader/GPUTrail_NodeInfo.comp");

	//compute buffer
	m_buffer_trail = std::make_shared<ComputeBuffer>(sizeof(trail)* trailNum);
	m_buffer_node = std::make_shared<ComputeBuffer>(sizeof(node)* nodeNum);
	m_buffer_input = std::make_shared<ComputeBuffer>(sizeof(input_data)* trailNum);

	Start();
}

KuraTrail::~KuraTrail() {
	kuraGPGPU_instance = nullptr;
}

void KuraTrail::Start() {
	//init buffer data
	std::vector<trail> init_trail = std::vector<trail>(trailNum, trail(0, nodeSegment));
	float input_pos[3] = { 0.0,0.0,0.0 };
	std::vector<input_data> init_input_data = std::vector<input_data>(trailNum, input_data(input_pos));
	
	std::vector<node> init_node;
	float initPos[3] = { 0,0,0 };
	float initvel[3] = { 0,0,0 };
	std::vector<BoidsData> init_boidsData = std::vector<BoidsData>(trailNum, BoidsData(initPos, initvel));
	kuraGPGPU_instance->m_computeBuffer_BoidsData->GetBufferData(&init_boidsData[0], 0, trailNum);

	for (int i = 0; i < trailNum;i++) {
		float boids_pos[3] = { init_boidsData[i].position[0],init_boidsData[i].position[1],init_boidsData[i].position[2] };
		for (int q = 0; q < nodeSegment; q++) {
			init_node.push_back(node(boids_pos, 5));
		}
	}

	init_boidsData.clear();

	//set init data to buffer
	m_buffer_trail->SetData<std::vector<trail>>(init_trail);
	m_buffer_node->SetData<std::vector<node>>(init_node);
	m_buffer_input->SetData<std::vector<input_data>>(init_input_data);

	//clear init data
	init_trail.clear();
	init_input_data.clear();
	init_node.clear();

	//next input
	m_computeShader_NextInputPos->SetBuffer(kuraGPGPU_instance->m_computeBuffer_BoidsData, 1, m_material);
	m_computeShader_NextInputPos->SetBuffer(m_buffer_input, 2, m_material);
	//node info
	m_computeShader_NodeInfo->SetBuffer(m_buffer_node, 3, m_material);
	m_computeShader_NodeInfo->SetBuffer(m_buffer_trail, 4, m_material);

	//set buffer to material
	m_material->SetBuffer(m_buffer_node, 3);
	m_material->SetBuffer(m_buffer_trail, 4);

}

void KuraTrail::Update() {
	//NextInputPos
	m_computeShader_NextInputPos->SetActive();
	m_computeShader_NextInputPos->Dispatch(trailNum/256,1,1);

	//NodeInfo
	m_computeShader_NodeInfo->SetActive();
	m_computeShader_NodeInfo->SetIntUniform("_nodeSegment", nodeSegment);
	m_computeShader_NodeInfo->SetFloatUniform("_nodeDistanceMin", 1);
	m_computeShader_NodeInfo->SetFloatUniform("_time",GraphicsMain::GetInstance()->time );
	m_computeShader_NodeInfo->SetFloatUniform("_initNodeLife", 5.0);
	m_computeShader_NodeInfo->Dispatch(trailNum / 256, 1, 1);

}

void KuraTrail::ProcessInput(const SDL_Event& e) {

}

void KuraTrail::Draw() {
	
	owner->GetRootNode()->GetComponent<TransformComponent>()->CalMatrix();
	m_material->SetActive();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	m_material->SetMatrixUniform("MVPMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetMVPMatrix()); 
	m_material->SetMatrixUniform("MMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetMMatrix());
	m_material->SetMatrixUniform("VMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetVMatrix());
	m_material->SetMatrixUniform("PMatrix", owner->GetRootNode()->GetComponent<TransformComponent>()->GetPMatrix());
	m_material->SetFloatUniform("_time", GraphicsMain::GetInstance()->time);
	m_material->SetFloatUniform("_deltaTime", GraphicsMain::GetInstance()->deltaTime);

	//GPU Trail Uniform
	m_material->SetVec3Uniform("_WorldSpaceCameraPos",GraphicsMain::GetInstance()->game_camera_instance->GetRootNode()->GetComponent<TransformComponent>()->GetPosition());
	m_material->SetFloatUniform("_initNodeLife", 5);
	m_material->SetFloatUniform("_nodeSegment",nodeSegment );
	m_material->SetFloatUniform("_TrailWidth", 0.4*7.0);
	m_material->SetIntUniform("_boids_count", nodeNum);

	m_texture_trail->SetActive();
	m_material->SetTexUniform("_trailTex", m_texture_trail->GetTextureID());

	Graphics::DrawInstancedWithMesh(m_mesh, nodeNum, m_material, GL_POINTS);
}