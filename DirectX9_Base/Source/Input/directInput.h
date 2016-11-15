#pragma once 

#define D3D_DEBUG_INFO	//Direct3Dデバッグフラグ

#define DIRECTINPUT_VERSION 0x0800

#define MOUSE_BUTTON_LEFT 0
#define MOUSE_BUTTON_RIGHT 1
#define MOUSE_BUTTON_MIDDLE 2


#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dinput8.lib")

#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>
#include <dinput.h>

#include "../other/MyVector.h"
#include "../Template/Singleton.h"

#define INPUT_BUFFER_SIZE 256


class DirectInput :public Singleton<DirectInput>
{
	friend class Singleton<DirectInput>;
private:

	LPDIRECTINPUT8		 pDInput;			//directinput オブジェクト
	LPDIRECTINPUTDEVICE8 pKeyDevice;		//キーボード用デバイス
	LPDIRECTINPUTDEVICE8 pMouseDevice;		//マウス用デバイス

	//キーボード情報
	int keyInput[INPUT_BUFFER_SIZE];
	int keyInputPreview[INPUT_BUFFER_SIZE];
	

	//マウス情報
	DIMOUSESTATE2 mouseInput;
	DIMOUSESTATE2 mouseInputPreview;
	int mouseButtonCounter[8];

	Vector2 mousePos;
	Vector2 mousePosPreview;
	Vector2 mousePosDelta;
	
	HWND		  hWnd;

	bool initialized;

private:

	HRESULT InitKey();//キーボードの初期化
	HRESULT InitMouse();

	//コンストラクタデストラクタ
	DirectInput();
	~DirectInput();

public:


	//初期化処理
	HRESULT Init(HWND hWnd);

	//キー状態の更新処理
	HRESULT Update();

	//解放処理
	HRESULT Release();	

	bool Initialized(){ return initialized; }

	//値取得関数

	bool KeyState(int code);
	bool KeyStatePreview(int code);
	bool KeyDown(int code);
	bool KeyUp(int code);
	int KeyCount(int code);

	DIMOUSESTATE2 MouseState(){ return  mouseInput; }
	DIMOUSESTATE2 MoouseStatePreview(){ return mouseInputPreview; }

	Vector2 MousePosition(){ return mousePos; }
	Vector2 MousePositionPreview(){ return mousePosPreview; }
	Vector2 MousePositionDelta(){ return mousePosDelta; }

	bool MouseButton(int num);
	bool MouseButtonDown(int num);
	bool MouseButtonUP(int num);
	int MouseButtonCount(int num);
};
