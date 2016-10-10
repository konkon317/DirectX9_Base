#include "Application.h"

#include "../Direct3D/Direct3D.h"


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
	updateFunc = NULL;
}

Application::~Application()
{
	
};


bool Application::RegistBasicWindowClass()
{
	
	//-----------
	//ウィンドウクラスの定義と作成
	//----------

	hInstance = GetModuleHandle(NULL);

	//シンプルウィンドウクラス設定
	//WNDCLASSEX wcex =
	//{
	//	sizeof(WNDCLASSEX),						// cbSize : 構造体のサイズ
	//	CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,	// style : ウィンドウスタイル
	//	(WNDPROC)WndProc,							// ipfnWndProc : ウィンドウプロシージャのアドレス
	//	0,										// cbClsExtra : 0固定
	//	0,										// cbWndExtra : 0固定
	//	hInstance,								// hInstance : WinMainのインスタンスハンドル
	//	
	//	(HICON)LoadImage(NULL,					// hIcon : 使用するアイコン
	//	MAKEINTRESOURCE(IDI_APPLICATION),
	//	IMAGE_ICON,
	//	0,
	//	0,
	//	LR_DEFAULTSIZE | LR_SHARED),

	//	(HCURSOR)LoadImage(NULL,					// hCursor :  マウスカーソル
	//	MAKEINTRESOURCE(IDC_ARROW),
	//	IMAGE_CURSOR,
	//	0,
	//	0,
	//	LR_DEFAULTSIZE | LR_SHARED),
	//	(HBRUSH)(COLOR_WINDOW + 1)
	//	/*(HBRUSH)GetStockObject(WHITE_BRUSH)*/,	// hbrBackground : ウィンドウクライアント領域の背景色
	//	NULL,										// lpszMeueName : メニュー
	//	WC_BASIC,									// lpszClassName : ウィンドウクラスの名前 CreateWindow()の第一引数と同じ
	//	NULL										// hIconSm : 使用するアイコン(小)
	//};

	//上のコメントアウトの部分をわかりやすいように整理
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));						//メモリの確保

	wcex.cbSize			=sizeof(WNDCLASSEX);					// cbSize : 構造体のサイズ
	wcex.cbClsExtra		= 0;									// cbClsExtra : 0固定
	wcex.cbWndExtra		= 0;									// cbWndExtra : 0固定
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;	// style : ウィンドウスタイル
	wcex.hInstance		= hInstance;							// hInstance : WinMainのインスタンスハンドル
	wcex.lpfnWndProc	= (WNDPROC)WndProc;						// ipfnWndProc : ウィンドウプロシージャのアドレス
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);	// hbrBackground : ウィンドウクライアント領域の背景色
	wcex.lpszMenuName	= NULL;									// lpszMeueName : メニュー
	wcex.lpszClassName  = WC_BASIC;								// lpszClassName : ウィンドウクラスの名前
	
	//アイコン
	wcex.hIcon = (HICON)LoadImage(NULL, MAKEINTRESOURCE(IDI_APPLICATION), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED);
	wcex.hIconSm =NULL;
	//カーソル
	wcex.hCursor = (HCURSOR)LoadImage(NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED);

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
	const	TCHAR* ClassName,//デフォルトで 変数 WC_BASIC
	DWORD	Style,
	DWORD	ExStyle
	)
{
	//ヘッダーファイルのデフォルト引数をチェック
	
	//クライアント領域サイズを基にしてウィンドウの大きさを決める
	RECT rect;
	rect.top = 0;
	rect.left = 0;
	rect.right = Width;
	rect.bottom = Height;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);

	//ウィンドウの作成
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
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

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
			
			if (updateFunc != NULL)
			{
				updateFunc();//アップデートの実行
			}

			if (ExcessTime == 0)
			{

				Direct3D::Render();//描画の実行
			}

			//フレームレート固定化処理
			endTime = timeGetTime();
			passTime = endTime - startTime;

			if (miriSecondPerFrame >= (passTime + ExcessTime))
			{
				//wait関数内のメッセージループで　WM_QUITを受け取った場合　こちらのメッセージループからも抜ける
				bool continueMessaggeLoop =
					Wait(miriSecondPerFrame - (passTime + ExcessTime));//処理を1/60秒に一回行うことを保つために待つ

				if (continueMessaggeLoop == false)
				{
					break;
				}

				ExcessTime = 0;
			}
			else
			{
				//miriSeconndPerFrameを超えてしまった時間　待たずに次の処理へ　
				//ただし次はコマ落ち（描画処理を行わない）
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
