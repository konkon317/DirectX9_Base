#pragma once

#include "MeshX.h"

class MeshXWithHeight : public MeshX
{
	static IDirect3DVertexDeclaration9* pVertexDelc;
	static void CreateDelc();

protected :

	LPDIRECT3DTEXTURE9* ppNormalTextures;
	
	

public : 
	MeshXWithHeight();
	~MeshXWithHeight();

	const LPDIRECT3DTEXTURE9* Get_ppNormalTextures() const { return ppNormalTextures; }

	LPDIRECT3DTEXTURE9* Get_ppNormalTextures() { return ppNormalTextures; }

	//void DrawMatrice(D3DXMATRIXA16& mat_transform, D3DXMATRIXA16& mat_scale, D3DXMATRIXA16& mat_rotate, Effect*pEffect) override;
	void Load(TCHAR* path) override;
};

