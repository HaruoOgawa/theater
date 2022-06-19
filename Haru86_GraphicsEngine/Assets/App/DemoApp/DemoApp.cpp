#include "DemoApp.h"
#include "../../../GraphicsEngine/Object/GameObject.h"
#include "../../../GraphicsEngine/Object/TimelineObject.h"
#include "../../../GraphicsEngine/Component/TimelineComponent.h"
#include "../../../GraphicsEngine/Component/UIComponent.h"
#include "../../../GraphicsEngine/Component/RectTransformComponent.h"
#include "../../../GraphicsEngine/Component/MeshRendererComponent.h"
#include "../../../GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include <memory>
#include "GraphicsEngine/Object/AudioObject.h"

DemoApp::DemoApp()
{

}

DemoApp::~DemoApp() {

}
void DemoApp::Start()  {

	/*Scene1_Frag = std::make_shared<GameObject>(PrimitiveType::BOARD, "./Assets/Shader/demo.vert", "./Assets/Shader/VedaStructuredShader/scene1.frag");
	Scene2_Frag = std::make_shared<GameObject>(PrimitiveType::BOARD, "./Assets/Shader/demo.vert", "./Assets/Shader/VedaStructuredShader/scene2.frag");
	Scene3_Frag = std::make_shared<GameObject>(PrimitiveType::BOARD, "./Assets/Shader/demo.vert", "./Assets/Shader/VedaStructuredShader/scene3.frag");
	Scene4_Frag = std::make_shared<GameObject>(PrimitiveType::BOARD, "./Assets/Shader/demo.vert", "./Assets/Shader/VedaStructuredShader/scene4.frag");
	Scene5_Frag = std::make_shared<GameObject>(PrimitiveType::BOARD, "./Assets/Shader/demo.vert", "./Assets/Shader/VedaStructuredShader/scene5.frag");

	/*GameObject* uiButton = new GameObject(PrimitiveType::BOARD,"./Assets/Shader/UIStandard.vert","./Assets/Shader/UIStandard.frag");
	uiButton->SetScale(glm::vec3(0.1f,0.05f,1.0f)*0.5f);
	uiButton->AddComponent(new UIComponent(uiButton));
	uiButton->AddComponent(new RectTransformComponent(uiButton,0.1f,0.1f));*/

	/*DemoText = std::make_shared<GameObject>(PrimitiveType::BOARD, "./Assets/Shader/FontText.vert", "./Assets/Shader/FontText.frag", "Demo Scene");
	MusicText = std::make_shared<GameObject>(PrimitiveType::BOARD, "./Assets/Shader/FontText.vert", "./Assets/Shader/FontText.frag","Music: lug00ber");
	MyCreditText= std::make_shared<GameObject>(PrimitiveType::BOARD, "./Assets/Shader/FontText.vert", "./Assets/Shader/FontText.frag","Mouvie: Haru86_");
	TitleText = std::make_shared<GameObject>(PrimitiveType::BOARD, "./Assets/Shader/FontText.vert", "./Assets/Shader/FontText.frag","Labyrinth");

	//Load PostProcess
	GraphicsMain::GetInstance()->renderBoard = std::make_unique<GameObject>(PrimitiveType::BOARD, "./Assets/Shader/VedaStructuredShader/BoardRender.vert", "./Assets/Shader/VedaStructuredShader/BoardRender.frag", RenderType::FrameBuffer);

	DemoAudioObj = std::make_shared<AudioObject>("./Assets/Resources/MusicData/lug00ber_they_are_feeding.mp3");
	if (DemoAudioObj->Initialize()) {
		DemoAudioObj->PlayAudio();
	}*/

}
void DemoApp::Update()  {

}
	
void DemoApp::Draw() {

}

