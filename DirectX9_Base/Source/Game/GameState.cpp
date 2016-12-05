#include "GameState.h"
#include "../Input/directInput.h"
#include "../Input/XInput.h"

void GameState::Draw_Static()
{
	GameState& instance = GameState::GetInstance();

	instance.Draw();
}

void GameState::Update_Static()
{
	GameState& instance = GameState::GetInstance();

	instance.Update();
}

GameState::GameState()
{
	sp.SetSize(100, 100);
	sp.SetPos(500, 500);
	tex.Load("test.bmp");

	tex.SetDivide(2, 2);
	tex.SetUVNum(1, 1);
	
	testMesh.Load(_T("sample0012.x"));

	camera.SetEyePoint(0.0f, 1.0f, -5.0f);
	camera.SetRelLookAtPoint(0.0f, 0.0f, 1.0f);
}

GameState::~GameState()
{
	
}


void GameState::Update()
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

	if (xi.GetButtonDown(0,GAMEPAD_DPAD_UP))
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

}

void GameState::Draw()
{

	Direct3D::GetInstance().SetRenderState(RENDER_MESH);
	camera.SetViewMatrix();

	D3DXMATRIXA16 mat_transform, mat_scale, mat_rotate;

	D3DXMatrixTranslation(&mat_transform, 0.0f, 0.0f, 0.0f);	//座標
	D3DXMatrixScaling(&mat_scale, 1.0f, 1.0f, 1.0f);		//拡大
	D3DXMatrixRotationY(&mat_rotate, 0);	//回転　（y軸中心）

	testMesh.DrawMatrice(mat_transform, mat_scale, mat_rotate);

	//Direct3D::DrawSprite(sp, tex, false);
}

void GameState::Init()
{
	//BGM_Wave.Load("Loop_148.wav");
	//
	//BGM_Buffer.Create(BGM_Wave);
	//BGM_Buffer2.Create(BGM_Wave);

	//BGM_Buffer.Play(true);
}