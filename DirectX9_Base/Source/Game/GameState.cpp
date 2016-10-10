#include "GameState.h"


GameState::GameState()
{
	sp.SetSize(100, 100);
	sp.SetPos(500, 500);
	tex.Load("font.bmp");

	tex.SetDivide(2, 2);
	tex.SetUVNum(1, 1);
	
	
}

GameState::~GameState()
{
	
}

void GameState::Draw_Static()
{
	GameState& instance = GameState::GetInstance();

	instance.Draw();	
}

void GameState::Draw()
{
	Direct3D::DrawSprite(sp,tex, false);
}