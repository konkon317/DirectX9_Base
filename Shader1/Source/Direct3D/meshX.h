#pragma once

#include "Direct3D.h"
#include "Effect.h"

class Camera;

class MeshX
{

	friend class Direct3D;

private:
	LPD3DXMESH pMesh;
	DWORD	   numMaterials;//マテリアルの個数

	LPDIRECT3DTEXTURE9* ppTextures;
	D3DMATERIAL9 *      pMaterials;

public:
	MeshX();
	~MeshX();

	void DrawMatrice(D3DXMATRIXA16& mat_transform, D3DXMATRIXA16& mat_scale, D3DXMATRIXA16& mat_rotate, Effect* pEfect=nullptr);
	
	void Load(TCHAR* path);
};