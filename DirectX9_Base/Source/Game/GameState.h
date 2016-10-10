#pragma once

#include "../Direct3D/Direct3D.h"
#include "../Direct3D/Sprite.h"
#include "../Direct3D/Texture.h"

#include "../Template/Singleton.h"

class GameState :public Singleton<GameState>
{
	friend class Singleton<GameState>;
public :
	static void Draw_Static();

private:
	GameState();
	~GameState();

	Texture tex;
	Sprite sp;

	void Draw();
};