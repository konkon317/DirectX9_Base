#pragma once

#include <fbxsdk.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//#include <memory>

#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>

#include <iostream>

#include <list>

struct Point2DF
{
	float x, y;

	bool operator==(Point2DF& val)
	{
		return(this->x == val.x &&this->y == val.y);
	}
};

class UvSet
{


public :
	std::string uvSetName;
	//std::list<std::string> textures;
	std::string texture;
	Point2DF* pUvBuffer;

	UvSet();
	~UvSet();

};





struct ColorRGBA
{
	float r;
	float g;
	float b;
	float a;

};



class FbxMeshLoader
{
private:

	//ロード済みかどうか
	bool loaded;

	//頂点座標の数
	int vertexPointCount;

	//頂点座標
	D3DXVECTOR4* pVertexPoints_DX;

	//ポリゴン数
	int polygonCount;

	////総長点数
	//int polygonVertexNum;
	//int * pIndexArray;

	//それぞれのポリゴンを構成する頂点の数
	// x番目のポリゴンを構成する頂点数 = pPolygonVertexCount[x];
	int *pPolygonVertexCount;

	//それぞれのポリゴンを構成する頂点のインデックス
	//頂点インデックス = ppPolygonVertexIndex [p][x];
	//	p :ポリゴン番号
	//	x = 0　～ (pPolygonVertexCount[p]-1)
	int **ppPolygonVertexIndex;


	//頂点色
	int VertexColorSetMax;
	int *pColorCount_ByVertexColorSet;
	ColorRGBA** ppVertexColor;


	//--------------
	//法線

	//法線レイヤー数
	int normalLayerCount;

	//法線レイヤー数ごとの法線数
	//法線数 = pNormalCounts[レイヤー番号]
	int *pNormalCounts;

	//法線データ
	//法線 = ppNormalVector[レイヤー番号][法線番号]
	D3DXVECTOR4 **ppNormalVector;


	//マテリアル数
	int materialCount;

	//uvマップ
	int uvSetCount;	
	UvSet* pUvSetArray;


public :
	//デフォルトコンストラクタ
	FbxMeshLoader();
	//デストラクタ
	~FbxMeshLoader();

	//ロード
	//ロードしたいメッシュを持つノード
	void Load(FbxNode* pNode);

	//解放処理
	void Release();

private:

	void GetVertexUV_Buffer(FbxMesh * pMesh);

	void GetNormal(FbxMesh * pMesh);

	void Asociate_UVSetAndMaterial(FbxNode* pNode);

};