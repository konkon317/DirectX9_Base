#include "Application.h"

#include "../Direct3D/Direct3D.h"

#include "../Input/directInput.h"
#include "../Input/XInput.h"


//ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	//メッセージ処理のための関数
	//DispatchMessage によって呼び出される

	//ウィンドウクラス登録時に各ウィンドウに設定される
	//アプリケーションがメッセージを取得したら呼び出されるようにする

	//メッセージの例
	//ウィンドウの右上の×が押された WM_DESTROY
	//ウィンドウの移動 WM_MOVE
	//ウィンドウサイズの変更 WM_SIZE
	//等


	//特別な処理を必要とするメッセージは　とりあえず終了処理だけ書いておく
	switch (msg)
	{
	case WM_DESTROY: //右上のバツが押された、AltF4が押された時などに受信するメッセージ
		PostQuitMessage(0);//メッセージキューにWM_QUITメッセージを送る
		break;

	}

	//特別な処理を要さないメッセージはデフォルトのウィンドウプロシージャが処理する
	return DefWindowProc(hWnd, msg, wParam, lParam);//移動とかサイズ変更とか
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
	updateFunc = nullptr;
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
	WNDCLASSEX wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));						//メモリの確保

	wcex.cbSize			=sizeof(WNDCLASSEX);					// cbSize : 構造体のサイズ
	wcex.cbClsExtra		= 0;									// cbClsExtra : 0固定
	wcex.cbWndExtra		= 0;									// cbWndExtra : 0固定
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;	// style : ウィンドウスタイル
	wcex.hInstance		= hInstance;							// hInstance : WinMainのインスタンスハンドル
	wcex.lpfnWndProc	= (WNDPROC)WndProc;						// ipfnWndProc : ウィンドウプロシージャのアドレス　関数ポインタ
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


	//ウィンドウクラススタイル http://www-higashi.ist.osaka-u.ac.jp/~k-maeda/vcpp/sec1-3wnddetail.html
	/*　CS_HREDRAW	：	横サイズが変わったとき，ウインドウ全体を再描画する
		CS_VREDRAW	：	縦サイズが変わったとき，ウインドウ全体を再描画する
		CS_DBLCLKS	：	ダブルクリックが発生したことをウインドウに通知する
		CS_NOCLOSE	：	システムメニューの［閉じる］コマンドを使用不可にする
		CS_CLASSDC	：	このクラスのウインドウ同士で１つのデバイスコンテキストを共有する
		CS_OWNDC	：	ウインドウ毎にデバイスコンテキストを１つづつ持つ
		CS_PARENTDC	：	親ウインドウのデバイスコンテキストを使用する
		CS_GLOBALCLASS	：	このクラスを作成したアプリケーションが実行されていれば， 他のアプリケーションからでもこのクラスを利用できるようになる． 一般的にDLLでカスタムコントロールを作成するときに使うらしい．*/
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

	//指定したクライアント領域を保持する大きさのウィンドウサイズを計算
	AdjustWindowRect(&rect, Style, TRUE);

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

	//hWnd　ウィンドウハンドル　複数のウィンドウを区別するためのもの

	//ウィンドウ作成失敗
	if (hWnd == NULL)
	{
		return false;
	}


	//// ウインドウ表示 //0623追加
	ShowWindow(hWnd, SW_SHOWNORMAL);//ウィンドウをアクティブにして表示
	UpdateWindow(hWnd);				//クライアント領域の更新

	return true;

	//CreateWindowExの　スタイル詳細 http://www-higashi.ist.osaka-u.ac.jp/~k-maeda/vcpp/sec1-3wnddetail.html
	/*　
	WS_OVERLAPPED	：	タイトルバー，境界線つきオーバーラップウインドウ
	WS_OVERLAPPEDWINDOW = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIMZEBOX
	WS_CHILD	：	子ウインドウ．WS_POPUPとは併用できない
	WS_POPUP	：	ポップアップウインドウ．WS_CHILDとは併用できない
	WS_POPUPWINDOW = WS_BORDER | WS_POPUP | WS_SYSMENU
	WS_CLIPCHILDREN	：	親の描画で子ウインドウの部分をクリッピング
	WS_CLIPSIBLINGS	：	子の描画で兄弟ウインドウの部分をクリッピング
	WS_CAPTION	：	タイトルバー(WS_DLGFRAMEも一緒についてくる)
	WS_SYSMENU	：	タイトルバーにシステムメニュー(閉じるボタンとアイコン)つける
	WS_MINIMIZEBOX	：	システムメニューに最小化ボタン加える
	WS_MAXIMIZEBOX	：	システムメニューに最大化ボタン加える
	WS_BORDER	：	細い境界線
	WS_DLGFRAME	：	サイズ変更できない境界線
	WS_THICKFRAME	：	サイズ変更できる境界線
	WS_HSCROLL	：	horizon scroll，つまり水平方向スクロール
	WS_VSCROLL	：	vertical scroll，つまり垂直方向スクロール
	WS_VISIBLE	：	初期状態で表示される
	WS_DISABLED	：	初期状態が使用不可
	WS_MAXIMIZE	：	初期状態で最大化
	WS_MINIMIZE	：	初期状態で最小化
	WS_TABSTOP	：	ダイアログのコントロールにつかう．Tabでフォーカスを当てられるようにする．
	WS_GROUP	：	主にダイアログのラジオボタンのグループ設定に使う．
	*/
}

void Application::MessageLoop()
{


	DWORD startTime, endTime, passTime;
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))//プログラムにmessageが送られているか調べる 
													 //messageキューからmsgにメッセージを書き込み
													 //メッセージキューからメッセージを削除する(PM_REMOVEが設定されているとき)
		{
			//メッセージ処理
			TranslateMessage(&msg); //仮想キーメッセージを文字メッセージに変換し、それをメッセージキューにポストする
			DispatchMessage(&msg);	//メッセージの割り当て　ウィンドウプロシージャでメッセージを処理する
		}
		else
		{
			//メッセージ処理をしていないときにする処理 (メッセージキューに何も入っていないとき)

			startTime = timeGetTime();//フレームの実行時間計測用


			//入力 key and mouse
			DirectInput& di = DirectInput::GetInstance();
			if (di.Initialized() == false)
			{
				di.Init(hWnd);
			}

			//入力ゲームパッド
			XInput& xi = XInput::GetInstance();
			xi.Update();
						
			//入力のアップデート
			di.Update();

			
			if (updateFunc != nullptr)
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
