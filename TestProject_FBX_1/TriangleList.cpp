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

//頂点構造体のインスタンス作成　動的にメモリを確保
bool TriangleList::TryAllocMem_Verticies(int vertexCount)
{ 
	if (pVertices != nullptr)
	{
		//割り当て済みなら失敗
		return false;
	}

	//頂点数0以下はありえん
	if (vertexCount <= 0)
	{
		return false;
	}

	//頂点数が3で割り切れる数かを調べる
	if (vertexCount % 3 == 0)
	{
		triangleCount = vertexCount / 3;
		this->vertexCount = vertexCount;
	}
	else
	{
		//総長点数が3で割り切れないとトライアングルリストになりえない
		//失敗
		return false;
	}

	//メモリの動的確保
	pVertices = new Vertex[vertexCount];

	if (pVertices == nullptr)
	{
		//失敗
		//状態を戻してfalseを返す
		Release();
		return false;
	}

	for (int i = 0; i < vertexCount; i++)
	{
		pVertices[i] = DEFAULT_VERTEX;
	}
	
	//成功
	return true;
}