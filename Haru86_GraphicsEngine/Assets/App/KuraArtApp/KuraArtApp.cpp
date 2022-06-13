#include "KuraArtApp.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Object/TimelineObject.h"
#include "GraphicsEngine/Component/TimelineComponent.h"
#include "GraphicsEngine/Component/UIComponent.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/RectTransformComponent.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Object/AudioObject.h"
#include "GraphicsEngine/Component/AudioSourceComponent.h"
#include <memory>
#include <string>
#include "GraphicsEngine/Object/Object.h"
#include "./Assets/App/KuraArtApp/Script/KuraGPGPU.h"
#include "./Assets/App/KuraArtApp/Script/KuraTrail.h"
#include "GraphicsEngine/Object/CameraObject.h"
#include <glew.h>
#include "GraphicsEngine/Object/CNode.h"

KuraArtApp::KuraArtApp()
	: BaseApp()
{
}

KuraArtApp::~KuraArtApp() {
	background_obj.reset();
	KuraTerrain_obj.reset();
	audioObj.reset();
}

void KuraArtApp::Start() {
	background_obj = std::make_unique<GameObject>(PrimitiveType::SPHERE,
		"./Assets/Shader/KuraShader/Background.vert",
		"./Assets/Shader/KuraShader/Background.frag");
	background_obj->SetScale(glm::vec3(5000.0));
	std::vector<std::string> texPath = { "./Assets/Resources/TextureData/InkBackground.png" };
	std::vector<std::string> texUniformName = { "_backgroundTex" };
	background_obj->meshComp->GetMaterial()->LoadTextureList(texPath, texUniformName);

	camera_obj = std::make_shared<CameraObject>(CameraType::FIXED_CAMERA);
	camera_obj->SetPosition(glm::vec3(0, 0.0, -3.0));
	GraphicsMain::GetInstance()->game_camera_instance = camera_obj;

	audioObj = std::make_unique<AudioObject>("./Assets/Resources/MusicData/n111.mp3");
	if (audioObj->Initialize()) {
		audioObj->PlayAudio();
	}

	std::map<GLenum, std::string> shaders = {
		{GL_VERTEX_SHADER,"./Assets/Shader/KuraShader/KuraTerrain.vert"},
		{GL_TESS_CONTROL_SHADER, "./Assets/Shader/KuraShader/KuraTerrain.tesc"},
		{GL_TESS_EVALUATION_SHADER,"./Assets/Shader/KuraShader/KuraTerrain.tese"},
		{GL_GEOMETRY_SHADER,"./Assets/Shader/KuraShader/KuraTerrain.geom"},
		{GL_FRAGMENT_SHADER,"./Assets/Shader/KuraShader/KuraTerrain.frag"}
	};
	
	KuraTerrain_obj = std::make_unique<GameObject>(PrimitiveType::BOARD,shaders);
	KuraTerrain_obj->SetScale(glm::vec3(5000.0,5000.0,1.0));
	KuraTerrain_obj->SetPosition(glm::vec3(0.0,-1.0,0.0));
	KuraTerrain_obj->SetRotation(glm::quat(glm::vec3(3.14159265/2.0,0.0,0.0)));
	std::vector<std::string> texPath_KuraTerrain_obj = { "./Assets/Resources/TextureData/KuraTerrainHeightMap.png","./Assets/Resources/TextureData/InkBackground.png" };
	std::vector<std::string> texUniformName_KuraTerrain_obj = { "_heightMap","_terrainMap" };
	KuraTerrain_obj->meshComp->GetMaterial()->LoadTextureList(texPath_KuraTerrain_obj, texUniformName_KuraTerrain_obj);

	gpgpu_obj = std::make_unique<Object>();
	gpgpu_obj->GetRootNode()->AddComponent<KuraGPGPU>(std::make_shared<KuraGPGPU>(gpgpu_obj.get()));
	gpgpu_obj->GetRootNode()->AddComponent<KuraTrail>(std::make_shared<KuraTrail>(gpgpu_obj.get(),
		gpgpu_obj->GetRootNode()->GetComponent<KuraGPGPU>())
	);

	GraphicsMain::GetInstance()->renderBoard = std::make_unique<GameObject>(PrimitiveType::BOARD, "./Assets/Shader/VedaStructuredShader/BoardRender.vert", "./Assets/Shader/KuraShader/KuraPostProcess.frag", RenderType::FrameBuffer);

}
void KuraArtApp::Update() {
	gpgpu_obj->Update();
}

void KuraArtApp::Draw() {
	gpgpu_obj->GetRootNode()->GetComponent<KuraGPGPU>()->Draw();
	gpgpu_obj->GetRootNode()->GetComponent<KuraTrail>()->Draw();
}

void KuraArtApp::Timeline(class TimelineObject* timeline) {
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 0.0, 1.0, [](float val) {
		GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_renderMode", 0.0);
		float animVal = 1.0;
		GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_alphaVal", val/ animVal);
	});
	
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 1.0, 3.0, [](float val) {
		GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_renderMode", 2.0);
		float animVal = 2.0;
		GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_alphaVal", val / animVal);
	});
	
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 235.0 -2.0, 235.0 -1.0, [](float val) {
		GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_renderMode", 1.0);
		float animVal = 1.0;
		GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_alphaVal", val / animVal);
	});
}

