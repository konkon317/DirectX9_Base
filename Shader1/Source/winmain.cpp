#if _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#endif

#include <Windows.h>
#pragma comment(lib, "winmm.lib")

#include "Template/Singleton.h"
#include "Application/Application.h"
#include "Direct3D/Direct3D.h"
#include "Direct3D/Effect.h"

#include "Direct3D/meshX.h"

#if _DEBUG
#define new ::new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif

MeshX mesh;

Effect effect;


void Dummy()
{
	Direct3D& d3d = Direct3D::GetInstance();

	static float f;
	D3DXMATRIXA16 View;
	f += 0.025f;
	D3DXMatrixLookAtLH(&View, &D3DXVECTOR3(30 * sin(f), 20, -30 * cos(f)), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
	d3d.SetViewMatrix(View);
}

void DummyDraw()
{
	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);
	D3DXMATRIXA16 scale;
	D3DXMatrixIdentity(&scale);
	D3DXMatrixScaling(&scale, 10, 10, 10);

	mesh.DrawMatrice(mat, scale, mat, &effect);
}

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	/*
	fbxsdk::FbxManager* fbxManager = fbxsdk::FbxManager::Create();

	fbxManager->Destroy();
	*/	

	//ウィンドウやメインループの実行を管理するクラスのインスタンスを作製
	Application& App = Application::GetInstance();
	
	App.RegistBasicWindowClass();							//ウィンドウクラスの登録
	App.CreateWind(_T("ウィンドウタイトル"), 1024, 780);	//登録したウィンドウクラスを基にウィンドウを作成

	//デバイスを管理するクラスのインスタンスを作成
	Direct3D& D3d = Direct3D::GetInstance();

	//描画デバイスの作成
	//Applicationで作成したウィンドウのハンドルから、描画領域の設定を行う
	if (D3d.TryCreate(App.GetWindowHandle()))
	{
		//MessageBox(NULL, "3Dデバイス作成成功", TEXT("WinMain"), MB_OK);
	}
	else
	{
		MessageBox(NULL, "3Dデバイス作成失敗", TEXT("WinMain"), MB_OK);
	}
	
	D3d.SetupRrojectionMatrix();

	HRESULT result = effect.CreateFromFile("Shader/Empty.fx");

	mesh.Load(_T("Mesh/iasel/iasel_brackboard.x"));
	

	App.SetUpdateFuncPointer(Dummy);
	D3d.SetDrawFunc(DummyDraw);	

	App.MessageLoop();//メッセージループ

	//メッセージループが終了したので終了処理をしてプログラムを終える
	

	
	SingletonFinalizer::Finalize();//シングルトンなクラスの終了処理

	

}

