
#include <Windows.h>
#pragma comment(lib, "winmm.lib")


#include "Template/Singleton.h"
#include "Application/Application.h"

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application& App = Application::GetInstance();
	
	App.RegistBasicWindowClass();

	App.CreateWind(_T("ウィンドウタイトル"), 1024, 780);

	App.MessageLoop();

	SingletonFinalizer::Finalize();
}