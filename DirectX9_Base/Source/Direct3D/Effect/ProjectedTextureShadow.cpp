#include"ProjectedTextureShadow.h"

#include"EffectParamSetter.h"

HRESULT EffectProjectedTextureShadow::CreateFromFile(std::string filePath)
{

	HRESULT h = Effect::CreateFromFile(filePath);

	if (FAILED(h))
	{
		return h;
	}

	H_TecBasic = pEffect->GetTechniqueByName("BasicTec");
	H_matWorldViewProj = pEffect->GetParameterByName(NULL, "matWorldViewProj");
	H_matWorldViewProjTex = pEffect->GetParameterByName(NULL, "matWorldViewProjTex");
	H_VecLightPos = pEffect->GetParameterByName(NULL, "vLightDir");

	H_TexMain = pEffect->GetParameterByName(NULL, "DecaleMap");
	H_TexShadowMap = pEffect->GetParameterByName(NULL, "ShadowMap");
	return S_OK;
}

HRESULT EffectProjectedTextureShadow::GetTeqniqueHandle(int n, D3DXHANDLE&handle)
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

HRESULT EffectProjectedTextureShadow::SetupParameter_OnSetTechnique(EffectParamSetter& setter, int tecniqueNum, D3DXHANDLE& tecHandle)
{
	return setter.OnSetTechnique(this, tecniqueNum, tecHandle);
}

HRESULT EffectProjectedTextureShadow::SetupParameter_OnBegin(EffectParamSetter& setter, UINT*pPasses, DWORD Flag, int subsetNum)
{
	return setter.OnBegin(this, pPasses, Flag, subsetNum);
}
HRESULT EffectProjectedTextureShadow::SetupParameter_OnBeginPass(EffectParamSetter& setter, UINT pass)
{
	return setter.OnBeginPass(this, pass);
}

//ÉpÉâÉÅÅ[É^ê›íËä÷êî
void EffectProjectedTextureShadow::SetVectorLightPos(const D3DXVECTOR4& v)
{
	Effect::SetVector4(H_VecLightPos, v);
}

void EffectProjectedTextureShadow::SetMatrixWorldViewProj(D3DXMATRIXA16& mat)
{
	Effect::SetMatrix(H_matWorldViewProj, mat);
}

void EffectProjectedTextureShadow::SetMatrixWorldViewProjTex(D3DXMATRIXA16& mat)
{
	Effect::SetMatrix(H_matWorldViewProjTex, mat);
}

void EffectProjectedTextureShadow::SetTextureMain(LPDIRECT3DTEXTURE9 pTexture)
{
	Effect::SetTexture(H_TexMain, pTexture);
}

void EffectProjectedTextureShadow::SetShadowMap(LPDIRECT3DTEXTURE9 pTexture)
{
	Effect::SetTexture(H_TexShadowMap, pTexture);
}