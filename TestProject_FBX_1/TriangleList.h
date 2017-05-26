#pragma once

#include "Global.h"



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
	void Release();



};

