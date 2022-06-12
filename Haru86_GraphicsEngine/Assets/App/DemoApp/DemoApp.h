#pragma once
#include "../../../GraphicsEngine/GraphicsMain/GraphicsMain.h"
#include "../../../GraphicsEngine/App/BaseApp/BaseApp.h"
#include "../../../GraphicsEngine/Graphics/Mesh.h"
#include <vector>

class DemoApp
	: public BaseApp
{
public:
	DemoApp();
	~DemoApp();
	void Start()  override;
	void Update()  override;
	void Draw() override;
	void Timeline(class TimelineObject* timelineObj)  override;
private:
	//audio
	std::shared_ptr<class AudioObject> DemoAudioObj;

	//ray
	std::shared_ptr<class GameObject> Scene1_Frag;
	std::shared_ptr<class GameObject> Scene2_Frag;
	std::shared_ptr<class GameObject> Scene3_Frag;
	std::shared_ptr<class GameObject> Scene4_Frag;
	std::shared_ptr<class GameObject> Scene5_Frag;

	//font
	std::shared_ptr<class GameObject> DemoText;
	std::shared_ptr<class GameObject> MusicText;
	std::shared_ptr<class GameObject> MyCreditText;
	std::shared_ptr<class GameObject> TitleText;
};

