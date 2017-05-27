#pragma once

#include "Global.h"
#include "TriangleList.h"

#include "FbxMeshLoader.h"

typedef list<TriangleList> TL_List;

class Mesh
{
public:
	//コンストラクタ
	Mesh();

	//デストラクタ
	~Mesh();

private:
	//メンバ変数 private
	TL_List tlList;

public:
	//メンバ変数 public 



private:
	//メンバ関数 private



public:
	//メンバ関数 public 
	
	//アクセサ　ミューテータなど
	Mesh* GetThisPointer() { return this; }

	

	//通常のメンバ関数

	//解放関数
	void Release();
	
	//ロード関数
	bool Load(FbxMeshLoader* pMeshLoader);




};