#include "GameState.h"
#include "../Input/directInput.h"
#include "../Input/XInput.h"

void GameState::Draw_Static()
{
	GameState& instance = GameState::GetInstance();

	instance.Draw();
}

void GameState::Update_Static()
{
	GameState& instance = GameState::GetInstance();

	instance.Update();
}

GameState::GameState()
{
	sp.SetSize(100, 100);
	sp.SetPos(500, 500);
	tex.Load("test.bmp");

	tex.SetDivide(2, 2);
	tex.SetUVNum(1, 1);
	
}

GameState::~GameState()
{
	
}


void GameState::Update()
{
	DirectInput &di = DirectInput::GetInstance();
	if (!di.Initialized())
	{
		return;
	}

	XInput& xi = XInput::GetInstance();

	if (xi.GetTrigger(0, TRIGGER_LEFT) > 0)
	{
		int a = 0;
	}

	if (di.KeyDown(DIK_RETURN))
	{
		int i = 0;
	}

	if (di.MouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		int x = di.MousePosition().X();
		int y = di.MousePosition().Y();

		int a = 0;

	}


	spriteRotate += 3.14f * (1.0f / 60.0f);

	sp.SetRotate(spriteRotate);


	static int i = 0;
	i++;
	if (i == 60 * 10)
	{
		BGM_Buffer2.Play(true);
	}


}

void GameState::Draw()
{
	Direct3D::DrawSprite(sp,tex, false);
}

void GameState::Init()
{
	BGM_Wave.Load("Loop_148.wav");
	
	BGM_Buffer.Create(BGM_Wave);
	BGM_Buffer2.Create(BGM_Wave);

	BGM_Buffer.Play(true);
}