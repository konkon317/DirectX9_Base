#pragma once

#include "Global.h"
#include "TriangleList.h"

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




public:
	//メンバ変数 public 

	TL_List tlList;

private:
	//メンバ関数 private



public:
	//メンバ関数 public 
	
	//アクセサ　ミューテータなど
	Mesh* GetThisPointer() { return this; }


	//通常のメンバ関数
	void Release();
	

	void TestFunc();

	void TestShow() { cout << tlList.size(); }



};