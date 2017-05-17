
#include "OrientedBoundinngBox.h"



//F forward <--->B back
//R right   <--->L left
//U up      <--->D down

#define VERTEX_FRU 0
#define VERTEX_FLU 1
#define VERTEX_BRU 2
#define VERTEX_BLU 3
#define VERTEX_FRD 4
#define VERTEX_FLD 5
#define VERTEX_BRD 6
#define VERTEX_BLD 7

#define FORWARD 0
#define RIGHT   1
#define UP      2

OrientedBoundingBox::OrientedBoundingBox()
{
	position.x = position.y = position.z;

	for (int i = 0; i < 3; i++)
	{
		length[i] = 0;

		NormDirection[i].x = 0;
		NormDirection[i].y = 0;
		NormDirection[i].z = 0;
	}

	for (int i = 0; i < 8; i++)
	{
		vertexpos[i].x = 0;
		vertexpos[i].y = 0;
		vertexpos[i].z = 0;
	}

	offSetForward = offSetRight = offSetUp = 0;
}

OrientedBoundingBox::~OrientedBoundingBox()
{

}

void OrientedBoundingBox::UpdateInfo(D3DXVECTOR3 newPos, D3DXVECTOR3 newFor, D3DXVECTOR3 newRight, D3DXVECTOR3 newUp)
{
	D3DXVECTOR3 v = newPos;
	v+= newFor*offSetForward;
	v += newRight*offSetRight;
	v+= newUp*offSetUp;

	D3DXVec3Normalize(&NormDirection[FORWARD],&newFor);
	D3DXVec3Normalize(&NormDirection[RIGHT], &newRight);
	D3DXVec3Normalize(&NormDirection[UP], &newUp);

	position = v;


	//頂点座標
	//上半分
	vertexpos[VERTEX_FRU] = position + (NormDirection[FORWARD] * 0.5*length[FORWARD]) + 
		(NormDirection[RIGHT] * 0.5*length[RIGHT]) + 
		(NormDirection[UP] * 0.5*length[UP]);

	vertexpos[VERTEX_FLU] = position + (NormDirection[FORWARD] * 0.5*length[FORWARD]) +
		(NormDirection[RIGHT] * -0.5*length[RIGHT]) +
		(NormDirection[UP] * 0.5*length[UP]);

	vertexpos[VERTEX_BRU] = position + (NormDirection[FORWARD] * -0.5*length[FORWARD]) +
		(NormDirection[RIGHT] * 0.5*length[RIGHT]) +
		(NormDirection[UP] * 0.5*length[UP]);

	vertexpos[VERTEX_BLU] = position + (NormDirection[FORWARD] *- 0.5*length[FORWARD]) +
		(NormDirection[RIGHT] * -0.5*length[RIGHT]) +
		(NormDirection[UP] * 0.5*length[UP]);

	//下半分

	vertexpos[VERTEX_FRD] = position + (NormDirection[FORWARD] * 0.5*length[FORWARD]) +
		(NormDirection[RIGHT] * 0.5*length[RIGHT]) +
		(NormDirection[UP] * -0.5*length[UP]);

	vertexpos[VERTEX_FLD] = position + (NormDirection[FORWARD] * 0.5*length[FORWARD]) +
		(NormDirection[RIGHT] * -0.5*length[RIGHT]) +
		(NormDirection[UP] *- 0.5*length[UP]);

	vertexpos[VERTEX_BRD] = position + (NormDirection[FORWARD] * -0.5*length[FORWARD]) +
		(NormDirection[RIGHT] * 0.5*length[RIGHT]) +
		(NormDirection[UP] * -0.5*length[UP]);

	vertexpos[VERTEX_BLD] = position + (NormDirection[FORWARD] * -0.5*length[FORWARD]) +
		(NormDirection[RIGHT] * -0.5*length[RIGHT]) +
		(NormDirection[UP] * -0.5*length[UP]);
	
}

void OrientedBoundingBox::SetOffset(float forward, float right, float up)
{
	offSetForward =forward;
	offSetUp =up;
	offSetRight =right;
}
void OrientedBoundingBox::SetLength(float forward, float right, float up)
{
	length[0] = forward;
	length[1] = right;
	length[2] = up;

}

