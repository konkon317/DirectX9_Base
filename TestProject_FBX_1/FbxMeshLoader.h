#pragma once

#include <fbxsdk.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//#include <memory>

#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>

#include <iostream>

class FbxMeshLoader
{
private:

	bool Loaded;

	int controlNum;
	
	D3DXVECTOR4* pVertexPoints_DX;

	int polygonNum;
	int polygonVertexNum;
	int * pIndexArray;

	int *pPolygonVertexCount;
	int **ppPolygonVertexIndex;


public :
	FbxMeshLoader();
	~FbxMeshLoader();
	void Load(FbxNode* pNode);

	void Release();


};