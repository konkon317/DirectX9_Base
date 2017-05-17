#include "GameState.h"
#include "../Input/directInput.h"
#include "../Input/XInput.h"

#include "../Scene/Scene/MainScene.h"

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

	if (di.KeyDown(DIK_R))
	{
		sceneManager.ChangeScene(typeid(MainScene).hash_code());
	}
	if (di.KeyDown(DIK_T))
	{
		sceneManager.ChangeScene(typeid(TestScene).hash_code());
	}


	sceneManager.Update();
}

void GameState::Draw()
{
	sceneManager.Draw();
	
}

void GameState::Init()
{
	sceneManager.ChangeScene(typeid(MainScene).hash_code());

	//BGM_Wave.Load("Loop_148.wav");
	//
	//BGM_Buffer.Create(BGM_Wave);
	//BGM_Buffer2.Create(BGM_Wave);

	//BGM_Buffer.Play(true);
}