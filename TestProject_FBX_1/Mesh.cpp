﻿#include "Mesh.h"


//--------------

//コンストラクタ
Mesh::Mesh()
{

}

//--------------

//デストラクタ
Mesh::~Mesh()
{

}

//--------------

//解放関数
void Mesh::Release()
{ 


}

//--------------

//ロード関数
bool Mesh::Load(FbxMeshLoader* pMeshLoader)
{
	if (pMeshLoader == nullptr)
	{
		return false;
	}

	{
		TriangleList tl;
		tlList.push_back(tl);
	}

	TL_List::iterator it = tlList.end();
	it--;//最終要素はダミーなので endの一つ前が直前に追加したオブジェクト


	//頂点構造体配列のためのメモリ確保
	if (it->TryAllocMem_Verticies(pMeshLoader->indexCount) ==false)
	{
		tlList.erase(it);
		return false;
	}

	TriangleList::LoadParamator param;

	param.indexCount					= pMeshLoader->indexCount;
	param.pVertexPoints_DX				= pMeshLoader->pVertexPoints_DX;
	param.VertexColorSetNum				= pMeshLoader->VertexColorSetMax;
	param.pColorCount_ByVerTexColorSet	= pMeshLoader->pColorCount_ByVertexColorSet;
	param.ppVertexColor					= pMeshLoader->ppVertexColor;
	


	//ロードに失敗したらfalse を返す
	if (it->LoadVerticies(param) == false)
	{
		tlList.erase(it);
		return false;
	}

	return false;


}