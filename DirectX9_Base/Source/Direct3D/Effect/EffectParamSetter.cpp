#include "EffectParamSetter.h"
#include "EffectLambert.h"
#include "EffectBasic.h"
#include "Effect.h"

#include "../meshX.h"

//==============================================================================================================
//EffectLambert用の関数

HRESULT  EffectParamSetter::OnSetTechnique(EffectLambert* pEffect, int techniqueNum, D3DXHANDLE& tecHandle)
{

	
	HRESULT h= pEffect->GetTeqniqueHandle(techniqueNum, tecHandle);
	if (FAILED(h))
		return h;
	

	Direct3D& d3d = Direct3D::GetInstance();

	D3DXMATRIXA16 view, proj,worldMat;

	h =d3d.GetTransForm(D3DTS_VIEW, view);
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


	//光源　向き-----------------------

	//ノーマル変換の為の行列を求める
	//ライトの向きを計算　
	//シェーダでの光量計算に使用
	
	//各ポリゴンの法線をそれぞれ変換するのではなく
	//光の向きを変換することによりシェーダでの計算量を減らす

	D3DXMATRIXA16 worldInverse;
	D3DXMatrixInverse(&worldInverse, NULL, &worldMat);//逆行列
	D3DXVECTOR4 lightVec = D3DXVECTOR4(1, -1, 1, 0);
	D3DXVec4Transform(&lightVec,&lightVec,&worldInverse);
	D3DXVec3Normalize((D3DXVECTOR3*)&lightVec, (D3DXVECTOR3*)&lightVec);
	
	pEffect->SetVectorLightDirection(lightVec);


	return S_OK;
}

HRESULT  EffectParamSetter::OnBegin(EffectLambert* pEffect, UINT*pPasses, DWORD Flag,int subsetNum)
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
					pEffect->SetTextureMain(pMesh->Get_ppTextures()[subsetNum]);
				}
			}
			else
			{
				h= E_FAIL;
			}
			
			break;
		default:
			h = E_FAIL;
			break;
	}
	return h;
}

HRESULT  EffectParamSetter::OnBeginPass(EffectLambert* pEffect, UINT pass)
{

	return S_OK;
}

//==============================================================================================================


//==============================================================================================================
//EffectBasic用の関数

HRESULT  EffectParamSetter::OnSetTechnique(EffectBasic* pEffect, int techniqueNum, D3DXHANDLE& tecHandle)
{


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

	D3DXMATRIXA16 worldViewProj;
	D3DXMatrixMultiply(&worldViewProj, &worldMat, &view);
	D3DXMatrixMultiply(&worldViewProj, &worldViewProj, &proj);
																		
	pEffect->SetMatrixWorldViewProj(worldViewProj);//シェーダに行列を設定
	

	return S_OK;
}

HRESULT  EffectParamSetter::OnBegin(EffectBasic* pEffect, UINT*pPasses, DWORD Flag, int subsetNum)
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
					pEffect->SetTextureMain(pMesh->Get_ppTextures()[subsetNum]);
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

HRESULT  EffectParamSetter::OnBeginPass(EffectBasic* pEffect, UINT pass)
{

	return S_OK;
}

//===