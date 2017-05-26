#include "TriangleList.h"

//静的メンバ変数の初期化

//頂点構造体宣言
const int TriangleList::FVF_TRIANGLE_LIST_VERTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE;

//頂点のデフォルト値
const TriangleList::Vertex	TriangleList::DEFAULT_VERTEX =
{
	D3DXVECTOR3(0.0f,0.0f,0.0f),
	DWORD(0xffffffff)
};


//--------------

//コンストラクタ
TriangleList::TriangleList()
{
	pVertices = nullptr;
	triangleCount=0;
	vertexCount=0;
}

//--------------

//デストラクタ
TriangleList::~TriangleList()
{

	Release();
}

//--------------

//解放関数
void TriangleList::Release()
{
	//コンストラクタ実行時と同じ状態に戻す

	if (pVertices != nullptr)
	{
		delete[]pVertices;
		pVertices = nullptr;
	}

	triangleCount = 0;
	vertexCount = 0;
}


//--------------