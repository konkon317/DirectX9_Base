#pragma once

#include "../Direct3D/Direct3D.h"
#include "../Direct3D/Sprite.h"
#include "../Direct3D/Texture.h"

#include "../Template/Singleton.h"

class GameState :public Singleton<GameState>
{
	friend class Singleton<GameState>;
private:
	Texture tex;
	Sprite sp;

	float spriteRotate;


public :
	static void Draw_Static();
	static void Update_Static();

private:
	GameState();
	~GameState();

	void Update();
	void Draw();
};