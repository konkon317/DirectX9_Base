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

//fbx
#include <fbxsdk.h>
using namespace fbxsdk;

using namespace std;


#include "TriangleList.h"

#include "../FBX/FbxMeshLoader.h"

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

	//描画関数
	void Draw(D3DXMATRIXA16& worldMat);


};