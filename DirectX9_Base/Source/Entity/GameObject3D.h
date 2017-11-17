#pragma once

#include "GameObject.h"
#include "../Direct3D/meshX.h"
#include "../BoundingBox/OrientedBoundinngBox.h"
#include "../Direct3D/Effect/Effect.h"

class GameObject3D : public GameObject
{ 
protected:

	//座標
	D3DXVECTOR3 position;

	//各方向のベクトル
	D3DXVECTOR3 forwardVector;
	D3DXVECTOR3 rightVector;
	D3DXVECTOR3 upVector;

	//y軸を中心とした回転度
	//その他の軸は今のところ使用しない
	float rotateYaw_Rad;

	Effect* pEffect;

	//メッシュクラスへのポインタ
	MeshX* pMesh;

	OrientedBoundingBox obb;

public :

	bool moveF;
	bool rotateF;

	GameObject3D();

	virtual void SetEffectFile(Effect* pEffect) { this->pEffect = pEffect; }

	virtual ~GameObject3D();

	virtual void Update();

	virtual void Draw(UINT pass=0);

	void DrawLine();

	void SetMesh(MeshX* pMesh_){ if (pMesh_ != nullptr)pMesh = pMesh_; }

	//正面　上　右方向のベクトルを更新
	void UpdateVectors();

	//y軸を中心とした回転をセット
	void SetRotateYaw(float yawRad){ rotateYaw_Rad = yawRad; UpdateVectors(); };

	//座標を設定
	void SetPosition(float x, float y, float z){ position.x = x; position.y = y; position.z = 0; }

	//回転度取得
	float GetRotateYaw_rad(){ return rotateYaw_Rad; }

	//座標取得
	D3DXVECTOR3 GetPosition(){ return position; }

	//バウンディンングボックス取得
	OrientedBoundingBox* GetObb(){ return &obb; }

	//正面方向へのベクトル
	D3DXVECTOR3 GetForwardVector()
	{ 
		return forwardVector;
	}

	//右方向へのベクトル
	D3DXVECTOR3 GetRightVector()
	{		
		return rightVector;
	}

	//上方向へのベクトル
	D3DXVECTOR3 GetUpVector()
	{	
		return upVector;
	}
};