void OrientedBoundingBox::DrawLine()
{
	LINE_VERTEX lineVertex[24];
	for (int i = 0; i < 24; i ++)
	{
		lineVertex[i].v.x = 0;
		lineVertex[i].v.y =0;
		lineVertex[i].v.z =0;
		lineVertex[i].color=0xFF000000;
	}

	//上面の辺
	lineVertex[0].v = vertexpos[VERTEX_FLU];
	lineVertex[1].v = vertexpos[VERTEX_FRU];
	
	lineVertex[2].v = vertexpos[VERTEX_FRU];
	lineVertex[3].v = vertexpos[VERTEX_BRU];

	lineVertex[4].v = vertexpos[VERTEX_BRU];
	lineVertex[5].v = vertexpos[VERTEX_BLU];

	lineVertex[6].v = vertexpos[VERTEX_BLU];
	lineVertex[7].v = vertexpos[VERTEX_FLU];

	//下面の辺

	lineVertex[8].v = vertexpos[VERTEX_FLD];
	lineVertex[9].v = vertexpos[VERTEX_FRD];

	lineVertex[10].v = vertexpos[VERTEX_FRD];
	lineVertex[11].v = vertexpos[VERTEX_BRD];

	lineVertex[12].v = vertexpos[VERTEX_BRD];
	lineVertex[13].v = vertexpos[VERTEX_BLD];

	lineVertex[14].v = vertexpos[VERTEX_BLD];
	lineVertex[15].v = vertexpos[VERTEX_FLD];

	//柱

	lineVertex[16].v = vertexpos[VERTEX_FLU];
	lineVertex[17].v = vertexpos[VERTEX_FLD];

	lineVertex[18].v = vertexpos[VERTEX_FRU];
	lineVertex[19].v = vertexpos[VERTEX_FRD];

	lineVertex[20].v = vertexpos[VERTEX_BLU];
	lineVertex[21].v = vertexpos[VERTEX_BLD];

	lineVertex[22].v = vertexpos[VERTEX_BRU];
	lineVertex[23].v = vertexpos[VERTEX_BRD];


	Direct3D::GetInstance().DrawLine(lineVertex, 12);


}


