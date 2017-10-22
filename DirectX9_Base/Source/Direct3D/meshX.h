#pragma once

#include "Direct3D.h"
class Camera;
class Effect;

class MeshX
{

	friend class Direct3D;

protected:
	LPD3DXMESH pMesh;
	DWORD	   numMaterials;//マテリアルの個数

	LPDIRECT3DTEXTURE9* ppTextures;
	D3DMATERIAL9 *      pMaterials;

public:

	const LPDIRECT3DTEXTURE9* Get_ppTextures() const{ return ppTextures; }
	const D3DMATERIAL9*       Get_ppMaterials()const { return pMaterials; }
	DWORD GetNumMaterials() { return numMaterials; }

	MeshX();
	virtual ~MeshX();

	virtual void DrawMatrice(D3DXMATRIXA16& mat_transform, D3DXMATRIXA16& mat_scale, D3DXMATRIXA16& mat_rotate,Effect*pEffect);
	
	virtual void Load(TCHAR* path);
};