#pragma once

#include "../Direct3D/Direct3D.h"
#include "../Direct3D/Sprite.h"
#include "../Direct3D/Texture.h"

#include "../DirectSound/DirectSound.h"
#include "../DirectSound/WaveFile.h"
#include "../DirectSound/SoundBuffer.h"

#include "../Template/Singleton.h"

class GameState :public Singleton<GameState>
{
	friend class Singleton<GameState>;
private:
	Texture tex;
	Sprite sp;

	float spriteRotate;


	WaveFile BGM_Wave;
	SoundBuffer BGM_Buffer;
	SoundBuffer BGM_Buffer2;

public :
	static void Draw_Static();
	static void Update_Static();

	void Init();

private:
	GameState();
	~GameState();

	void Update();
	void Draw();
};