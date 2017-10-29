#pragma once
#pragma once

#include "Effect.h"

class EffectBasic :public Effect
{
	D3DXHANDLE H_matWorldViewProj;
	D3DXHANDLE H_TecBasic;
	D3DXHANDLE H_TexMain;

public:

	HRESULT CreateFromFile(std::string filePath);


	HRESULT SetupParameter_OnSetTechnique(EffectParamSetter& setter, int tecniqueNum, D3DXHANDLE& tecHandle);


	HRESULT SetupParameter_OnBegin(EffectParamSetter& setter, UINT*pPasses, DWORD Flag,unsigned int subsetNum);

	HRESULT SetupParameter_OnBeginPass(EffectParamSetter& setter, UINT pass);


	//パラメータ設定関数
	void SetMatrixWorldViewProj(D3DXMATRIXA16& mat);
	

	void SetTextureMain(LPDIRECT3DTEXTURE9 pTexture);


	HRESULT GetTeqniqueHandle(int n, D3DXHANDLE&handle);



};