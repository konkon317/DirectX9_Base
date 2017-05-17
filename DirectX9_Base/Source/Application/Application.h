#pragma once

#include <Windows.h>

#include <tchar.h>
#include <windowsx.h>
#include <commctrl.h>
#include <WinUser.h>

#pragma comment (lib,"comctl32.lib")
#pragma comment (lib,"winmm.lib")

#include "../Template/Singleton.h"


static const TCHAR * WC_BASIC = _T("Basic");	//基本ウィンドウクラス名

class Application :public Singleton<Application>
{
	friend class Singleton<Application>;

public:
	typedef void(*FuncPointer)();//関数ポインタの型定義

//変数
private:
	static HINSTANCE hInstance;	//インスタンスハンドル
	HWND hWnd;					//ウィンドウハンドル

	static const DWORD FPS;
	static const DWORD miriSecondPerFrame;
	static DWORD ExcessTime;
	static bool pathed;

	FuncPointer updateFunc;

//関数
public :

	static bool RegistBasicWindowClass();		//ウィンドウクラスの定義と作成

	bool CreateWind(							//　戻り値　true 成功   false 作成失敗
		const TCHAR	*Title,						//　タイトル
		int		Width = CW_USEDEFAULT,			//　幅
		int		Height = CW_USEDEFAULT,			//　高さ
		int		X = CW_USEDEFAULT,				//　X座標
		int		Y = CW_USEDEFAULT,				//　Y座標
		HWND	hParentWnd = NULL,				//　親ウィンドウハンドル
		HMENU	hMenu = NULL,					//	メニューハンドル　もしくはリソースID
		const	TCHAR* ClassName = WC_BASIC,	//	ウィンドウクラス名
		DWORD	Style = (WS_OVERLAPPEDWINDOW^WS_THICKFRAME^WS_MAXIMIZEBOX) | WS_CLIPCHILDREN | WS_VISIBLE,		//	ウィンドウスタイル
		DWORD	ExStyle = 0						//	ウィンドウ拡張スタイル
		);

	HWND GetWindowHandle(){ return hWnd; }

	void MessageLoop();
	bool Wait(DWORD wait_time);

	void SetUpdateFuncPointer(FuncPointer fp){ updateFunc = fp; };
	
protected:



private:
	Application();		//コンストラクタ
	~Application();		//デストラクタのオーバーロード




};
