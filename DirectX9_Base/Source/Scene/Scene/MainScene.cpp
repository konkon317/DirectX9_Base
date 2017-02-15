#include "MainScene.h"

#include "../../Input/directInput.h"
#include "../../Input/XInput.h"



MainScene::MainScene()
{
	sp.SetSize(100, 100);
	sp.SetPos(500, 500);
	tex.Load("test.bmp");

	tex.SetDivide(2, 2);
	tex.SetUVNum(1, 1);

	testMesh.Load(_T("Mesh/iasel/iasel_brackboard.x"));

	camera.SetEyePoint(0.0f, 0.0f, -5);
	camera.SetRelLookAtPoint(0.0f, 0, 1.0f);

	for (int i = 0; i < 2; i++)
	{
		pGameObject[i] = NULL;
		pGameObject[i] = new GameObject3D();

		if (pGameObject != NULL)
		{
			pGameObject[i]->SetMesh(&testMesh);

			pGameObject[i]->SetPosition(i * 5, -i * 1, i*0.3);
		}
	}

	pGameObject[0]->rotateF = false;
	pGameObject[1]->moveF = false;
}

MainScene::~MainScene()
{
	for (int i = 1; i >= 0; i--)
	{
		if (pGameObject[i] != NULL)
		{
			delete pGameObject[i];
			pGameObject[i] = NULL;
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
	float x = di.MousePositionDelta().X();
	x /= 800.0f / 2;
	x *= -1;
	float y = di.MousePositionDelta().Y();
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
	camera.SetViewMatrix();

	for (int i = 1; i >= 0; i--)
	{

		pGameObject[i]->Draw();
	}

	
	static int dir = -1;
	static float targetValue=0;

	sp.SetAlpha(sp.GetAlpha() + (0.01f*dir));
	if (sp.GetAlpha() == targetValue)
	{
		dir *= -1;
		targetValue = (dir == -1) ? 0 : 1;
	}

	Direct3D::GetInstance().SetRenderState(RENDERSTATE::RENDER_ALPHABLEND);
	Direct3D::DrawSprite(sp, tex, false);
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


