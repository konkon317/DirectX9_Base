#pragma once

#include "Direct3D.h"

#include <string>

class Effect
{
	friend class Direct3D;

private:

	ID3DXEffect* pEffect;

public :

	Effect();
	~Effect();

	HRESULT CreateFromFile(std::string filePath);

	HRESULT SetMatrix(D3DXHANDLE hParamater, const D3DXMATRIX& mat)
	{
		if (pEffect == nullptr)return E_FAIL;
		return pEffect->SetMatrix(hParamater, &mat);
	}

	HRESULT SetTexture(D3DXHANDLE hParamater, LPDIRECT3DTEXTURE9 texture)
	{
		if (pEffect == nullptr)return E_FAIL;
		return pEffect->SetTexture(hParamater, texture);
	}

	//テクニックのセット
	HRESULT SetTechnique(D3DXHANDLE hParamater)
	{
		if (pEffect == nullptr)return E_FAIL;

		this->SetupParameter_OnSetTechnique(hParamater);

		return pEffect->SetTechnique(hParamater);
	}


	//エフェクト使用の開始
	HRESULT Begine(UINT*pPasses, DWORD Flag)
	{
		if (pEffect == nullptr)return E_FAIL;

		this->SetupParameter_OnBegin();
		return pEffect->Begin(pPasses,Flag);
	}

	//パスの開始
	HRESULT BeginePass(UINT pass) 
	{
		if (pEffect == nullptr)return E_FAIL;

		this->SetupParameter_OnBeginPass(pass);
		return pEffect->BeginPass(pass);
	}

	//パスの終了
	HRESULT EndPass()
	{
		if (pEffect == nullptr)return E_FAIL;
		return pEffect->EndPass();
	}

	//エフェクト使用の終了
	HRESULT End() 
	{
		if (pEffect == nullptr)return E_FAIL;
		return pEffect->End();
	}	

protected:
	//
	virtual void SetupParameter_OnSetTechnique(D3DXHANDLE hParamater) {};// = 0;
	virtual void SetupParameter_OnBegin() {};// = 0;
	virtual void SetupParameter_OnBeginPass(UINT pass) {};// = 0;
		
};