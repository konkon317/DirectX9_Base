#pragma once

#include<vector>

#include "../../Direct3D/Direct3D.h"
#include "../../Direct3D/Sprite.h"
#include "../../Direct3D/Texture.h"
#include "../../Direct3D/Camera.h"
#include "../../Direct3D/meshX.h"
#include "../../Direct3D/MeshXWithHeight.h"

#include "../../DirectSound/DirectSound.h"
#include "../../DirectSound/WaveFile.h"
#include "../../DirectSound/SoundBuffer.h"

#include "../../Entity/GameObject3D.h"

#include "../../Model3D/Model.h"

#include "../Manager/SceneManager.h"

#include "../../Direct3D/ShadowMapTexture.h"
#include "../../Direct3D/Light.h"

#include "../../Direct3D/Effect/EffectLambert.h"
#include "../../Direct3D/Effect/EffectBasic.h"
#include "../../Direct3D//Effect/EffectPhong.h"
#include "../../Direct3D//Effect/EffectPhongAndNormal.h"
#include "../../Direct3D/Effect/ProjectedTextureShadow.h"

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

	MeshXWithHeight testMesh;
	MeshX mapMesh;

	Model model;
	EffectLambert effectLambert;
	EffectBasic effectBasic;
	EffectPhong effectPhong;
	EffectPhong effectPhong2;
	EffectPhong effectCookTorrance;
	EffectPhongAndNormal  effectPhongAndNormal;
	EffectProjectedTextureShadow effectProjectedShadow;

	ShadowMapTexture shadowTexture;

	GameObject3D* pGameObject[3];

	std::vector<Effect* > EffectPointerList;
	int currentEffectIndex;

	Light light;

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