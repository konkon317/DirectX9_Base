#include "EffectParamSetter.h"
#include "EffectPhong.h"
#include "EffectPhongAndNormal.h"
#include "EffectLambert.h"
#include "EffectBasic.h"
#include "Effect.h"

#include "../meshX.h"
#include "../meshXWithHeight.h"

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

	//--------------------------------
	//アンビエント色
	DWORD d;
	D3DXVECTOR4 col;
	d3d.GetRenderState(D3DRS_AMBIENT, &d);

	col.x = static_cast<float>((0x00ff0000 & d) >> (8 * 2)) / 255;
	col.y = static_cast<float>((0x0000ff00 & d) >> (8 * 1)) / 255;
	col.z = static_cast<float>((0x000000ff & d) >> (8 * 0)) / 255;
	col.w = 1.0f;// static_cast<float>((0xff000000 & d) >> (8 * 3)) / 255;
	pEffect->SetVector4("I_a", col);



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

					D3DMATERIAL9 mat = pMesh->Get_ppMaterials()[subsetNum];			
					

					D3DXVECTOR4 amb;
					amb.x = mat.Ambient.r;
					amb.y = mat.Ambient.g;
					amb.z = mat.Ambient.b;
					amb.w = mat.Ambient.a;//*col.a*/;

					pEffect->SetVector4("K_a", amb);

					D3DXVECTOR4 dif;
					dif.x = mat.Diffuse.r;
					dif.y = mat.Diffuse.g;
					dif.z = mat.Diffuse.b;
					dif.w = mat.Diffuse.a;
					pEffect->SetVector4("K_d", dif);
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
//EffectPhong用の関数

HRESULT  EffectParamSetter::OnSetTechnique(EffectPhong* pEffect, int techniqueNum, D3DXHANDLE& tecHandle)
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
	D3DXVECTOR4 lightVec = D3DXVECTOR4(1, -1,1, 0);
	D3DXVec4Transform(&lightVec, &lightVec, &worldInverse);
	D3DXVec3Normalize((D3DXVECTOR3*)&lightVec, (D3DXVECTOR3*)&lightVec);

	pEffect->SetVectorLightDirection(lightVec);

	//視線　向き

	D3DXMATRIXA16 matTmp;
	D3DXMatrixMultiply(&matTmp, &worldMat, &view);//ローカル座標系からビュー座標系への変換行列
	D3DXMatrixInverse(&matTmp, NULL, &matTmp);//上の行列の逆行列　ビュー座標系からローカル座標系への変換行列
	D3DXVECTOR4 v(0, 0, 0, 1);//原点
	D3DXVec4Transform(&v, &v, &matTmp);//ビュー座標系原点をローカル座標系の点へ変換
	pEffect->SetVectorEye(v);


	//--------------------------------
	//アンビエント色
	DWORD d;
	D3DXVECTOR4 col;
	d3d.GetRenderState(D3DRS_AMBIENT, &d);

	col.x = static_cast<float>((0x00ff0000 & d) >> (8 * 2)) / 255;
	col.y = static_cast<float>((0x0000ff00 & d) >> (8 * 1)) / 255;
	col.z = static_cast<float>((0x000000ff & d) >> (8 * 0)) / 255;
	col.w = 1.0f;// static_cast<float>((0xff000000 & d) >> (8 * 3)) / 255;
	pEffect->SetVector4("I_a", col);



	return S_OK;
}

HRESULT  EffectParamSetter::OnBegin(EffectPhong* pEffect, UINT*pPasses, DWORD Flag, int subsetNum)
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

				

					D3DMATERIAL9 mat = pMesh->Get_ppMaterials()[subsetNum];


					D3DXVECTOR4 amb;
					amb.x = mat.Ambient.r;
					amb.y = mat.Ambient.g;
					amb.z = mat.Ambient.b;
					amb.w = mat.Ambient.a;//*col.a*/;

					pEffect->SetVector4("K_a", amb);

					D3DXVECTOR4 dif;
					dif.x = mat.Diffuse.r;
					dif.y = mat.Diffuse.g;
					dif.z = mat.Diffuse.b;
					dif.w = mat.Diffuse.a;
					pEffect->SetVector4("K_d", dif);
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

