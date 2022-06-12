#include "GraphicsEngine/Object/Object.h"
#include "GraphicsEngine/Component/Component.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "GraphicsEngine/Graphics/Graphics.h"
#include "GraphicsEngine/Object/CameraObject.h"
#include "CKura3DGPUTrail.h"
#include "GraphicsEngine/Graphics/ComputeShader.h"
#include "GraphicsEngine/Graphics/ComputeBuffer.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Object/CNode.h"

CKura3DGPUTrail::CKura3DGPUTrail(Object* o)
	: Component(o),
	num(32),
	m_computeBuffer_BoidsForce(nullptr),
	m_computeBuffer_BoidsData(nullptr),
	m_computeShader_force(nullptr),
	m_computeShader_data(nullptr),
	m_material(nullptr),
	m_mesh(nullptr),
	m_buffer_trail(nullptr),
	m_buffer_node(nullptr),
	m_buffer_input(nullptr),
	m_computeShader_NextInputPos(nullptr),
	m_computeShader_NodeInfo(nullptr),
	nodeSegment(256)
{
	trailNum = num;
	nodeNum = trailNum * nodeSegment;

	Start();
}

CKura3DGPUTrail::~CKura3DGPUTrail() {
}

void CKura3DGPUTrail::Start() {
	//Boids/////////////////////////////////////////////////
	//force=0 data=1
	//force buffer
	m_computeBuffer_BoidsForce = std::make_shared<ComputeBuffer>(sizeof(BoidsForce) * num);
	std::vector<BoidsForce> forceData;

	//data buffer
	m_computeBuffer_BoidsData = std::make_shared<ComputeBuffer>(sizeof(BoidsData) * num);
	std::vector<BoidsData> resultData;

	for (int i = 0; i < num; i++) {
		float size = 0.0;
		float pos[3] = {
			(rand(glm::vec2(i,0.951)) * 2.0 - 1.0) * size,
			(rand(glm::vec2(i,0.456)) * 2.0 - 1.0) * size,
			(rand(glm::vec2(i,0.753)) * 2.0 - 1.0) * size
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
	m_computeShader_force = std::make_shared<ComputeShader>("./Assets/Shader/Kura02/GPUTrail_Force.comp");
	
	//data compute
	m_computeShader_data = std::make_shared<ComputeShader>("./Assets/Shader/Kura02/GPUTrail_Data.comp");
	
	//clear data
	forceData.clear();
	resultData.clear();
	////////////////////////////////////////////////////////
	
	//Trail /////////////////////////////////////////////////
	m_material = std::make_shared<Material>(
		"./Assets/Shader/Kura02/GPUTrail_Render.vert",
		"./Assets/Shader/Kura02/GPUTrail_Render.geom",
		"./Assets/Shader/Kura02/GPUTrail_Render.frag"
		);

	m_texture_trail = std::make_shared<Texture>();
	if (!m_texture_trail->Load("./Assets/Resources/TextureData/brush_pow2._WithWing.png")) {
		printf("Cannot Load Texture\n");
	}

	m_mesh = std::make_shared<Mesh>(PrimitiveType::POINT);

	//compute shader
	m_computeShader_NextInputPos = std::make_shared<ComputeShader>("./Assets/Shader/Kura02/GPUTrail_NextInputPos.comp");
	m_computeShader_NodeInfo = std::make_shared<ComputeShader>("./Assets/Shader/Kura02/GPUTrail_NodeInfo.comp");

	//compute buffer
	m_buffer_trail = std::make_shared<ComputeBuffer>(sizeof(trail) * trailNum);
	m_buffer_node = std::make_shared<ComputeBuffer>(sizeof(node) * nodeNum);
	m_buffer_input = std::make_shared<ComputeBuffer>(sizeof(input_data) * trailNum);

	//init buffer data
	std::vector<trail> init_trail = std::vector<trail>(trailNum, trail(0, nodeSegment));
	float input_pos[3] = { 0.0,0.0,0.0 };
	std::vector<input_data> init_input_data = std::vector<input_data>(trailNum, input_data(input_pos));

	std::vector<node> init_node;
	float initPos[3] = { 0,0,0 };
	float initvel[3] = { 0,0,0 };
	std::vector<BoidsData> init_boidsData = std::vector<BoidsData>(trailNum, BoidsData(initPos, initvel));
	m_computeBuffer_BoidsData->GetBufferData(&init_boidsData[0], 0, trailNum);

	for (int i = 0; i < trailNum; i++) {
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
	m_computeShader_NextInputPos->SetBuffer(m_computeBuffer_BoidsData, 1, m_material);
	m_computeShader_NextInputPos->SetBuffer(m_buffer_input, 2, m_material);
	//node info
	m_computeShader_NodeInfo->SetBuffer(m_buffer_node, 3, m_material);
	m_computeShader_NodeInfo->SetBuffer(m_buffer_trail, 4, m_material);

	//set buffer to material
	m_material->SetBuffer(m_buffer_node, 3);
	m_material->SetBuffer(m_buffer_trail, 4);
	////////////////////////////////////////////////////////

}

void CKura3DGPUTrail::Update() {
	//Boids/////////////////////////////////////////////////
	//Dispatch force///////////////////////////
	m_computeShader_force->SetActive();
	m_computeShader_force->SetFloatUniform("_time", GraphicsMain::GetInstance()->time*0.001);
	m_computeShader_force->SetFloatUniform("_deltaTime", GraphicsMain::GetInstance()->deltaTime);
	//boid uniform
	m_computeShader_force->SetFloatUniform("_centerPosPower", 1000000.0);
	m_computeShader_force->SetFloatUniform("_maxBoidsDist", 50);

	//ŽÀÛ‚ÉŽg‚Á‚Ä‚¢‚éuniform
	m_computeShader_force->SetIntUniform("_boids_count", num);
	//m_computeShader_force->SetFloatUniform("_maxBoidsField", 200);
	m_computeShader_force->SetVec3Uniform("_maxBoidsField", glm::vec3(50,10,50));
	m_computeShader_force->SetFloatUniform("_NoiseValue", 0.1);
	m_computeShader_force->SetFloatUniform("_joinPower", 100);
	m_computeShader_force->SetFloatUniform("_separatePower", 1000);
	m_computeShader_force->SetFloatUniform("_alignmentPower", 100);
	m_computeShader_force->SetFloatUniform("_joinDisMin", 0.1);  // pre10
	m_computeShader_force->SetFloatUniform("_separateDistMin", 1000); //pre10
	m_computeShader_force->SetFloatUniform("_alignmentDistMin", 0.1); //pre10
	m_computeShader_force->Dispatch(num / 32, 1, 1);

	//Dispatch Data///////////////
	m_computeShader_data->SetActive();
	m_computeShader_data->SetFloatUniform("_time", GraphicsMain::GetInstance()->time * 0.001);
	m_computeShader_data->SetFloatUniform("_deltaTime", GraphicsMain::GetInstance()->deltaTime);
	//boid uniform
	m_computeShader_data->Dispatch(num / 32, 1, 1);
	////////////////////////////////////////////////////////
	
	//Trail /////////////////////////////////////////////////
	//NextInputPos
	m_computeShader_NextInputPos->SetActive();
	m_computeShader_NextInputPos->Dispatch(trailNum / 32, 1, 1);

	//NodeInfo
	m_computeShader_NodeInfo->SetActive();
	m_computeShader_NodeInfo->SetIntUniform("_nodeSegment", nodeSegment);
	m_computeShader_NodeInfo->SetFloatUniform("_nodeDistanceMin", 0.01);
	m_computeShader_NodeInfo->SetFloatUniform("_time", GraphicsMain::GetInstance()->time * 0.001);
	m_computeShader_NodeInfo->SetFloatUniform("_initNodeLife", 5.0);
	m_computeShader_NodeInfo->Dispatch(trailNum / 32, 1, 1);
	////////////////////////////////////////////////////////
}

void CKura3DGPUTrail::Draw() {
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
	m_material->SetVec3Uniform("_WorldSpaceCameraPos", GraphicsMain::GetInstance()->game_camera_instance->GetRootNode()->GetComponent<TransformComponent>()->GetPosition());
	m_material->SetFloatUniform("_initNodeLife", 5);
	m_material->SetFloatUniform("_nodeSegment", nodeSegment);
	//m_material->SetFloatUniform("_TrailWidth", 0.4 * 7.0);
	m_material->SetFloatUniform("_TrailWidth", 0.1 );
	m_material->SetIntUniform("_boids_count", nodeNum);

	m_texture_trail->SetActive();
	m_material->SetTexUniform("_trailTex", m_texture_trail->GetTextureID());

	m_material->SetVec3Uniform("_CameraPosition", GraphicsMain::GetInstance()->game_camera_instance->GetPosition());
	m_material->SetVec3Uniform("_lightDir", glm::vec3(-2.0, 1.0, 1.0));
	m_material->SetFloatUniform("_Roughness", 0.5);
	m_material->SetFloatUniform("_FresnelReflectance", 1.0);

	Graphics::DrawInstancedWithMesh(m_mesh, nodeNum, m_material, GL_POINTS);

}

void CKura3DGPUTrail::ProcessInput(const SDL_Event& e) {

}

float CKura3DGPUTrail::rand(glm::vec2 st) {
	return glm::fract(glm::sin(glm::dot(st, glm::vec2(12.9898, 78.233))) * 43758.5453);
}