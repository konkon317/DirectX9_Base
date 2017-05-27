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

#include "Mesh.h"

#include "../FBX/FbxMeshLoader.h"

typedef list<Mesh> Mesh_List ;

class Model
{
public :
	//コンストラクタ
	Model();

	//デストラクタ
	~Model();

private:
	//メンバ変数 private
	Mesh_List meshList;


public :
	//メンバ変数 public 





private:
	//メンバ関数 private



public :
	//メンバ関数 public 

	void Release();
	bool AddMeshFromFbxMeshLoader(FbxMeshLoader* pMeshLoader);

	//テスト用のデバッグ関数
	void Debug_TestShow();
};