bool OrientedBoundingBox::Collision(OrientedBoundingBox& obb1, OrientedBoundingBox& obb2)
{
	//参考
	//http://marupeke296.com/COL_3D_No13_OBBvsOBB.html

	D3DXVECTOR3 AE[3];
	D3DXVECTOR3 BE[3];

	D3DXVECTOR3 Interval = obb1.position - obb2.position;//obbの中心点同士を結ぶ線分を計算
	
	for (int i = 0; i < 3;i++)
	{
		AE[i] = obb1.NormDirection[i] * (obb1.length[i]/2.0f);
		BE[i] = obb2.NormDirection[i] * (obb2.length[i]/2.0f);
	}

	FLOAT rA, rB, L;

	//方向軸を分離軸に obb1

	//分離軸 obb1.NormDirection[0] : obb1の正面方向のベクトルに平行な軸にobb2を投影
	rA = obb1.length[0] / 2;
	rB = LengthSegmentOnSeparateAxis(&obb1.NormDirection[0], &BE[0], &BE[1], &BE[2]);
	L = fabs(D3DXVec3Dot(&obb1.NormDirection[0], &Interval));
	if (L > rA + rB)return false;//衝突していない

	//分離軸 obb1.NormDirection[1] : obb1の右方向のベクトルに平行な軸にobb2を投影
	rA = obb1.length[1] / 2;
	rB = LengthSegmentOnSeparateAxis(&obb1.NormDirection[1], &BE[0], &BE[1], &BE[2]);
	L = fabs(D3DXVec3Dot(&obb1.NormDirection[1], &Interval));
	if (L > rA + rB)return false;//衝突していない

	//分離軸 obb1.NormDirection[2] : obb1の上方向のベクトルに平行な軸にobb2を投影
	rA = obb1.length[2] / 2;
	rB = LengthSegmentOnSeparateAxis(&obb1.NormDirection[2], &BE[0], &BE[1], &BE[2]);
	L = fabs(D3DXVec3Dot(&obb1.NormDirection[2], &Interval));
	if (L > rA + rB)return false;//衝突していない


	//方向軸を分離軸に obb2

	//分離軸 obb1.NormDirection[0] : obb2の正面方向のベクトルに平行な軸にobb2を投影
	rA= LengthSegmentOnSeparateAxis(&obb2.NormDirection[0], &AE[0], &AE[1], &AE[2]);
	rB= obb2.length[0] / 2;
	L = fabs(D3DXVec3Dot(&obb2.NormDirection[0], &Interval));
	if (L > rA + rB)return false;//衝突していない

	//分離軸 obb1.NormDirection[0] : obb2の右方向のベクトルに平行な軸にobb2を投影
	rA = LengthSegmentOnSeparateAxis(&obb2.NormDirection[1], &AE[0], &AE[1], &AE[2]);
	rB = obb2.length[1] / 2;
	L = fabs(D3DXVec3Dot(&obb2.NormDirection[1], &Interval));
	if (L > rA + rB)return false;//衝突していない

	//分離軸 obb1.NormDirection[0] : obb2の上方向のベクトルに平行な軸にobb2を投影
	rA = LengthSegmentOnSeparateAxis(&obb2.NormDirection[1], &AE[0], &AE[1], &AE[2]);
	rB = obb2.length[2] / 2;
	L = fabs(D3DXVec3Dot(&obb2.NormDirection[2], &Interval));
	if (L > rA + rB)return false;//衝突していない


	//外積を分離軸に

	//分離軸の略称について
	//C00 : cross obb1.normdirection[0] obb.normdirection[1]
	//C10 : cross obb1.normdirection[1] obb.normdirection[1]

	D3DXVECTOR3 Cross;

	//分離軸 c00
	D3DXVec3Cross(&Cross, &obb1.NormDirection[0], &obb2.NormDirection[0]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[1], &AE[2]);//crossに使用した番号 0は使用しない
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[1], &BE[2]);//crossに使用した番号 0は使用しない
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//衝突していない
	
	//分離軸 c01
	D3DXVec3Cross(&Cross, &obb1.NormDirection[0], &obb2.NormDirection[1]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[1], &AE[2]);//crossに使用した番号 0は使用しない
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[0], &BE[2]);//crossに使用した番号 1は使用しない
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//衝突していない

	//分離軸 c01
	D3DXVec3Cross(&Cross, &obb1.NormDirection[0], &obb2.NormDirection[2]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[1], &AE[2]);//crossに使用した番号 0は使用しない
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[0], &BE[1]);//crossに使用した番号 2は使用しない
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//衝突していない

	//--

	//分離軸 c10
	D3DXVec3Cross(&Cross, &obb1.NormDirection[1], &obb2.NormDirection[0]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[0], &AE[2]);//crossに使用した番号 1は使用しない
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[1], &BE[2]);//crossに使用した番号 0は使用しない
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//衝突していない

	//分離軸 c11
	D3DXVec3Cross(&Cross, &obb1.NormDirection[1], &obb2.NormDirection[1]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[0], &AE[2]);//crossに使用した番号 1は使用しない
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[0], &BE[2]);//crossに使用した番号 1は使用しない
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//衝突していない

	//分離軸 c12
	D3DXVec3Cross(&Cross, &obb1.NormDirection[1], &obb2.NormDirection[2]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[0], &AE[2]);//crossに使用した番号 1は使用しない
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[0], &BE[1]);//crossに使用した番号 2は使用しない
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//衝突していない

	//--

	//分離軸 c20
	D3DXVec3Cross(&Cross, &obb1.NormDirection[2], &obb2.NormDirection[0]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[0], &AE[1]);//crossに使用した番号 2は使用しない
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[1], &BE[2]);//crossに使用した番号 0は使用しない
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//衝突していない

	//分離軸 c21
	D3DXVec3Cross(&Cross, &obb1.NormDirection[2], &obb2.NormDirection[1]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[0], &AE[1]);//crossに使用した番号 2は使用しない
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[0], &BE[2]);//crossに使用した番号 1は使用しない
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//衝突していない

	//分離軸 c22
	D3DXVec3Cross(&Cross, &obb1.NormDirection[2], &obb2.NormDirection[2]);
	rA = LengthSegmentOnSeparateAxis(&Cross, &AE[0], &AE[1]);//crossに使用した番号 2は使用しない
	rB = LengthSegmentOnSeparateAxis(&Cross, &BE[0], &BE[1]);//crossに使用した番号 2は使用しない
	L = fabs(D3DXVec3Dot(&Cross, &Interval));
	if (L > rA + rB)return false;//衝突していない


	//分離平面が存在しない = 衝突
	return true;

}

float OrientedBoundingBox::LengthSegmentOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3)
{
	// 3つの内積の絶対値の和で投影線分長を計算

	FLOAT r1 = fabs(D3DXVec3Dot(Sep, e1));
	FLOAT r2 = fabs(D3DXVec3Dot(Sep, e2));
	FLOAT r3 = e3 ? (fabs(D3DXVec3Dot(Sep, e3))) : 0;
	return r1 + r2 + r3;
}