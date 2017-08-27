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

	HRESULT SetTechnique(D3DXHANDLE hParamater)
	{
		if (pEffect == nullptr)return E_FAIL;
		return pEffect->SetTechnique(hParamater);
	}

	HRESULT Begine(UINT*pPasses, DWORD Flag)
	{
		if (pEffect == nullptr)return E_FAIL;
		return pEffect->Begin(pPasses,Flag);
	}

	HRESULT BeginePass(UINT pass) 
	{
		if (pEffect == nullptr)return E_FAIL;
		return pEffect->BeginPass(pass);
	}

	HRESULT EndPass()
	{
		if (pEffect == nullptr)return E_FAIL;
		return pEffect->EndPass();
	}

	HRESULT End() 
	{
		if (pEffect == nullptr)return E_FAIL;
		return pEffect->End();
	}
		
};