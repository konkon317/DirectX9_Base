#pragma once


#include "../../Direct3D/Direct3D.h"
#include "../../Direct3D/Sprite.h"
#include "../../Direct3D/Texture.h"
#include "../../Direct3D/Camera.h"
#include "../../Direct3D/meshX.h"

#include "../../DirectSound/DirectSound.h"
#include "../../DirectSound/WaveFile.h"
#include "../../DirectSound/SoundBuffer.h"

#include "../../Entity/GameObject3D.h"

#include "../../Model3D/Model.h"

#include "../Manager/SceneManager.h"


#include "../../Direct3D/Effect/Effect.h"



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

	MeshX testMesh;

	Model model;
	Effect effect;

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