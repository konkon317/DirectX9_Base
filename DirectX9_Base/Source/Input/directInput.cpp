#include "directInput.h"

//コンストラクタ
DirectInput::DirectInput()
{
	pDInput			= NULL;
	pKeyDevice		= NULL;
	pMouseDevice	= NULL;

	initialized = false;

	//構造体の初期化 全てを0で初期化
	ZeroMemory(&keyInput,sizeof(int)*INPUT_BUFFER_SIZE);
	ZeroMemory(&keyInputPreview, sizeof(int)*INPUT_BUFFER_SIZE);
	ZeroMemory(&mouseInput, sizeof(DIMOUSESTATE2));
	ZeroMemory(&mouseInputPreview, sizeof(DIMOUSESTATE2));

	for (int i = 0; i < 8; i++)
	{
		mouseButtonCounter[i] = 0;
	}

	mousePos.Set(0, 0);
	mousePosDelta.Set(0, 0);
	mousePosPreview.Set(0, 0);

}

//デストラクタ
DirectInput::~DirectInput()
{
	Release();
}

//初期化
HRESULT DirectInput::Init(HWND hwnd)
{
	hWnd = hwnd;

	//DirectInputオブジェクトの作成
	HRESULT hresult = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, NULL);
	if (FAILED(hresult))
	{
		return hresult;
	}

	//キーボード初期化処理
	hresult = InitKey();
	if (FAILED(hresult))
	{
		return hresult;
	}

	//マウス初期化
	hresult = InitMouse();
	if (FAILED(hresult))
	{
		return hresult;
	}

	initialized = true;

	return S_OK;
	
}

//キー配列初期化
HRESULT DirectInput::InitKey()
{
	HRESULT hresult;

	//デバイスオブジェクトの作成
	hresult = pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, NULL);	
	if (FAILED(hresult))
	{
		return hresult;
	}

	//データフォーマット設定
	hresult = pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hresult))
	{
		return hresult;
	}

	//協調モードを設定
	hresult = pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(hresult))
	{
		return hresult;
	}

	//入力デバイスへのアクセス権を取得
	pKeyDevice->Acquire();

	return S_OK;
}

//マウス状態を管理するクラスインスタンスを初期化
HRESULT DirectInput::InitMouse()
{
	HRESULT hresult;

	hresult = pDInput->CreateDevice(GUID_SysMouse, &pMouseDevice, NULL);
	//デバイスオブジェクトの作成
	if (FAILED(hresult))
	{
		return hresult;
	}

	//データフォーマットを作成
	hresult = pMouseDevice->SetDataFormat(&c_dfDIMouse2);
	if (FAILED(hresult))
	{
		return hresult;
	}
	//協調モードを設定
	hresult = pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(hresult))
	{
		return hresult;
	}

	//軸モードを設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	//相対
	//diprop.dwData = DIPROPAXISMODE_ABS;//絶対

	hresult = pMouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(hresult))
	{
		return hresult;
	}

	pMouseDevice->Acquire();//入力デバイスへのアクセス権を取得

	RECT rec;
	GetClientRect(hWnd, &rec);
	int Width = rec.right - rec.left;
	int Height = rec.bottom - rec.top;

	POINT localPos;
	localPos.x = Width / 2;
	localPos.y = Height / 2;
	ClientToScreen(hWnd, &localPos);
	SetCursorPos(localPos.x,localPos.y);
	mousePos.Set(localPos.x, localPos.y);
	mousePosPreview.Set(localPos.x, localPos.y);
	mousePosDelta.Set(0, 0);

	return S_OK;
}


