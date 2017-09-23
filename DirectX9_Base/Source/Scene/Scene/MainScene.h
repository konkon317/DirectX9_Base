#pragma once

#include<vector>

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


#include "../../Direct3D/Effect/EffectLambert.h"
#include "../../Direct3D/Effect/EffectBasic.h"
#include "../../Direct3D//Effect/EffectPhong.h"

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
	EffectLambert effectLambert;
	EffectBasic effectBasic;
	EffectPhong effectPhong;
	EffectPhong effectPhong2;
	EffectPhong effectCookTorrance;

	GameObject3D* pGameObject[2];

	std::vector<Effect* > EffectPointerList;
	int currentEffectIndex;

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