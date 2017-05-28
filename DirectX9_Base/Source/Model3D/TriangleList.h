#pragma once


#include <windows.h>
#pragma comment(lib, "winmm.lib")
#include <comdef.h>
#include <mmsystem.h>

//directX
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")

//list
#include <list>

//基本入出力
//#include <iostream>

using namespace std;

//fbx
#include <fbxsdk.h>
using namespace fbxsdk;


struct ColorRGBA;

class Direct3D;

class TriangleList
{
	friend class Direct3D;

public:
	//インナークラス(構造体)
	struct Vertex
	{
		D3DXVECTOR3 pos;
		DWORD color;
	};

	//静的定数
	static const int FVF_TRIANGLE_LIST_VERTEX;
	static const Vertex DEFAULT_VERTEX;

	struct LoadParamator
	{
		int indexCount;
		D3DXVECTOR4* pVertexPoints_DX;
		int VertexColorSetNum;
		int* pColorCount_ByVerTexColorSet;
		ColorRGBA** ppVertexColor;
	};

public:
	//コンストラクタ
	TriangleList();

	//デストラクタ
	~TriangleList();

private:
	//メンバ変数 private


	Vertex* pVertices;

	int triangleCount;
	int vertexCount;

public:
	//メンバ変数 public 



private:
	//メンバ関数 private



public:
	//メンバ関数 public 
	//アクセサ　ミューテータなど
	TriangleList* GetThisPointer() { return this; }
	
	
	//通常のメンバ関数
	void Release();

	//vertcies用のメモリ領域を確保
	bool TryAllocMem_Verticies(int vertexCount);

	bool LoadVerticies(LoadParamator param);

};

