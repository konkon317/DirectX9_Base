#include "EffectPriorityBufferShadow.h"

#include "EffectParamSetter.h"


#include "../meshX.h"
#include "../meshXWithHeight.h"


HRESULT EffectPriorityBufferShadow::CreateFromFile(std::string filePath)
{

	HRESULT h = Effect::CreateFromFile(filePath);

	if (FAILED(h))
	{
		return h;
	}

	H_TecBasic = pEffect->GetTechniqueByName("BasicTec");

	H_matWorldViewProj = pEffect->GetParameterByName(NULL, "matWorldViewProj");
	H_matWorldViewProj_L = pEffect->GetParameterByName(NULL, "matWorldViewProj_L");
	H_matWorldViewProjTex = pEffect->GetParameterByName(NULL, "matWorldViewProjTex");

	H_TexDecale = pEffect->GetParameterByName(NULL, "DecaleMap");
	H_TexIdMap = pEffect->GetParameterByName(NULL, "IdMap");

	H_VecLightDirection = pEffect->GetParameterByName(NULL, "vLightDir");
	H_vCol = pEffect->GetParameterByName(NULL, "vCol");
	H_vId = pEffect->GetParameterByName(NULL, "vId");

	return S_OK;
}

HRESULT EffectPriorityBufferShadow::GetTeqniqueHandle(int n, D3DXHANDLE&handle)
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

HRESULT EffectPriorityBufferShadow::SetupParameter_OnSetTechnique(EffectParamSetter& setter, int tecniqueNum, D3DXHANDLE& tecHandle)
{
	return setter.OnSetTechnique(this, tecniqueNum, tecHandle);
}

HRESULT EffectPriorityBufferShadow::SetupParameter_OnBegin(EffectParamSetter& setter, UINT*pPasses, DWORD Flag, unsigned int subsetNum)
{
	return setter.OnBegin(this, pPasses, Flag, subsetNum);
}
HRESULT EffectPriorityBufferShadow::SetupParameter_OnBeginPass(EffectParamSetter& setter, UINT pass)
{
	return setter.OnBeginPass(this, pass);
}

//パラメータ設定関数==============================================================

//行列
void EffectPriorityBufferShadow::SetMatrixWorldViewProj(D3DXMATRIXA16& mat)
{
	Effect::SetMatrix(H_matWorldViewProj, mat);
}
void EffectPriorityBufferShadow::SetMatrixWorldViewProj_L(D3DXMATRIXA16& mat)
{
	Effect::SetMatrix(H_matWorldViewProj_L, mat);
}
void EffectPriorityBufferShadow::SetMatrixWorldViewProjTex(D3DXMATRIXA16& mat)
{
	Effect::SetMatrix(H_matWorldViewProjTex, mat);
}

//テクスチャ
void EffectPriorityBufferShadow::SetTextureDecale(LPDIRECT3DTEXTURE9 pTexture)
{
	Effect::SetTexture(H_TexDecale, pTexture);
}
void EffectPriorityBufferShadow::SetTextureIdMap(LPDIRECT3DTEXTURE9 pTexture)
{
	Effect::SetTexture(H_TexIdMap, pTexture);
}

//ベクトル
void EffectPriorityBufferShadow::SetVectorId(const D3DXVECTOR4& vec)
{
	Effect::SetVector4(H_vId, vec);
}

void EffectPriorityBufferShadow::SetVectorLightDirection(const D3DXVECTOR4& vec)
{
	Effect::SetVector4(H_VecLightDirection, vec);
}

void EffectPriorityBufferShadow::SetVectorColor(const D3DXVECTOR4& vec)
{
	Effect::SetVector4(H_vCol, vec);
}

//====================================================================================

















HRESULT EffectParamSetter::OnSetTechnique(EffectPriorityBufferShadow* pEffect, int techniqueNum, D3DXHANDLE& tecHandle)
{
	D3DXMATRIXA16 mt = D3DXMATRIXA16
	(0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);


	HRESULT h = pEffect->GetTeqniqueHandle(techniqueNum, tecHandle);
	if (FAILED(h))
		return h;


	Direct3D& d3d = Direct3D::GetInstance();

	D3DXMATRIXA16 view, proj, worldMat;

	h = d3d.GetTransForm(D3DTS_VIEW, view);
	if (FAILED(h))
		return h;

	h = d3d.GetTransForm(D3DTS_PROJECTION, proj);
	if (FAILED(h))
		return h;

	h = d3d.GetTransForm(D3DTS_WORLD, worldMat);

	//ワールド変換行列-----------------
	D3DXMATRIXA16 worldViewProj;

	D3DXMatrixMultiply(&worldViewProj, &worldMat, &view);
	D3DXMatrixMultiply(&worldViewProj, &worldViewProj, &proj);	
	pEffect->SetMatrixWorldViewProj(worldViewProj);

	D3DXMATRIXA16 lightView = pEffect->getlightView();
	D3DXMATRIXA16 lightProj = pEffect->getlightProj();

	D3DXMATRIXA16 worldViewProjTex;
	D3DXMatrixMultiply(&worldViewProjTex, &worldMat, &lightView);
	D3DXMatrixMultiply(&worldViewProjTex, &worldViewProjTex, &lightProj);
	D3DXMatrixMultiply(&worldViewProjTex, &worldViewProjTex, &mt);
	pEffect->SetMatrixWorldViewProjTex(worldViewProjTex);

	return S_OK;
}


HRESULT EffectParamSetter::OnBegin(EffectPriorityBufferShadow* pEffect, UINT*pPasses, DWORD Flag, unsigned int subsetNum)
{
	HRESULT h = S_OK;
	switch (mode)
	{
		case EffectParamSetter::NONE:
			break;
		case EffectParamSetter::MESH_X:
			if (pMesh != nullptr)
			{

				if (subsetNum >= 0 && subsetNum < pMesh->GetNumMaterials())
				{
					pEffect->SetTextureDecale(pMesh->Get_ppTextures()[subsetNum]);
				}
			}
			else
			{
				h = E_FAIL;
			}

			break;
		default:
			h = E_FAIL;
			break;
	}
	return h;
}

HRESULT EffectParamSetter::OnBeginPass(EffectPriorityBufferShadow* pEffect, UINT pass)
{
	return S_OK;
}