void DemoApp::Timeline(class TimelineObject* timeline)  {
	/*//Ray Activation
	timeline->AddActivationClip(Scene1_Frag.get(),0.0f,42.0f);
	timeline->AddActivationClip(Scene2_Frag.get(), 42.0f, 64.0f);
	timeline->AddActivationClip(Scene3_Frag.get(), 64.0f, 85.0f);
	//-14.0f
	float adjustTime = 1.5f;

	timeline->AddActivationClip(Scene4_Frag.get(), 85.0f, 128.0f - 12.0f- adjustTime);
	timeline->AddActivationClip(Scene5_Frag.get(), 128.0f - 12.0f - adjustTime, 146.0f-20.0f + 1.0f - adjustTime) ;
	timeline->AddActivationClip(Scene1_Frag.get(), 126.0f + 1.0f - adjustTime, 157.0f - adjustTime);
	//146.0f - 20.0f=126.0f, 179.0f - 22.0f=157.0f
	
		float startTime = 0.0f;	
	float timeVal = 3.0f;
	float endTime = 0.0f;
	endTime += timeVal;

	GraphicsMain* gamept = GraphicsMain::GetInstance();

	timeline->AddAnimationClip(Scene1_Frag.get(), 0.0f,0.5f, [this](float val) {Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_isWhiteOut", 0.0); });
	timeline->AddAnimationClip(Scene1_Frag.get(), 0.0f, 0.5f, [this](float val) {Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_isBlackOut", 0.0); });

	for (int i = 0; i < 13;i++) {
		int val = i % 3;
		switch (val) {
		case 0:
			timeline->AddAnimationClip(Scene1_Frag.get(), static_cast<float>(startTime + 3.0 * i), static_cast<float>(endTime + 3.0 * i), [gamept, this](float val) {gamept->GetInstance()->animTime = 0.0f; Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 0); });
			break;
		case 1:
			timeline->AddAnimationClip(Scene1_Frag.get(), static_cast<float>(startTime + 3.0 * i), static_cast<float>(endTime + 3.0 * i), [gamept, this](float val) {gamept->GetInstance()->animTime = 0.0f; Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 1); });
			break;
		case 2:
			timeline->AddAnimationClip(Scene1_Frag.get(), static_cast<float>(startTime + 3.0 * i), static_cast<float>(endTime + 3.0 * i), [gamept, this](float val) {gamept->GetInstance()->animTime = 0.0f; Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 2); });
			break;
		default:
			break;

		}
		
	}

	timeline->AddAnimationClip(Scene1_Frag.get(), 38.0f, 42.0f,
		[this](float val) {
			Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 3);
			Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraMoveVal", val);
			

	});
	
	timeline->AddAnimationClip(Scene1_Frag.get(), 42.0f, 42.5f,
		[this](float val) {
			Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 0);
			Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraMoveVal", val);
	});

	startTime = 42.0f;
	endTime = startTime + 3.0;

	for (int i = 0; i < 6;i++) {
		int val = i % 3;
		switch (val) {
		case 0:
			timeline->AddAnimationClip(Scene2_Frag.get(), static_cast<float>(startTime + 3.0 * i), static_cast<float>(endTime + 3.0 * i), [this](float val) {Scene2_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 0); });
			break;
		case 1:
			timeline->AddAnimationClip(Scene2_Frag.get(), static_cast<float>(startTime + 3.0 * i), static_cast<float>(endTime + 3.0 * i), [this] (float val){Scene2_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 1); });
			break;
		case 2:
			timeline->AddAnimationClip(Scene2_Frag.get(), static_cast<float>(startTime + 3.0 * i), static_cast<float>(endTime + 3.0 * i), [this](float val) {Scene2_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 2); });
			break;
		default:
			break;

		}
	}
	
	startTime = 64.0f;
	endTime = startTime + 3.0;

	for (int i = 0; i < 8;i++) {
		int val = i % 3;
		switch (val) {
		case 0:
			timeline->AddAnimationClip(Scene3_Frag.get(), static_cast<float>(startTime + 3.0 * i), static_cast<float>(endTime + 3.0 * i), [this](float val) {Scene3_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 0.0); });
			break;
		case 1:
			timeline->AddAnimationClip(Scene3_Frag.get(), static_cast<float>(startTime + 3.0 * i), static_cast<float>(endTime + 3.0 * i), [this] (float val){Scene3_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 1.0); });
			break;
		case 2:
			timeline->AddAnimationClip(Scene3_Frag.get(), static_cast<float>(startTime + 3.0 * i), static_cast<float>(endTime + 3.0 * i), [this](float val) {Scene3_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 2.0); });
			break;
		default:
			break;

		}
	}
	
	startTime = 85.0f;
	endTime = startTime + 3.0;

	for (int i = 0; i < 11;i++) {
		int val = i % 3;
		switch (val) {
		case 0:
			timeline->AddAnimationClip(Scene4_Frag.get(), static_cast<float>(startTime + 3.0 * i), static_cast<float>(endTime + 3.0 * i), [this](float val) {Scene4_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 0.0); });
			break;
		case 1:
			timeline->AddAnimationClip(Scene4_Frag.get(), static_cast<float>(startTime + 3.0 * i), static_cast<float>(endTime + 3.0 * i), [this] (float val){Scene4_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 1.0); });
			break;
		case 2:
			timeline->AddAnimationClip(Scene4_Frag.get(), static_cast<float>(startTime + 3.0 * i), static_cast<float>(endTime + 3.0 * i), [this](float val) {Scene4_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 2.0); });
			break;
		default:
			break;

		}
	}
	
	startTime = 128.0f - 12.0f;
	endTime = startTime + 3.0;

	for (int i = 0; i < 5;i++) {
		int val = i % 3;
		switch (val) {
		case 0:
			timeline->AddAnimationClip(Scene5_Frag.get(), static_cast<float>(startTime + 3.0 * i), static_cast<float>(endTime + 3.0 * i), [this](float val) {Scene5_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 0.0); });
			break;
		case 1:
			timeline->AddAnimationClip(Scene5_Frag.get(), static_cast<float>(startTime + 3.0 * i), static_cast<float>(endTime + 3.0 * i), [this] (float val){Scene5_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 1.0); });
			break;
		case 2:
			timeline->AddAnimationClip(Scene5_Frag.get(), static_cast<float>(startTime + 3.0 * i), static_cast<float>(endTime + 3.0 * i), [this](float val) {Scene5_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 2.0); });
			break;
		default:
			break;

		}
	}

	timeline->AddAnimationClip(Scene5_Frag.get(), 125.0f+1.0f - adjustTime,126.0f + 1.0f - adjustTime, [this](float val) {Scene5_Frag->meshComp->GetMaterial()->SetFloatUniform("_isWhiteOut", 1.0); });
	timeline->AddAnimationClip(Scene5_Frag.get(), 125.0f + 1.0f - adjustTime,126.0f + 1.0f - adjustTime, [this](float val) {Scene5_Frag->meshComp->GetMaterial()->SetFloatUniform("_whiteOutVal", val); });
	
	timeline->AddAnimationClip(Scene1_Frag.get(), 126.0f + 1.0f - adjustTime,127.0f + 1.0f - adjustTime, [this](float val) {Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_isWhiteOut", 1.0); });
	timeline->AddAnimationClip(Scene1_Frag.get(), 126.0f + 1.0f - adjustTime,127.0f + 1.0f - adjustTime, [this](float val) {Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_whiteOutVal", 1.0f-val); });
	timeline->AddAnimationClip(Scene1_Frag.get(), 127.0f + 1.0f - adjustTime, 127.5f + 1.0f - adjustTime, [this](float val) {Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_isWhiteOut", 0.0); });
	
	timeline->AddAnimationClip(Scene1_Frag.get(), 126.0f + 30.0 - adjustTime, 126.0f + 30.0+0.5f - adjustTime, [this](float val) {Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_isBlackOut", 1.0); });
	timeline->AddAnimationClip(Scene1_Frag.get(), 126.0f + 30.0 - adjustTime, 126.0f + 30.0+0.5f - adjustTime, [this](float val) {Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_blackOutVal", 1.0f-val); });
	

	timeline->AddAnimationClip(Scene2_Frag.get(), static_cast<float>(57), static_cast<float>(64.0), [this](float val) {Scene2_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 0);  });

	timeline->AddAnimationClip(Scene4_Frag.get(), 85.0, 104.0f- adjustTime, [this](float val) {Scene4_Frag->meshComp->GetMaterial()->SetFloatUniform("isTransform",0.0f); });
	timeline->AddAnimationClip(Scene4_Frag.get(), 104.0f - adjustTime, 105.5 - adjustTime , [this](float val) {Scene4_Frag->meshComp->GetMaterial()->SetFloatUniform("isTransform",1.0f); });

	timeline->AddAnimationClip(Scene1_Frag.get(), 126.0f + 1.0f - adjustTime, 157.0f - adjustTime,
		[this](float val) {
		Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraPositionIndex", 4);
		Scene1_Frag->meshComp->GetMaterial()->SetFloatUniform("_cameraMoveVal", val);
	});

	//font/////////////////////////////////

	//FirstText
	timeline->AddActivationClip(DemoText.get(), 0.0f, 10.0f);
	timeline->AddActivationClip(MusicText.get(), 10.0f, 20.0f);
	timeline->AddActivationClip(MyCreditText.get(), 20.0f, 30.0f);
	timeline->AddActivationClip(TitleText.get(), 30.0f, 40.0f);
	
	timeline->AddAnimationClip(DemoText.get(), 0.0f, 10.0f, [this](float val) {
		
	//printf("val: %f \n", val);
	
	float delayVal = 0.2f;
	if (val<= delayVal) {
		DemoText->meshComp->GetMaterial()->SetFloatUniform("_alpha", val/ delayVal);
	}
	else if (val > delayVal && val < 1.0f- delayVal) {
		DemoText->meshComp->GetMaterial()->SetFloatUniform("_alpha", 1.0f);
	}
	else if (val>=1.0f- delayVal) {
		DemoText->meshComp->GetMaterial()->SetFloatUniform("_alpha", (1.0f - val)/delayVal);
	}

	});

	timeline->AddAnimationClip(MusicText.get(), 10.0f, 20.0f, [this](float val) {
		
		//printf("val: %f \n", val);

		float delayVal = 0.2f;
		if (val <= delayVal) {
			MusicText->meshComp->GetMaterial()->SetFloatUniform("_alpha", val / delayVal);
		}
		else if (val > delayVal && val < 1.0f - delayVal) {
			MusicText->meshComp->GetMaterial()->SetFloatUniform("_alpha", 1.0f);
		}
		else if (val >= 1.0f - delayVal) {
			MusicText->meshComp->GetMaterial()->SetFloatUniform("_alpha", (1.0f - val) / delayVal);
		}

	});
	timeline->AddAnimationClip(MyCreditText.get(), 20.0f, 30.0f, [this](float val) {
		
		//printf("val: %f \n", val);

		float delayVal = 0.2f;
		if (val <= delayVal) {
			MyCreditText->meshComp->GetMaterial()->SetFloatUniform("_alpha", val / delayVal);
		}
		else if (val > delayVal && val < 1.0f - delayVal) {
			MyCreditText->meshComp->GetMaterial()->SetFloatUniform("_alpha", 1.0f);
		}
		else if (val >= 1.0f - delayVal) {
			MyCreditText->meshComp->GetMaterial()->SetFloatUniform("_alpha", (1.0f - val) / delayVal);
		}

	});
	timeline->AddAnimationClip(TitleText.get(), 30.0f, 40.0f, [this](float val) {
		
		//printf("val: %f \n", val);

		float delayVal = 0.2f;
		if (val <= delayVal) {
			TitleText->meshComp->GetMaterial()->SetFloatUniform("_alpha", val / delayVal);
		}
		else if (val > delayVal && val < 1.0f - delayVal) {
			TitleText->meshComp->GetMaterial()->SetFloatUniform("_alpha", 1.0f);
		}
		else if (val >= 1.0f - delayVal) {
			TitleText->meshComp->GetMaterial()->SetFloatUniform("_alpha", (1.0f - val) / delayVal);
		}

	});
	
	//finalText
	//  126.0f, 157.0f
	timeline->AddActivationClip(DemoText.get(), 126.0f + 1.0f - adjustTime, 126.0f+10.0 + 1.0f - adjustTime);
	timeline->AddActivationClip(MusicText.get(), 126.0f + 10.0 + 1.0f - adjustTime, 126.0f + 20.0 + 1.0f - adjustTime);
	timeline->AddActivationClip(MyCreditText.get(), 126.0f + 20.0 + 1.0f - adjustTime, 126.0f + 30.0 + 1.0f - adjustTime);
	
	timeline->AddAnimationClip(DemoText.get(), 126.0f - adjustTime, 126.0f + 10.0 - adjustTime, [this](float val) {

		//printf("val: %f \n", val);
		DemoText->meshComp->GetMaterial()->SetFloatUniform("_ColorIndex", 1.0f);

		float delayVal = 0.2f;
		if (val <= delayVal) {
			DemoText->meshComp->GetMaterial()->SetFloatUniform("_alpha", val / delayVal);
		}
		else if (val > delayVal && val < 1.0f - delayVal) {
			DemoText->meshComp->GetMaterial()->SetFloatUniform("_alpha", 1.0f);
		}
		else if (val >= 1.0f - delayVal) {
			DemoText->meshComp->GetMaterial()->SetFloatUniform("_alpha", (1.0f - val) / delayVal);
		}

	});

	timeline->AddAnimationClip(MusicText.get(), 126.0f + 10.0 - adjustTime, 126.0f + 20.0 - adjustTime, [this](float val) {

		//printf("val: %f \n", val);
		DemoText->meshComp->GetMaterial()->SetFloatUniform("_ColorIndex", 1.0f);

		float delayVal = 0.2f;
		if (val <= delayVal) {
			MusicText->meshComp->GetMaterial()->SetFloatUniform("_alpha", val / delayVal);
		}
		else if (val > delayVal && val < 1.0f - delayVal) {
			MusicText->meshComp->GetMaterial()->SetFloatUniform("_alpha", 1.0f);
		}
		else if (val >= 1.0f - delayVal) {
			MusicText->meshComp->GetMaterial()->SetFloatUniform("_alpha", (1.0f - val) / delayVal);
		}

	});
	timeline->AddAnimationClip(MyCreditText.get(), 126.0f + 20.0 - adjustTime, 126.0f + 30.0 - adjustTime, [this](float val) {

		//printf("val: %f \n", val);
		DemoText->meshComp->GetMaterial()->SetFloatUniform("_ColorIndex", 1.0f);

		float delayVal = 0.2f;
		if (val <= delayVal) {
			MyCreditText->meshComp->GetMaterial()->SetFloatUniform("_alpha", val / delayVal);
		}
		else if (val > delayVal && val < 1.0f - delayVal) {
			MyCreditText->meshComp->GetMaterial()->SetFloatUniform("_alpha", 1.0f);
		}
		else if (val >= 1.0f - delayVal) {
			MyCreditText->meshComp->GetMaterial()->SetFloatUniform("_alpha", (1.0f - val) / delayVal);
		}

	});
	

	//PostProcess
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 0.0f, 38.0f, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag",0); });
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 38.0, 42.0f, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag",1); });
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 42.0f, 42.3f, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag", 0); });
	
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 42.0f, 62.0f, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag",0); });
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 62.0f,65.0f, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag",2); });
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 65.0f, 66.0f, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag", 0); });
	
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 84.0f, 84.5f, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag",0); });
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 84.5f,85.5f, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag",2); });
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 85.5f, 86.0f, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag",0); });
	
	float adjustGlitch = 0.25f;
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 102.0 - adjustTime, 102.5 - adjustTime, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag", 0); });
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 102.0 - adjustTime, 102.5 - adjustTime, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessVal", 0.0); });
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 102.5 - adjustTime, 105.5 - adjustTime, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag", 5); });
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 102.5 - adjustTime, 105.5 - adjustTime, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessVal", val); });
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 105.5 - adjustTime, 106.5 - adjustTime, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag", 0); });

	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 106.5 - adjustTime - 1.0f, 128.0f - 12.0f - adjustTime, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag",4); });
	
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 128.0f - 12.0f - adjustTime, 126.0f + 1.0f - adjustTime, [this](float val) {
		GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag", 6);
		GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessVal", val);
	});
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 126.0f + 1.0f - adjustTime, 157.0f + 1.0f - adjustTime, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag",3); });
	timeline->AddAnimationClip(GraphicsMain::GetInstance()->renderBoard.get(), 157.0f + 1.0f - adjustTime, 157.5f + 1.0f - adjustTime, [this](float val) {GraphicsMain::GetInstance()->renderBoard->meshComp->GetMaterial()->SetFloatUniform("_postProcessFlag",0); });
		*/
}

