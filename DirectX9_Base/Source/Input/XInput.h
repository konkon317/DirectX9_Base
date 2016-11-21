#pragma once

#include "../Template/Singleton.h"

#include <Xinput.h>
#pragma comment(lib,"xinput.lib")

//ボタンと配列　添え字の関係
#define GAMEPAD_DPAD_UP			0
#define GAMEPAD_DPAD_DOWN		1
#define GAMEPAD_DPAD_LEFT		2
#define GAMEPAD_DPAD_RIGHT		3	
#define GAMEPAD_START			4
#define GAMEPAD_BACK			5
#define GAMEPAD_LEFT_THUMB		6
#define GAMEPAD_RIGHT_THUMB		7	
#define GAMEPAD_LEFT_SHOULDER	8
#define GAMEPAD_RIGHT_SHOULDER	9	
#define GAMEPAD_A				10	
#define GAMEPAD_B				11
#define GAMEPAD_X				12
#define GAMEPAD_Y				13

#define STICK_LEFT		0
#define STICK_RIGHT		1

#define TRIGGER_LEFT	0
#define TRIGGER_RIGHT	1


//スティックの傾き　-1~1
struct  Stick
{
	float x;
	float y;
};


//4つのコントローラ入力をまとめて管理
class XInput :public Singleton<XInput>
{

	//インナークラス 
	//ひとつのコントローラの入力状態
	//privateで宣言しているので
	//このクラス外からは一切アクセスできない
	class  GamePadState
	{
		//ボタン番号とマスクの関係
		static int ButtonCode[14];

		//それぞれのボタンの押され続けたフレーム数
		int buttonCount[14];
		int buttonCountPrev[14];

		//コントローラが接続されているか
		bool conected;

		//トリガーボタンの入力状態 0~1
		float trigger[2];

		//スティックの傾き
		Stick stick[2];		

		//入力状態　加工前
		XINPUT_STATE state;

	public:
		GamePadState();
		~GamePadState();

		//状態の更新
		void Update(int PadNum);

		//ボタンが押された瞬間
		bool GetButtonDown(int buttonNum);

		//ボタンが離された瞬間
		bool GetButtonRelease(int ButtunNum);

		//ボタンが押され続けたフレーム数
		int GetButtonCount(int ButtonNum);

		//トリガーの入力状態
		float GetTrigger(int TriggerNum);

		//コントローラの接続
		bool Conected(){ return conected; }

		//スティックの傾き状態
		Stick GetStick(int stickNum);

	};


	//親クラスからnew できるようにする
	friend class Singleton<XInput>;

private:

	//コントローラの入力状態
	//コントローラの数だけ
	GamePadState state[4];

public :

	//状態の更新
	void Update();


private:

	XInput();
	~XInput();
	

public:

	//ボタンが押された瞬間
	bool GetButtonDown(int padNum, int buttonNum);
	//ボタンが離された瞬間
	bool GetButtonRelease(int padNum, int buttonNum);

	//ボタンが押され続けたフレーム数
	int GetButtonCount(int padNum, int buttonNum);

	//トリガーの入力状態
	float GetTrigger(int padNum,int TriggerNum);

	//スティックの傾き状態
	Stick GetStick(int padNum,int stickNum);

};

