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


class Model;


class FbxUtil
{
	//静的メンバだけのクラスとして作る
	
	//まずい手法だったら元に戻す

private:

	//インスタンスの作成を禁じる
	FbxUtil() {}
	FbxUtil(FbxUtil& ref) {}
	FbxUtil& operator=(FbxUtil& ref) { return (*this); }

public :
	static void ProbeNode(FbxNode* pNode, Model* pModel = nullptr, int depth = 0);
	
	static bool IsMesh(FbxNode* pNode);
};