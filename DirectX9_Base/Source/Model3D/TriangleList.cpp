#include "TriangleList.h"
#include "../FBX/FbxMeshLoader.h"


//静的メンバ変数の初期化

//頂点構造体宣言
const int TriangleList::FVF_TRIANGLE_LIST_VERTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE;

//頂点のデフォルト値
const TriangleList::Vertex	TriangleList::DEFAULT_VERTEX =
{
	D3DXVECTOR3(0.0f,0.0f,0.0f),
	D3DCOLOR_ARGB(255,255,255,255)
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


//--------------

//頂点情報のロード
bool TriangleList::LoadVerticies(LoadParamator param)
{
	if (pVertices == nullptr)
	{
		//メモリの確保がされていない
		return false;
	}

	if (param.indexCount != vertexCount)
	{
		return false;
	}

	//頂点座標のロード
	if (param.pVertexPoints_DX != nullptr)
	{
		for (int i = 0; i < vertexCount; i++)
		{
			pVertices[i].pos.x = param.pVertexPoints_DX[i].x;
			pVertices[i].pos.y = param.pVertexPoints_DX[i].y;
			pVertices[i].pos.z = param.pVertexPoints_DX[i].z;
		}
	}

	bool b1 = param.ppVertexColor != nullptr &&param.pColorCount_ByVerTexColorSet!=nullptr ;
	bool b2 = param.VertexColorSetNum > 0;
	if (b1 && b2)
	{
		bool b3 = false;
		for (int i = 0; i < param.VertexColorSetNum; i++)
		{
			if (param.ppVertexColor == nullptr || param.pColorCount_ByVerTexColorSet[i]!=vertexCount)
			{
				b3 = true;
				break;
			}
		}

		if (b3 == false)
		{
			for (int i = 0; i < vertexCount; i++)
			{
				float r = param.ppVertexColor[0][i].r;
				float g = param.ppVertexColor[0][i].g;
				float b = param.ppVertexColor[0][i].b;
				float a = param.ppVertexColor[0][i].a;

				pVertices[i].color = 0xffffffff;
				pVertices[i].color =D3DCOLOR_ARGB( static_cast<int>(255*a), static_cast<int>(255 * r), static_cast<int>(255 * g), static_cast<int>(255 * b));				
			}
		}
	}


	return true;
}

//--------------

//デバッグ用の関数

void TriangleList::Debug_TestShow()
{
	int bp = 0;

	Vertex* p = pVertices;
	int vc = this->vertexCount;
	int tc = this->triangleCount;
	bp = 0;


};