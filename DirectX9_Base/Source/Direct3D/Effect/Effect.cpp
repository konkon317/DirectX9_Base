

#include "Effect.h"

Effect::Effect()
{
	pEffect = nullptr;
}

Effect::~Effect()
{
	if (pEffect!=nullptr)
	{
		pEffect->Release();
		pEffect = nullptr;
	}
}

HRESULT Effect::CreateFromFile(std::string filepath)
{
	if (pEffect != nullptr)
	{
		return S_FALSE;
	}

	Direct3D& d3d = Direct3D::GetInstance();

	HRESULT result=d3d.CreateEffectFromFile(*this,filepath);
	return result;
}

HRESULT  Effect::SetMatrix(D3DXHANDLE hParamater, const D3DXMATRIX& mat)
{
	if (pEffect == nullptr)return E_FAIL;
	return pEffect->SetMatrix(hParamater, &mat);
}

HRESULT  Effect::SetTexture(D3DXHANDLE hParamater, LPDIRECT3DTEXTURE9 texture)
{
	if (pEffect == nullptr)return E_FAIL;
	return pEffect->SetTexture(hParamater, texture);
}

HRESULT Effect::SetVector4(D3DXHANDLE hPramater,const D3DXVECTOR4& vec4)
{
	if (pEffect == nullptr)return E_FAIL;

	return pEffect->SetVector(hPramater, &vec4);
}

//テクニックのセット
HRESULT  Effect::SetTechnique(EffectParamSetter& setter, int tecniqueNum)
{
	if (pEffect == nullptr)return E_FAIL;

	D3DXHANDLE tecHandle;
	HRESULT h = this->SetupParameter_OnSetTechnique(setter, tecniqueNum, tecHandle);

	if (FAILED(h))
	{
		return h;
	}
	else
	{
		return pEffect->SetTechnique(tecHandle);
	}
}


//エフェクト使用の開始
HRESULT  Effect::Begine(EffectParamSetter& setter, UINT*pPasses, DWORD Flag,unsigned int subsetNum)
{
	if (pEffect == nullptr)return E_FAIL;

	HRESULT h = this->SetupParameter_OnBegin(setter, pPasses, Flag, subsetNum);
	if (FAILED(h))
	{
		return h;
	}
	else
	{
		return pEffect->Begin(pPasses, Flag);
	}
}

//パスの開始
HRESULT  Effect::BeginePass(EffectParamSetter& setter, UINT pass)
{
	if (pEffect == nullptr)return E_FAIL;

	HRESULT h = this->SetupParameter_OnBeginPass(setter, pass);
	if (FAILED(h))
	{
		return h;
	}
	else
	{
		return pEffect->BeginPass(pass);
	}
}

//パスの終了
HRESULT Effect::EndPass()
{
	if (pEffect == nullptr)return E_FAIL;
	return pEffect->EndPass();
}

//エフェクト使用の終了
HRESULT Effect::End()
{
	if (pEffect == nullptr)return E_FAIL;
	return pEffect->End();
}

