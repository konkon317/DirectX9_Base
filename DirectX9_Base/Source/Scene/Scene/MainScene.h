#pragma once


#include "../../Direct3D/Direct3D.h"
#include "../../Direct3D/Sprite.h"
#include "../../Direct3D/Texture.h"
#include "../../Direct3D/Camera.h"
#include "../../Direct3D/mesh.h"

#include "../../DirectSound/DirectSound.h"
#include "../../DirectSound/WaveFile.h"
#include "../../DirectSound/SoundBuffer.h"

#include "../../Entity/GameObject3D.h"

#include "../Manager/SceneManager.h"

class MainScene :public Scene
{
private:

	Texture tex;
	Sprite sp;

	float spriteRotate;

	WaveFile BGM_Wave;
	SoundBuffer BGM_Buffer;
	SoundBuffer BGM_Buffer2;

	Camera camera;

	Mesh testMesh;

	GameObject3D* pGameObject[2];

public:
	MainScene();
	virtual ~MainScene();

	virtual void Update();// {};
	virtual void Draw();// {};
	virtual void Init();// {};
};

class MainSceneCreater : public SceneCreater
{
private:
	static MainSceneCreater instance;

	//protected:
	MainSceneCreater();

public:
	~MainSceneCreater(){};
	Scene* CreateScene();
};