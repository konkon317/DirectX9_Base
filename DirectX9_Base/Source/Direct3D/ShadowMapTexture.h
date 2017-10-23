#pragma once

#include"Direct3D.h"

class ShadowMapTexture
{
public :
	static HRESULT Create(IDirect3DDevice9* pDevice,ShadowMapTexture& shadowMapTexture);
private:
	LPDIRECT3DTEXTURE9 shadowTex;
	LPDIRECT3DSURFACE9 shadowSurf;
	LPDIRECT3DSURFACE9 shadowTexZ;

	int size;

public : 
	ShadowMapTexture();
	virtual ~ShadowMapTexture();

	virtual void Release();
	bool Created() { return shadowTex != nullptr; }

	HRESULT Create(int size);

};
