#pragma once

#include "../Direct3D/Direct3D.h"
#include "../Direct3D/Sprite.h"
#include "../Direct3D/Texture.h"
#include "../Direct3D/Camera.h"
#include "../Direct3D/meshX.h"

#include "../DirectSound/DirectSound.h"
#include "../DirectSound/WaveFile.h"
#include "../DirectSound/SoundBuffer.h"

#include "../Template/Singleton.h"

#include "../Entity/GameObject3D.h"

#include "../Scene/Manager/SceneManager.h"


class GameState :public Singleton<GameState>
{
	friend class Singleton<GameState>;

private:	

	SceneManager sceneManager;

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