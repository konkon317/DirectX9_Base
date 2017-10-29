#pragma once

#pragma once

#include "Effect.h"

class EffectPhongAndNormal :public Effect
{
	D3DXHANDLE H_matWorldViewProj;
	D3DXHANDLE H_matWorldInverseTranspose;
	D3DXHANDLE H_TecBasic;
	D3DXHANDLE H_TexMain;
	D3DXHANDLE H_NormalTex;
	D3DXHANDLE H_VecLightDirection;
	D3DXHANDLE H_VecEye;

public:

	HRESULT CreateFromFile(std::string filePath);


	HRESULT SetupParameter_OnSetTechnique(EffectParamSetter& setter, int tecniqueNum, D3DXHANDLE& tecHandle);


	HRESULT SetupParameter_OnBegin(EffectParamSetter& setter, UINT*pPasses, DWORD Flag,unsigned int subsetNum);

	HRESULT SetupParameter_OnBeginPass(EffectParamSetter& setter, UINT pass);




	//パラメータ設定関数
	void SetMatrixWorldViewProj(D3DXMATRIXA16& mat);


	void SetMatrixWorldInverseTranspose(D3DXMATRIXA16& mat);


	void SetTextureMain(LPDIRECT3DTEXTURE9 pTexture);
	void SetTextureNormal(LPDIRECT3DTEXTURE9 pTexture);

	void SetVectorLightDirection(const D3DXVECTOR4& v);
	void SetVectorEye(const D3DXVECTOR4& v);


	HRESULT GetTeqniqueHandle(int n, D3DXHANDLE&handle);



};
