#include "GameState.h"

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
	spriteRotate += 3.14f * (1.0f / 60.0f);

	sp.SetRotate(spriteRotate);
}

void GameState::Draw()
{
	Direct3D::DrawSprite(sp,tex, false);
}