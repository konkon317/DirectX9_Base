
#include <Windows.h>
#pragma comment(lib, "winmm.lib")


#include "Template/Singleton.h"
#include "Application/Application.h"

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application& App = Application::GetInstance();
	
	

	SingletonFinalizer::Finalize();
}