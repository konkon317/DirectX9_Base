#pragma once

#include "Effect.h"

class EffectPriorityBufferShadow:public Effect
{
	D3DXHANDLE H_matWorldViewProj;
	D3DXHANDLE H_matWorldViewProj_L;//ライト視点の射影空間への変換行列
	D3DXHANDLE H_matWorldViewProjTex;//ライト視点からテクスチャ座標系への変換
	
	D3DXHANDLE H_TexDecale;//デカールマップ
	D3DXHANDLE H_TexIdMap;//IDマップ
	
	D3DXHANDLE H_VecLightDirection;
	D3DXHANDLE H_vCol;//メッシュの色

	D3DXHANDLE H_vId;//IDマップへ書きこむID


	D3DXHANDLE H_TecBasic;

	//行列

	D3DXMATRIX lightView;
	D3DXMATRIX lightProj;

	D3DXVECTOR4 lightDir;

	

public:

	HRESULT CreateFromFile(std::string filePath);

	HRESULT SetupParameter_OnSetTechnique(EffectParamSetter& setter, int tecniqueNum, D3DXHANDLE& tecHandle);
	HRESULT SetupParameter_OnBegin(EffectParamSetter& setter, UINT*pPasses, DWORD Flag, unsigned int subsetNum);
	HRESULT SetupParameter_OnBeginPass(EffectParamSetter& setter, UINT pass);
	

	//パラメータ設定関数
	void SetMatrixWorldViewProj(D3DXMATRIXA16& mat);
	void SetMatrixWorldViewProj_L(D3DXMATRIXA16& mat);
	void SetMatrixWorldViewProjTex(D3DXMATRIXA16& mat);

	void SetTextureDecale(LPDIRECT3DTEXTURE9 pTexture);
	void SetTextureIdMap(LPDIRECT3DTEXTURE9 pTexture);

	void SetVectorId(const D3DXVECTOR4& vec);
	void SetVectorLightDirection(const D3DXVECTOR4& vec);
	void SetVectorColor(const D3DXVECTOR4& vec);

	HRESULT GetTeqniqueHandle(int n, D3DXHANDLE&handle);	


	void SetLightView(D3DXMATRIXA16& mat) { lightView = mat; }
	D3DXMATRIX& getlightView() { return lightView; };
	void SetLightProj(D3DXMATRIXA16& mat) { lightProj = mat; }
	D3DXMATRIX& getlightProj() { return lightProj; };

	

	void SetWorldLightDir(D3DXVECTOR4 lightDir) { this->lightDir = lightDir; }

	D3DXVECTOR4 GetWorldLightDir() { return lightDir; }


};