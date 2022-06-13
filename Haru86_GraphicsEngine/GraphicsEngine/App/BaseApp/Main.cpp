#include <Windows.h>
//#include <iostream>
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"

//#include "Assets/App/CollisionArt/CollisionArt.h"
//#include "Assets/App/KuraArtApp/KuraArtApp.h"
//#include "Assets/App/DemoApp/DemoApp.h"
//#include "Assets/App/Kura02/Kura02.h"
#include "Assets/App/AnimationLearning/AnimationLearning.h"

int WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
//int main(int argc, char** argv) 
{
	GraphicsMain::Create();
	
	if (GraphicsMain::GetInstance()->CreateApp()) {
		do
		{
			if (!GraphicsMain::GetInstance()->Initialize(new AnimationLearning()))return 0;
			if (!GraphicsMain::GetInstance()->RunLoop())return 0;
		} while (GraphicsMain::GetInstance()->isRestart);
	}

	GraphicsMain::GetInstance()->ShutDown();
	GraphicsMain::Destroy();

	return 0;
}