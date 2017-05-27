#pragma once

#include "Global.h"

struct ColorRGBA;


class TriangleList
{
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

