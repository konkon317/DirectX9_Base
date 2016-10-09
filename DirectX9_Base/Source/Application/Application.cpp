#include "Application.h"


//ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


//Applicationクラス　静的変数の初期化
//インスタンスハンドル
HINSTANCE Application::hInstance = NULL;

const DWORD  Application::FPS = 60;
const DWORD  Application::miriSecondPerFrame = 1000 / FPS;

DWORD  Application::ExcessTime = 0;
bool   Application::pathed = false;

//Applicationクラス関数定義

Application::Application()
{
	//コンストラクタ

	char szBuff[256];
	wsprintf(szBuff, "Application");
	std::string st = szBuff;
	MessageBox(NULL, st.c_str(), TEXT("作成"), MB_OK);
}

Application::~Application()
{
	//デストラクタ //overload

	char szBuff[256];
	wsprintf(szBuff, "開放Application");
	std::string st = szBuff;
	MessageBox(NULL, st.c_str(), TEXT("開放"), MB_OK);
};


bool Application::RegistBasicWindowClass()
{
	
	//-----------
	//ウィンドウクラスの定義と作成
	//----------

	hInstance = GetModuleHandle(NULL);

	//シンプルウィンドウクラス設定
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						// cbSize : 構造体のサイズ
		CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,	// style : ウィンドウスタイル
		(WNDPROC)WndProc,							// ipfnWndProc : ウィンドウプロシージャのアドレス
		0,										// cbClsExtra : 0固定
		0,										// cbWndExtra : 0固定
		hInstance,								// hInstance : WinMainのインスタンスハンドル
		
		(HICON)LoadImage(NULL,					// hIcon : 使用するアイコン
		MAKEINTRESOURCE(IDI_APPLICATION),
		IMAGE_ICON,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED),

		(HCURSOR)LoadImage(NULL,				// hCursor :  マウスカーソル
		MAKEINTRESOURCE(IDC_ARROW),
		IMAGE_CURSOR,
		0,
		0,
		LR_DEFAULTSIZE | LR_SHARED),
		(HBRUSH)(COLOR_WINDOW + 1)
		/*(HBRUSH)GetStockObject(WHITE_BRUSH)*/,	// hbrBackground : ウィンドウクライアント領域の背景色
		NULL,										// lpszMeueName : メニュー
		WC_BASIC,									// lpszClassName : ウィンドウクラスの名前 CreateWindow()の第一引数と同じ
		NULL										// hIconSm : 使用するアイコン(小)
	};

	//シンプルウィンドウクラス作成
	if (!RegisterClassEx(&wcex))
	{
		return false;
	}

	return true;

}

bool Application::CreateWind(
	const TCHAR	*Title,
	int		Width,
	int		Height,
	int		X,
	int		Y,
	HWND	hParentWnd,
	HMENU	hMenu,
	const	TCHAR* ClassName,
	DWORD	Style,
	DWORD	ExStyle
	)
{


	//--
	//ウィンドウの作成
	//--

	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = Width;
	rect.bottom = Height;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);

	hWnd = CreateWindowEx(
		ExStyle,
		ClassName,
		Title,
		Style,
		X,
		Y,
		rect.right - rect.left,
		rect.bottom - rect.top,
		hParentWnd,
		hMenu,
		hInstance,
		NULL
		);

	//ウィンドウ作成失敗
	if (hWnd == NULL)
	{
		return false;
	}


	//// ウインドウ表示 //0623追加
	//ShowWindow(hWnd, SW_SHOWNORMAL);
	//UpdateWindow(hWnd);

	return true;
}

void Application::MessageLoop()
{
	DWORD startTime, endTime, passTime;
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//メッセージ処理
			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
		else
		{
			//メッセージ処理をしていないときにする処理

			startTime = timeGetTime();//フレームの実行時間計測用	


			//ここに処理を書く




			//フレームレート固定化処理
			endTime = timeGetTime();
			passTime = endTime - startTime;

			if (miriSecondPerFrame >= (passTime + ExcessTime))
			{
				bool continueMessaggeLoop =
					Wait(miriSecondPerFrame - (passTime + ExcessTime));

				if (continueMessaggeLoop == false)
				{
					break;
				}

				ExcessTime = 0;
			}
			else
			{
				//miriSeconndPerFrameを超えてしまった時間
				//次はコマ落ち
				ExcessTime = (passTime + ExcessTime) - miriSecondPerFrame;
			}
		}
	}
}


//フレームレート固定処理の際の待ち
//戻り値はメッセージループを継続するかどうか
//WM_QUITの時だけfalseを返す
bool Application::Wait(DWORD wait_time)
{
	MSG msg = {};
	DWORD startTime = timeGetTime();

	do
	{
		if (msg.message == WM_QUIT)return false;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//メッセージ処理
			TranslateMessage(&msg);
			DispatchMessage(&msg);	// アプリケーションの各ウィンドウプロシージャにメッセージを転送する
		}
		if (wait_time > 0)Sleep(1);

	} while (timeGetTime() < startTime + wait_time);

	return true;
}
