#include "MainScene.h"

#include "../../Input/directInput.h"
#include "../../Input/XInput.h"

#include "../../FBX/fbxUtil.h"




MainScene::MainScene()
{
	

	tex.Load("test.bmp");

	tex.SetDivide(2, 2);
	tex.SetUVNum(1, 1);

	effectLambert.CreateFromFile("Shader/lambert.fx");
	effectBasic.CreateFromFile("Shader/BasicShader.fx");
	effectPhong.CreateFromFile("Shader/phong.fx");
	effectPhong2.CreateFromFile("Shader/phong2.fx");
	effectCookTorrance.CreateFromFile("Shader/CookTorrance.fx");
	effectPhongAndNormal.CreateFromFile("Shader/bumpmap.fx");
	effectProjectedShadow.CreateFromFile("Shader/ProjectedTextureShadow.fx");
	effectPriorityBufferShadow.CreateFromFile("Shader/DepthBufferShadow.fx");
	//effectPriorityBufferShadow.CreateFromFile("Shader/PriorityBufferShadow.fx");

	EffectPointerList.push_back(&effectLambert);
	EffectPointerList.push_back(&effectBasic);
	EffectPointerList.push_back(&effectPhong);
	EffectPointerList.push_back(&effectPhong2);
	EffectPointerList.push_back(&effectCookTorrance);
	EffectPointerList.push_back(&effectPhongAndNormal);

	currentEffectIndex=0;

	testMesh.Load(_T("Mesh/pumpkin/pumpkin.x"));
	mapMesh.Load(_T("Mesh/map/map.x"));

	if (SUCCEEDED(shadowTexture.Create(4096)))
	{
		int a = 0;
	}

	camera.SetEyePoint(0.0f, 5.0f, -5);
	camera.SetRelLookAtPoint(0.0f, 0, 1.0f);

	for (int i = 0; i < 3; i++)
	{
		pGameObject[i] = nullptr;
		pGameObject[i] = new GameObject3D();

		if (pGameObject != nullptr)
		{
			pGameObject[i]->SetMesh(&testMesh);

			pGameObject[i]->SetPosition(i * 5.0f, i * 1.0f, i*0.3f);

			if (EffectPointerList.size() >= 0)
			{
				Effect* pEffect = EffectPointerList[currentEffectIndex];
				//pGameObject[i]->SetEffectFile(pEffect);
				pGameObject[i]->SetEffectFile(&effectPriorityBufferShadow);
			}
		}
	}



	pGameObject[0]->rotateF = false;
	pGameObject[1]->moveF = false;


	FbxUtil::ReadModelFromFbx(model, "FBX_FILES/cube4.fbx");

	D3DXVECTOR4 lightDir(-120,- 60,-40, 1);
	/*lightDir *= 0.5;
	lightDir.w = 1;*/
	light.Init(lightDir, 0);

	model.Debug_TestShow();

	sp.SetSize(256, 256);
	sp.SetPos(128, 128);
	
}

MainScene::~MainScene()
{
	for (int i = 2; i >= 0; i--)
	{
		if (pGameObject[i] != nullptr)
		{
			delete pGameObject[i];
			pGameObject[i] = nullptr;
		}
	}
}

void MainScene::Update()
{
	DirectInput &di = DirectInput::GetInstance();
	if (!di.Initialized())
	{
		return;
	}

	XInput& xi = XInput::GetInstance();

	if (xi.GetTrigger(0, TRIGGER_LEFT) > 0)
	{
		int a = 0;
	}

	if (xi.GetButtonDown(0, GAMEPAD_DPAD_UP))
	{
		int a = 0;
	}

	if (di.KeyDown(DIK_RETURN))
	{
		int i = 0;
	}

	if (di.MouseButtonDown(MOUSE_BUTTON_RIGHT))
	{
		int x = di.MousePosition().X();
		int y = di.MousePosition().Y();

		int a = 0;
	}

	Effect* pEffect = nullptr;

	if (di.KeyDown(DIK_P))
	{
		if (EffectPointerList.size() > 0)
		{
			currentEffectIndex++;
			if (currentEffectIndex >= EffectPointerList.size())
			{
				currentEffectIndex = 0;
			}
		}
		pEffect= EffectPointerList[currentEffectIndex];
	}
	if (di.KeyDown(DIK_O))
	{
		if (EffectPointerList.size() > 0)
		{
			if (currentEffectIndex != 0)//unsigned なので0の状態で減算するとプラスの最大値になってしまう
			{
				currentEffectIndex--;
			}
			else
			{
				currentEffectIndex = EffectPointerList.size()-1;
			}
		}
		pEffect = EffectPointerList[currentEffectIndex];
	}



	if (pEffect != nullptr)
	{
		for (int i = 0; i < 3; i++)
		{
			if (pGameObject != nullptr)
			{
				pGameObject[i]->SetEffectFile(pEffect);
			}
		}
	}



	float f = 0.0f;
	float r = 0.0f;

	if (di.KeyState(DIK_W))
	{
		f += 1;
	}
	if (di.KeyState(DIK_S))
	{
		f -= 1;
	}
	if (di.KeyState(DIK_D))
	{
		r += 1;
	}
	if (di.KeyState(DIK_A))
	{
		r -= 1;
	}
	camera.Move(f, r);

	float x = static_cast<float>(di.MousePositionDelta().X());
	x /= 800.0f / 3.5f;
	x *= -1;
	float y = static_cast<float>(di.MousePositionDelta().Y());
	y /= 600.0f / 2.0f;
	y *= -1;

	camera.Rotate(x, y);

	/*spriteRotate += 3.14f * (1.0f / 60.0f);

	sp.SetRotate(spriteRotate);*/
	for (int i = 1; i >= 0; i--)
	{

		pGameObject[i]->Update();
	}


	if (OrientedBoundingBox::Collision(*(pGameObject[0]->GetObb()), *(pGameObject[1]->GetObb())))
	{
		int a = 0;
		//	MessageBox(NULL, "ヒット", TEXT("バウディングボックス"), MB_OK);
	}
}

