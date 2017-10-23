#pragma once

#include "Effect.h"

class EffectProjectedTextureShadow :public Effect
{
	D3DXHANDLE H_matWorldViewProj;
	D3DXHANDLE H_matWorldViewProjTex;
	D3DXHANDLE H_VecLightPos;
	D3DXHANDLE H_TecBasic;
	D3DXHANDLE H_TexMain;
	D3DXHANDLE H_TexShadowMap;

public:

	HRESULT CreateFromFile(std::string filePath);


	HRESULT SetupParameter_OnSetTechnique(EffectParamSetter& setter, int tecniqueNum, D3DXHANDLE& tecHandle);


	HRESULT SetupParameter_OnBegin(EffectParamSetter& setter, UINT*pPasses, DWORD Flag, int subsetNum);

	HRESULT SetupParameter_OnBeginPass(EffectParamSetter& setter, UINT pass);


	//�p�����[�^�ݒ�֐�
	void SetMatrixWorldViewProj(D3DXMATRIXA16& mat);
	void SetMatrixWorldViewProjTex(D3DXMATRIXA16& mat);
	void SetVectorLightPos(const D3DXVECTOR4& v);
	void SetTextureMain(LPDIRECT3DTEXTURE9 pTexture);
	void SetShadowMap(LPDIRECT3DTEXTURE9 pTexture);

	HRESULT GetTeqniqueHandle(int n, D3DXHANDLE&handle);
};