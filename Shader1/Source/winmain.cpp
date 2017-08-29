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
#include "Direct3D/meshX.h"

#if _DEBUG
#define new ::new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif

void DummyDraw();
void DummyUpdate();

//関数ポインタの引数の関係でDummyUpdateとDummyDrawに引数を持たせられん
//グローバル変数にせざるを得ない
//誠に遺憾
MeshX mesh;
D3DXMATRIXA16 meshTransformMatrix;


int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//ウィンドウやメインループの実行を管理するクラスのインスタンスを作製
	Application& App = Application::GetInstance();
	
	App.RegistBasicWindowClass();							//ウィンドウクラスの登録
	App.CreateWind(_T("ウィンドウタイトル"), 1024, 780);	//登録したウィンドウクラスを基にウィンドウを作成

	//デバイスを管理するクラスのインスタンスを作成
	Direct3D& d3d = Direct3D::GetInstance();

	//描画デバイスの作成
	//Applicationで作成したウィンドウのハンドルから、描画領域の設定を行う
	if (d3d.TryCreate(App.GetWindowHandle()))
	{
		MessageBox(NULL, "3Dデバイス作成成功", TEXT("WinMain"), MB_OK);
	}
	else
	{
		MessageBox(NULL, "3Dデバイス作成失敗", TEXT("WinMain"), MB_OK);
	}

	//プロジェクション行列の設定
	d3d.SetupRrojectionMatrix();

	mesh.Load(_T("Mesh/iasel/iasel_brackboard.x"));
	D3DXMatrixIdentity(&meshTransformMatrix);	
		
	App.SetUpdateFuncPointer(DummyUpdate);
	d3d.SetDrawFunc(DummyDraw);	

	App.MessageLoop();//メッセージループ

	//メッセージループが終了したので終了処理をしてプログラムを終える
	
	SingletonFinalizer::Finalize();//シングルトンなクラスの終了処理

}


void DummyUpdate()
{
	//ビュー行列の設定
	Direct3D& d3d = Direct3D::GetInstance();

	static float angle = 0;
	angle += 0.1;
	D3DXMATRIXA16 view;
	D3DXVECTOR3 eye(cosf(angle) * 10, 0, sinf(angle) * 10);
	static D3DXVECTOR3 lookat(0, 0, 0);
	static D3DXVECTOR3 up(0, 1, 0);
	D3DXMatrixLookAtLH(&view, &eye, &lookat, &up);
	d3d.SetViewMatrix(view);

}

void DummyDraw()
{
	D3DXMATRIXA16 scale, rotate;
	D3DXMatrixScaling(&scale, 1, 1, 1);
	D3DXMatrixIdentity(&rotate);

	Direct3D& d3d = Direct3D::GetInstance();
	d3d.SetRenderState(RENDERSTATE::RENDER_MESH);
	mesh.DrawMatrice(meshTransformMatrix, scale, rotate);
}
