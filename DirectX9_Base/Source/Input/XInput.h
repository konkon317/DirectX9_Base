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


struct  Stick
{
	float x;
	float y;
};

class XInput :public Singleton<XInput>
{

	//インナークラス
	class  GamePadState
	{
		static int ButtonCode[14];

		int buttonCount[14];
		int buttonCountPrev[14];

		bool conected;

		float trigger[2];

		Stick stick[2];		

		XINPUT_STATE state;

	public:
		GamePadState();
		void Update(int PadNum);

		bool GetButtonDown(int buttonNum);
		bool GetButtonRelease(int ButtunNum);
		int GetButtonCount(int ButtonNum);
		float GetTrigger(int TriggerNum);

		bool Conected(){ return conected; }

		Stick GetStick(int stickNum);

	};

	friend class Singleton<XInput>;

private:

	GamePadState state[4];

public :

	void Update();


private:

	XInput();
	~XInput();
	

public:

	bool GetButtonDown(int padNum, int buttonNum);
	bool GetButtonRelease(int padNum, int buttonNum);
	int GetButtonCount(int padNum, int buttonNum);
	float GetTrigger(int padNum,int TriggerNum);

	Stick GetStick(int padNum,int stickNum);

};

