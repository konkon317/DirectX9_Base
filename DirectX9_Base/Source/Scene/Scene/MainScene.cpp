#include "MainScene.h"

#include "../../Input/directInput.h"
#include "../../Input/XInput.h"

#include "../../FBX/fbxUtil.h"




MainScene::MainScene()
{
	sp.SetSize(100, 100);
	sp.SetPos(500, 500);
	tex.Load("test.bmp");

	tex.SetDivide(2, 2);
	tex.SetUVNum(1, 1);

	effectLambert.CreateFromFile("Shader/lambert.fx");
	effectBasic.CreateFromFile("Shader/BasicShader.fx");
	effectPhong.CreateFromFile("Shader/phong.fx");
	effectPhong2.CreateFromFile("Shader/phong2.fx");
	effectCookTorrance.CreateFromFile("Shader/CookTorrance.fx");
	effectPhongAndNormal.CreateFromFile("Shader/ProjectedTextureShadow.fx");

	EffectPointerList.push_back(&effectLambert);
	EffectPointerList.push_back(&effectBasic);
	EffectPointerList.push_back(&effectPhong);
	EffectPointerList.push_back(&effectPhong2);
	EffectPointerList.push_back(&effectCookTorrance);
	EffectPointerList.push_back(&effectPhongAndNormal);

	currentEffectIndex=0;

	testMesh.Load(_T("Mesh/pumpkin/pumpkin.x"));
	mapMesh.Load(_T("Mesh/map/map.x"));

	if (SUCCEEDED(shadowTexture.Create(256)))
	{
		int a = 0;
	}

	camera.SetEyePoint(0.0f, 0.0f, -5);
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
				pGameObject[i]->SetEffectFile(pEffect);
			}
		}
	}

	pGameObject[0]->rotateF = false;
	pGameObject[1]->moveF = false;


	FbxUtil::ReadModelFromFbx(model, "FBX_FILES/cube4.fbx");

	model.Debug_TestShow();
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
			currentEffectIndex--;
			if (currentEffectIndex <0)
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
	x /= 800.0f / 3.5;
	x *= -1;
	float y = static_cast<float>(di.MousePositionDelta().Y());
	y /= 600.0f / 2;
	y *= -1;

	camera.Rotate(x, y);

	spriteRotate += 3.14f * (1.0f / 60.0f);

	sp.SetRotate(spriteRotate);
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
	Direct3D::GetInstance().SetRenderState(RENDER_MESH);
	Direct3D::GetInstance().SetupRrojectionMatrix();
	camera.SetViewMatrix();

	{
		D3DXMATRIXA16 trans, scale, matidentity;
		D3DXMatrixIdentity(&matidentity);
		D3DXMatrixTranslation(&trans, 0, -10, 0);
		D3DXMatrixScaling(&scale, 5, 5, 5);
		mapMesh.DrawMatrice(trans, scale, matidentity, &effectPhong2);
	}

	for (int i = 2; i >= 0; i--)
	{

		pGameObject[i]->Draw();
	}

	
	static int dir = -1;
	static float targetValue=0;

	sp.SetAlpha(sp.GetAlpha() + (0.01f*dir));
	if (sp.GetAlpha() == targetValue)
	{
		dir *= -1;
		targetValue = (dir == -1) ? 0.0f : 1.0f;
	}

	static float f = 0.0f;

	f += 0.01f;
	static D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);
	//model.Draw(mat);
	mat._41 = f;

	Direct3D::GetInstance().SetRenderState(RENDERSTATE::RENDER_ALPHABLEND);
	//Direct3D::DrawSprite(sp, tex, false);
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


