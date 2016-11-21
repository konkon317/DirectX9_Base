#include "XInput.h"

int XInput::GamePadState::ButtonCode[14]=
{
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y
};


//インナークラス　コンストラクタ
XInput::GamePadState::GamePadState()
{
	for (int i = 0; i < 14; i++)
	{
		buttonCount[i] = buttonCountPrev[i] = 0;
	}

	for (int i = 0; i < 2; i++)
	{

		Stick s = { 0.0f, 0.0f };
		stick[i] = s;

		trigger[i] = 0;
	}
}

//インナークラス　更新
void XInput::GamePadState::Update(int PadNum)
{
	//1フレーム前のCounterを保存
	for (int i = 0; i < 16; i++)
	{	
		buttonCountPrev[i] = buttonCount[i];
	}


	//入力状態の取得がうまくいったかどうか
	DWORD funcResult;
	
	//入力状態の取得 (コントローラ番号0-3 , 状態を保存したい構造体のアドレス)
	funcResult = XInputGetState(PadNum,&state);

	if (funcResult == ERROR_SUCCESS)
	{
		//成功

		conected = true;

		//ボタンの更新
		for (int i = 0; i < 14; i++)
		{
			int code = ButtonCode[i];
			if ((state.Gamepad.wButtons & code) != 0)
			{
				buttonCount[i]++;
			}
			else
			{
				buttonCount[i] = 0;
			}
		}

		//ステックの更新
		for (int i = 0; i < 2; i++)
		{
			int x = (i == 0) ? state.Gamepad.sThumbLX : state.Gamepad.sThumbRX;
			int y = (i == 0) ? state.Gamepad.sThumbLY : state.Gamepad.sThumbRY;

			//デッドゾーン内の数値は0へ　ただし　xyのどちらかがデッドゾーンの外ならどちらも変更しない
			if (x<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&x>-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE
				&& y<XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && y>-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
			{
				x = y = 0;
			}


			//Xの入力値を -1~1で
			if (x < 0)
			{
				stick[i].x = (float)x / (float)32768;
			}
			else if (x>0)
			{
				stick[i].x = (float)x / (float)32767;
			}
			else
			{
				//x=0の場合
				stick[i].x = 0;
			}

			//Yの入力値を -1~1で
			if (y < 0)
			{
				stick[i].y = (float)y / (float)32768;
				
			}
			else if (y>0)
			{
				stick[i].y = (float)y/ (float)32767;
			}
			else
			{
				//y=0の場合
				stick[i].y = 0;
			}
		}

		for (int i = 0; i < 2; i++)
		{
			int x = (i == 0) ? state.Gamepad.bLeftTrigger : state.Gamepad.bRightTrigger;

			if (x > 10)//デッドゾーンから外れているか
			{

				trigger[i] = (float)x / (float)255;
			}
			else
			{
				trigger[i] = 0.0f;
			}
		}

	}
	else
	{
		//失敗
		conected = false;

		//コントローラにまったく触っていないのと同じ状態にする

		for (int i = 0; i < 16; i++)
		{
			 buttonCount[i] =0;
		}

		for (int i = 0; i < 2; i++)
		{
			stick[i].x = 0;
			stick[i].y = 0;
		}

		for (int i = 0; i < 2; i++)
		{
			trigger[i] = 0;
		}
	}
}


//インナークラス
//押された瞬間か
bool XInput::GamePadState::GetButtonDown(int buttonNum)
{
	if (buttonNum >= 14 || buttonNum < 0)
	{
		return false;
	}
		
	if (buttonCount[buttonNum] == 1)
	{
		return true;
	}
	else
	{
		return false;
	}

}


//インナークラス
//離された瞬間か
bool XInput::GamePadState::GetButtonRelease(int buttonNum)
{
	if (buttonNum >= 14 || buttonNum < 0)
	{
		return false;
	}

	if (buttonCount[buttonNum] == 0 && buttonCountPrev[buttonNum] >= 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}


//インナークラス
//押され続けたフレーム数
int  XInput::GamePadState::GetButtonCount(int buttonNum)
{
	if (buttonNum >= 14 || buttonNum < 0)
	{
		return 0;
	}
	
	return buttonCount[buttonNum];

}


//インナークラス
//スティックの傾き取得
Stick XInput::GamePadState::GetStick(int stickNum)
{
	if (stickNum >= 0 && stickNum < 2)
	{
		return stick[stickNum];
	}
	else
	{
		Stick s={ 0, 0 };
		return s;
	}
}

//トリガーの状態取得　押されていないと0 完全に押し込まれていると1  中間地を0~1の小数で
float XInput::GamePadState::GetTrigger(int triggerNum)
{
	if (triggerNum >= 0 && triggerNum < 2)
	{
		return trigger[triggerNum];
	}
	else
	{
		
		return 0;
	}
}


//コンストラクタ
XInput::XInput()
{


}

//デストラクタ
XInput::~XInput()
{

}


//全てのコントローラ入力の状態を更新
void XInput::Update()
{
	for (int i = 0; i < 4; i++)
	{
		state[i].Update(i);
	}
}


//ボタンが押されった瞬間か
bool XInput::GetButtonDown(int padNum, int buttonNum)
{
	if (padNum < 0 || padNum >= 4)
	{
		return false;
	}

	return state[padNum].GetButtonDown(buttonNum);
}

//ボタンが離された瞬間か
bool XInput::GetButtonRelease(int padNum, int buttonNum)
{
	if (padNum < 0 || padNum >= 4)
	{
		return false;
	}
	return state[padNum].GetButtonDown(buttonNum);
}

//ボタンが押され続けたフレーム数
int XInput::GetButtonCount(int padNum, int buttonNum)
{
	if (padNum < 0 || padNum >= 4)
	{
		return 0;
	}
	return state[padNum].GetButtonDown(buttonNum);
}

//スティックの傾き
Stick XInput::GetStick(int padNum, int stickNum)
{
	if (padNum < 0 || padNum >= 4)
	{
		Stick s={ 0, 0 };

		return s;
	}

	return state[padNum].GetStick(stickNum);
}

//トリガーの状態
float XInput::GetTrigger(int padNum, int triggerNum)
{
	if (padNum < 0 || padNum >= 4)
	{
		return 0;
	}

	return state[padNum].GetTrigger(triggerNum);
}
