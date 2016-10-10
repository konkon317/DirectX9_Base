#include "direct3d.h"

//コンストラクタ
Direct3D::Direct3D()
{
	pD3D9 = NULL;
	pDevice3D = NULL;
	isDeviceCreated = false;
}

//デストラクタ
Direct3D::~Direct3D()
{
	pDevice3D->Release();
	pD3D9->Release();
}

bool Direct3D::TryCreate(HWND hWnd)
{
	if (isDeviceCreated == false)//作られていない場合のみ
	{
		isDeviceCreated = Create(hWnd);
	}

	return isDeviceCreated;
}

bool Direct3D::Create(HWND hWnd)
{
	RECT rec;
	GetClientRect(hWnd,&rec);
	int Width=rec.right-rec.left;
	int Height=rec.bottom-rec.top;

	//Direct3D9オブジェクトの作成
	pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	//ディスプレイ情報取得
	D3DDISPLAYMODE Display;
	pD3D9->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);

	//スワップチェイン設定
	//スワップチェイン　バックバッファをフロントバッファに書きかえる
	//DISCARDはスワップチェインの方法をDIrectXに任せるという設定
	D3DPRESENT_PARAMETERS D3DParam =
	{
		Width, Height, Display.Format, 1, D3DMULTISAMPLE_NONE, 0,
		D3DSWAPEFFECT_DISCARD, hWnd, TRUE, TRUE, D3DFMT_D24S8, 0, 0, D3DPRESENT_INTERVAL_DEFAULT
	};

	//幾つかの設定でデバイス作成を試みる
	//HALモードで3Dデバイス作成
	if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
	{
		if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DParam.hDeviceWindow,
			D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
		{
			//Refモードで3Dデバイス作成
			if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
				D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
			{
				if (FAILED(pD3D9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, D3DParam.hDeviceWindow,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED, &D3DParam, &pDevice3D)))
				{
					// 3Dデバイス作成失敗
					pD3D9->Release();
					return false;
				}
			}
		}
	}
	return true;
}

//描画の反映時に実行
void Direct3D::Render()
{
	if (Singleton<Direct3D>::IsInstatnceCreated())
	{
		Direct3D & d3d = Direct3D::GetInstance();

		if (d3d.IsDeviceCreated())
		{
			DWORD ClearColor =/* (pathed) ? 0x11111111 :*/ 0xFFFF0000;//背景クリア色

			//背景クリア
			d3d.pDevice3D->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, ClearColor, 1.0f, 0);

			//ここに関数ポインタで描画処理を行う様にする



			d3d.pDevice3D->EndScene();
		
			d3d.pDevice3D->Present(NULL, NULL, NULL, NULL);
		}
		else
		{
			MessageBox(NULL, "デバイスが作成されていないので実行できません", TEXT("Direct3D Render"), MB_OK);
		}
	}
	else
	{
		MessageBox(NULL, "インスタンスが作成されていないので実行できません", TEXT("Direct3D Render"), MB_OK);
	}
}
