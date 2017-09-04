#include "GameObject3D.h"

GameObject3D::GameObject3D()
{
	moveF=true;
	rotateF=true;

	position.x = position.y = position.z;

	rotateYaw_Rad = 0;

	pMesh = nullptr;

	UpdateVectors();	

	obb.SetOffset(0, 0, 0.75);
	obb.SetLength(0.5f, 0.8f, 1.5f);

	obb.UpdateInfo(position, forwardVector, rightVector, upVector);

	pEffect = nullptr;
}

GameObject3D::~GameObject3D()
{
	
}

void GameObject3D::Update()
{
	if (moveF)
	position.x += 0.01f;
	if (rotateF)
	SetRotateYaw(rotateYaw_Rad + D3DX_PI*0.1f);

	obb.UpdateInfo(position, forwardVector, rightVector, upVector);
}

void GameObject3D::Draw()
{

	//DrawLine();

	obb.DrawLine();

	if (pMesh != nullptr)
	{
		D3DXMATRIXA16 mat_transform, mat_scale, mat_rotate;

		D3DXMatrixTranslation(&mat_transform,position.x,position.y,position.z);	//座標
		D3DXMatrixScaling(&mat_scale, 1.0f, 1.0f, 1.0f);		//拡大

		D3DXMatrixRotationY(&mat_rotate, -rotateYaw_Rad + D3DX_PI);	//回転　（y軸中心）		

		pMesh->DrawMatrice(mat_transform, mat_scale, mat_rotate,pEffect);

		//以下実験

	}

}


//正面　上　右方向のベクトルを更新
void GameObject3D::UpdateVectors()
{
	
	float rad =rotateYaw_Rad;
	
	D3DXVECTOR3 v;

	//右
	v.x = cos(rad);
	v.z = sin(rad);
	v.y = 0;
	rightVector = v;

	rad += D3DX_PI / 2;

	//前
	v.x = cos(rad);
	v.z = sin(rad);
	v.y = 0;
	forwardVector = v;

	//上
	v.x = v.z = 0;
	v.y = 1;
	upVector = v;

	obb.UpdateInfo(position, forwardVector, rightVector, upVector);
}

void GameObject3D::DrawLine()
{
	LINE_VERTEX lineVertex[6];
	for (int i = 0; i < 6; i += 2)
	{
		lineVertex[i].v.x = position.x;
		lineVertex[i].v.y = position.y;
		lineVertex[i].v.z = position.z;
	}	

	D3DXVECTOR3 v = position + (forwardVector * 10);
	lineVertex[1].v = v;
	lineVertex[0].color = lineVertex[1].color = 0xFF0000FF;

	v = position + (rightVector * 10);
	lineVertex[3].v = v;
	lineVertex[2].color = lineVertex[3].color = 0xFFFF0000;

	v = position + (upVector * 10);
	lineVertex[5].v = v;
	lineVertex[4].color = lineVertex[5].color = 0xFF00FF00;

	Direct3D::GetInstance().DrawLine(lineVertex, 3);

	/*Direct3D::GetInstance().DrawLine(&lineVertex[0], 1);
	Direct3D::GetInstance().DrawLine(&lineVertex[2], 1);*/
	//Direct3D::GetInstance().DrawLine(&lineVertex[4], 1);
}

