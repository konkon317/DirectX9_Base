#include "EffectPhong.h"
#include "EffectParamSetter.h"

HRESULT EffectPhong::CreateFromFile(std::string filePath)
{

	HRESULT h = Effect::CreateFromFile(filePath);

	if (FAILED(h))
	{
		return h;
	}

	H_TecBasic = pEffect->GetTechniqueByName("BasicTec");
	H_matWorldViewProj = pEffect->GetParameterByName(NULL, "matWorldViewProj");
	H_matWorldInverseTranspose = pEffect->GetParameterByName(NULL, "matWorldInverseTranspose");

	H_TexMain = pEffect->GetParameterByName(NULL, "Tex");

	H_VecLightDirection = pEffect->GetParameterByName(NULL, "vLightDir");
	H_VecEye = pEffect->GetParameterByName(NULL, "vEyePos");
	return S_OK;
}

HRESULT EffectPhong::GetTeqniqueHandle(int n, D3DXHANDLE&handle)
{

	HRESULT h = E_FAIL;
	switch (n)
	{
		case 0:
			handle = H_TecBasic;
			h = S_OK;
			break;

		default:
			return E_FAIL;
			break;
	}

	return h;
}

HRESULT EffectPhong::SetupParameter_OnSetTechnique(EffectParamSetter& setter, int tecniqueNum, D3DXHANDLE& tecHandle)
{
	return setter.OnSetTechnique(this, tecniqueNum, tecHandle);
}

HRESULT EffectPhong::SetupParameter_OnBegin(EffectParamSetter& setter, UINT*pPasses, DWORD Flag, int subsetNum)
{
	return setter.OnBegin(this, pPasses, Flag, subsetNum);
}
HRESULT EffectPhong::SetupParameter_OnBeginPass(EffectParamSetter& setter, UINT pass)
{
	return setter.OnBeginPass(this, pass);
}

//パラメータ設定関数
void EffectPhong::SetMatrixWorldViewProj(D3DXMATRIXA16& mat)
{
	Effect::SetMatrix(H_matWorldViewProj, mat);
}

void EffectPhong::SetMatrixWorldInverseTranspose(D3DXMATRIXA16& mat)
{
	Effect::SetMatrix(H_matWorldInverseTranspose, mat);
}

void EffectPhong::SetTextureMain(LPDIRECT3DTEXTURE9 pTexture)
{
	Effect::SetTexture(H_TexMain, pTexture);
}

void EffectPhong::SetVectorLightDirection(const D3DXVECTOR4& v)
{
	Effect::SetVector4(H_VecLightDirection, v);
}
void EffectPhong::SetVectorEye(const D3DXVECTOR4& v)
{
	Effect::SetVector4(H_VecEye, v);
}