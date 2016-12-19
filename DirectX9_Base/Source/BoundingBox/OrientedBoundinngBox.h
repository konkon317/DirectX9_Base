#pragma once
#include "../Direct3D/Direct3D.h"

//éQçl
//http://marupeke296.com/COL_3D_No13_OBBvsOBB.html


class OrientedBoundingBox
{
	//OBB

protected:
	D3DXVECTOR3 position;//íÜêS
	D3DXVECTOR3 NormDirection[3];

	FLOAT length[3];

	FLOAT offSetForward;
	FLOAT offSetUp;
	FLOAT offSetRight;

	D3DXVECTOR3 vertexpos[8];

public:
	OrientedBoundingBox();
	virtual ~OrientedBoundingBox();

	const D3DXVECTOR3* GetDirection_p(){ return NormDirection; }
	const FLOAT* GetLen_p(){ return length; }
	D3DXVECTOR3 GetPos(){ return position; }


	void UpdateInfo(D3DXVECTOR3 newPos, D3DXVECTOR3 newFor, D3DXVECTOR3 newRight, D3DXVECTOR3 newUp);
	
	void SetOffset(float forward, float right, float up);
	void SetLength(float forward, float right, float up);


	void DrawLine();

private:
	static float LengthSegmentOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3 = 0);

public :


	static bool Collision(OrientedBoundingBox& obb1, OrientedBoundingBox& obb2);
};