//アップデート
HRESULT DirectInput::Update()
{
	HRESULT hresult = pKeyDevice->Acquire();

	if ((hresult == DI_OK) || (hresult == S_FALSE))
	{
		//前回のキー入力情報を退避
		memcpy(keyInputPreview, keyInput, sizeof(int)*INPUT_BUFFER_SIZE);

		//状態ワーク
		BYTE tKeyInputWork[INPUT_BUFFER_SIZE];

		pKeyDevice->GetDeviceState(sizeof(BYTE)*INPUT_BUFFER_SIZE, &tKeyInputWork);

		for (int i = 0; i < INPUT_BUFFER_SIZE; i++)
		{
			if (tKeyInputWork[i])
			{
				if (keyInput[i] < 2147483647)//int 最大値まで
				{
					keyInput[i]++;
				}				
			}
			else
			{
				keyInput[i] = 0;
			}
		}
	}

	//マウスのボタン状態を取得
	hresult = pMouseDevice->Acquire();
	if (hresult == DI_OK || hresult == S_FALSE)
	{
		//前回のマウス状態を退避
		memcpy(&mouseInputPreview, &mouseInput, sizeof(DIMOUSESTATE2));

		pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseInput);
	
		for (int i = 0; i < 8; i++)
		{
			if (mouseInput.rgbButtons[i])
			{
				if (mouseButtonCounter[i] < 2147483647)//int 最大値まで
				{
					mouseButtonCounter[i]++;
				}
			}
			else
			{
				mouseButtonCounter[i] = 0;
			}
		}
	}

	//スクリーン中　マウス座標取得
	POINT worldPos;
	GetCursorPos(&worldPos);//ワールド座標を取得
	POINT localPos = worldPos;
	ScreenToClient(hWnd,&localPos);//ワールド座標をローカル座標に変換

	mousePosPreview = mousePos;
	mousePos.Set(localPos.x, localPos.y);
	mousePosDelta = mousePos-mousePosPreview;

	



	return S_OK;
}

//解放処理
HRESULT  DirectInput::Release()
{
	int a = 0;
	if (pMouseDevice)
	{
		pMouseDevice->Unacquire();
		pMouseDevice->Release();
		pMouseDevice = NULL;
	}
	
	if (pKeyDevice)
	{
		pKeyDevice->Unacquire();
		pKeyDevice->Release();
		pKeyDevice = NULL;
	}

	if (pDInput)
	{
		pDInput->Release();
		pDInput = NULL;
	}
	
	return S_OK;
}

//キーが押されているならtrue
bool DirectInput::KeyState(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		if (keyInput[code] > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

//位置フレーム前のキー状態
bool DirectInput::KeyStatePreview(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		if (keyInputPreview[code] > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

//キーが押された瞬間化
bool DirectInput::KeyDown(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		return  (keyInput[code] >= 1) && (keyInputPreview[code] == 0);
	}
	return false;
}

//キーが離された瞬間化
bool DirectInput::KeyUp(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		return   (keyInput[code] == 0) && (keyInputPreview[code] >= 1);
	}
	return false;
}
//キーが押され続けているフレーム数
int DirectInput::KeyCount(int code)
{
	if (code >= 0 && code < INPUT_BUFFER_SIZE)
	{
		return keyInput[code];
	}
	return 0;
}

//マウスのボタン状態
bool DirectInput::MouseButton(int num)
{ 
	if (num >= 0 && num < 8)
	{
		return (mouseInput.rgbButtons[num] >= 1);
	}
	else
	{
		return false;
	}
}

//マウスのボタンが押された瞬間か
bool DirectInput::MouseButtonDown(int num)
{
	if (num >= 0 && num < 8)
	{
		return (mouseInput.rgbButtons[num] >= 1) && (mouseInputPreview.rgbButtons[num] == 0);
	}
	else
	{
		return false;
	}
}

//マウスボタンが離された瞬間か
bool DirectInput::MouseButtonUP(int num)
{
	if (num >= 0 && num < 8)
	{
		return (mouseInput.rgbButtons[num] ==0) && (mouseInputPreview.rgbButtons[num] >=1);
	}
	else
	{
		return false;
	}
}

//マウスボタンが押され続けているフレーム数
int DirectInput::MouseButtonCount(int num)
{
	if (num >= 0 && num < 8)
	{
		return mouseButtonCounter[num];
	}
	else
	{
		return 0;
	}
}