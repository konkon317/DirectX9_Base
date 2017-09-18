#pragma once

#include "../Direct3D.h"
class EffectParamSetter;

#include <string>

class Effect
{
	friend class Direct3D;

protected:

	ID3DXEffect* pEffect;

public :

	Effect();
	~Effect();

	virtual HRESULT CreateFromFile(std::string filePath)=0;

	HRESULT SetMatrix(D3DXHANDLE hParamater, const D3DXMATRIX& mat);


	HRESULT SetTexture(D3DXHANDLE hParamater, LPDIRECT3DTEXTURE9 texture);

	HRESULT SetVector4(D3DXHANDLE hParameter,const D3DXVECTOR4& vec4 );


	//テクニックのセット
	HRESULT SetTechnique(EffectParamSetter& setter, int tecniqueNum);

	//エフェクト使用の開始
	HRESULT Begine(EffectParamSetter& setter, UINT*pPasses, DWORD Flag, int subsetNum);


	//パスの開始
	HRESULT BeginePass(EffectParamSetter& setter, UINT pass);


	//パスの終了
	HRESULT EndPass();


	//エフェクト使用の終了
	HRESULT End();
	

protected:
	
	//テクニックのセット　派生後のクラスでは必ずテクニックのセットまで行うこと
	virtual HRESULT SetupParameter_OnSetTechnique(EffectParamSetter& setter, int tecniqueNum,D3DXHANDLE& handle) = 0;
	//Begineに際してのパラメータの設定
	virtual HRESULT SetupParameter_OnBegin(EffectParamSetter& setter, UINT*pPasses, DWORD Flag,int subsetNum) = 0;
	//Begineに際してのパラメータの設定
	virtual HRESULT SetupParameter_OnBeginPass(EffectParamSetter& setter,UINT pass) = 0;
		
};