HRESULT  EffectParamSetter::OnBeginPass(EffectPhong* pEffect, UINT pass)
{

	return S_OK;
}

//==============================================================================================================
//==============================================================================================================
//==============================================================================================================
//EffectPhong用の関数

HRESULT  EffectParamSetter::OnSetTechnique(EffectPhongAndNormal* pEffect, int techniqueNum, D3DXHANDLE& tecHandle)
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
	D3DXVec4Transform(&lightVec, &lightVec, &worldInverse);
	D3DXVec3Normalize((D3DXVECTOR3*)&lightVec, (D3DXVECTOR3*)&lightVec);

	pEffect->SetVectorLightDirection(lightVec);

	//視線　向き

	D3DXMATRIXA16 matTmp;
	D3DXMatrixMultiply(&matTmp, &worldMat, &view);//ローカル座標系からビュー座標系への変換行列
	D3DXMatrixInverse(&matTmp, NULL, &matTmp);//上の行列の逆行列　ビュー座標系からローカル座標系への変換行列
	D3DXVECTOR4 v(0, 0, 0, 1);//原点
	D3DXVec4Transform(&v, &v, &matTmp);//ビュー座標系原点をローカル座標系の点へ変換
	pEffect->SetVectorEye(v);


	//--------------------------------
	//アンビエント色
	DWORD d;
	D3DXVECTOR4 col;
	d3d.GetRenderState(D3DRS_AMBIENT, &d);

	col.x = static_cast<float>((0x00ff0000 & d) >> (8 * 2)) / 255;
	col.y = static_cast<float>((0x0000ff00 & d) >> (8 * 1)) / 255;
	col.z = static_cast<float>((0x000000ff & d) >> (8 * 0)) / 255;
	col.w = 1.0f;// static_cast<float>((0xff000000 & d) >> (8 * 3)) / 255;
	pEffect->SetVector4("I_a", col);



	return S_OK;
}

HRESULT  EffectParamSetter::OnBegin(EffectPhongAndNormal* pEffect, UINT*pPasses, DWORD Flag, int subsetNum)
{
	HRESULT h = S_OK;
	switch (mode)
	{
		case EffectParamSetter::NONE:
			break;
		case EffectParamSetter::MESH_X:
			if (pMesh != nullptr)
			{
				MeshXWithHeight* pMeshWithHeight = dynamic_cast<MeshXWithHeight*>(pMesh);
				if (subsetNum >= 0 && subsetNum < pMesh->GetNumMaterials())
				{
					pEffect->SetTextureMain(pMesh->Get_ppTextures()[subsetNum]);

					if (pMeshWithHeight != nullptr)
					{
						pEffect->SetTextureNormal(pMeshWithHeight->Get_ppNormalTextures()[subsetNum]);
					}
					else
					{
						pEffect->SetTextureNormal(nullptr);
					}

					D3DMATERIAL9 mat = pMesh->Get_ppMaterials()[subsetNum];


					D3DXVECTOR4 amb;
					amb.x = mat.Ambient.r;
					amb.y = mat.Ambient.g;
					amb.z = mat.Ambient.b;
					amb.w = mat.Ambient.a;//*col.a*/;

					pEffect->SetVector4("K_a", amb);

					D3DXVECTOR4 dif;
					dif.x = mat.Diffuse.r;
					dif.y = mat.Diffuse.g;
					dif.z = mat.Diffuse.b;
					dif.w = mat.Diffuse.a;
					pEffect->SetVector4("K_d", dif);
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

HRESULT  EffectParamSetter::OnBeginPass(EffectPhongAndNormal* pEffect, UINT pass)
{

	return S_OK;
}


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