void MainScene::Draw()
{
	Direct3D& d3d = Direct3D::GetInstance();
	//d3d.SetRenderState(RENDER_SHADOW_MAP);
	//d3d.SetupRrojectionMatrix();

	shadowTexture.SetRenderTarget();

	{
		D3DXMATRIXA16 projmat;
		D3DXMatrixPerspectiveFovLH(&projmat, D3DX_PI *(0.3), 1, 80.0f, 200.0f);
		D3DXMatrixOrthoLH(&projmat, 100, 100, 50, 200);
		d3d.SetProjectionMatrix(projmat);
		effectProjectedShadow.SetLightProj(projmat);

		effectPriorityBufferShadow.SetLightProj(projmat);
	}


	{
		D3DXVECTOR4 lightDir = light.GetDir();
		D3DXVECTOR3  eye = D3DXVECTOR3(-lightDir.x, -lightDir.y, -lightDir.z);
		D3DXVECTOR3 lookat = D3DXVECTOR3(0, -10, 0);
		D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);

		D3DXMATRIXA16 view;
		D3DXMatrixLookAtLH(&view, &eye, &lookat, &up);
		d3d.SetViewMatrix(view);

		effectProjectedShadow.SetLightView(view);

		effectPriorityBufferShadow.SetLightView(view);
		effectPriorityBufferShadow.SetWorldLightDir(lightDir);
	}
	
	D3DXVECTOR4 id;
	id = D3DXVECTOR4(0, 0, 0, 0);



	for (int i = 2; i >= 0; i--)
	{
		id.x += 0.1f;
		effectPriorityBufferShadow.SetVectorId(id);
		

		pGameObject[i]->Draw(0);
	}

	{

		D3DXMATRIXA16 trans, scale, matidentity;
		D3DXMatrixIdentity(&matidentity);
		D3DXMatrixTranslation(&trans, 10, -10, 10);
		D3DXMatrixScaling(&scale, 10, 10, 10);
		D3DXVECTOR4 lightPos = -light.GetDir();
		lightPos.w = lightPos.w*-1;
		effectProjectedShadow.SetVectorLightPos(lightPos);
		effectProjectedShadow.SetShadowMap(shadowTexture.ShadowTex());

		id.x = 0.5f;
		id.y = 0.5;
		id.z = 0.5;
		id.w = 1.0;
		effectPriorityBufferShadow.SetVectorId(id);

		mapMesh.DrawMatrice(trans, scale, matidentity, &effectPriorityBufferShadow,0);
	}
	

	//通常の描画
	d3d.SetRenderState(RENDER_MESH);

	d3d.ChangeDepthStencilSurfac_Default();
	d3d.ChangeRenderTarget_Default();
	d3d.ChangeViewPort_Default();

	d3d.Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L);

	d3d.SetupRrojectionMatrix();
	camera.SetViewMatrix();

	id = D3DXVECTOR4(0, 0, 0, 0);


	D3DXVECTOR4 color = D3DXVECTOR4(1, 1, 1, 1);
	effectPriorityBufferShadow.SetVectorColor(color);

	effectPriorityBufferShadow.SetTextureIdMap(shadowTexture.ShadowTex());

	for (int i = 2; i >= 0; i--)
	{
		id.x += 0.1f;
		effectPriorityBufferShadow.SetVectorId(id);

		pGameObject[i]->Draw(1);
	}

	{
		
		D3DXMATRIXA16 trans, scale, matidentity;
		D3DXMatrixIdentity(&matidentity);
		D3DXMatrixTranslation(&trans, 10,-10,10);
		D3DXMatrixScaling(&scale, 10, 10, 10);
		D3DXVECTOR4 lightPos = -light.GetDir();
		lightPos.w = lightPos.w*-1;
		effectProjectedShadow.SetVectorLightPos(lightPos);
		effectProjectedShadow.SetShadowMap(shadowTexture.ShadowTex());

		id.x = 0.5f;
		id.y = 0.5;
		id.y = 0.5;
		id.w = 1.0;
		effectPriorityBufferShadow.SetVectorId(id);

		mapMesh.DrawMatrice(trans, scale, matidentity,&effectPriorityBufferShadow,1);
	}

	d3d.SetRenderState(RENDER_DEFAULT);
	d3d.DrawSprite(sp,shadowTexture.ShadowTex());
}

void MainScene::Init()
{

}


MainSceneCreater  MainSceneCreater::instance;

MainSceneCreater::MainSceneCreater()
{
	SetTargetSceneHash(typeid(MainScene).hash_code());

	if (doneHashSet == true)
	{
		SceneManager::RegisterSceneCreater(this);
	}
}

Scene* MainSceneCreater::CreateScene()
{
	Scene* pScene = new MainScene();
	return pScene;
}


