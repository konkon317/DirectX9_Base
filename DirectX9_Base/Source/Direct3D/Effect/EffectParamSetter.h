#pragma once

#include "Effect.h"

class Effect;
class EffectLambert;
class EffectBasic;

class MeshX;

class EffectParamSetter
{
public :
	enum MODE
	{
		NONE,
		MESH_X
	};

private:
	MODE mode;

	MeshX* pMesh;
public :
	EffectParamSetter() { mode = NONE; pMesh = nullptr; }

	void SetMode(MODE mode) { this->mode = mode; }
	void SetMeshPointer(MeshX* pMesh) { this->pMesh = pMesh; }
	void Reset() { mode = MODE::NONE; pMesh = nullptr; }

	//窓口　pEffectから再度SetTechniqueを呼んでもらい　実行時型に合わせたオーバーロードを実行　
	//実行時型に合わせた設定を行う
	HRESULT SetTechnique(Effect* pEffect, int techniqueNum);// { return (pEffect != nullptr) ? pEffect->SetTechnique(*this, techniqueNum) : E_FAIL; };
	HRESULT Begin(Effect* pEffect, UINT*pPasses, DWORD Flag, int subsetNum);// { return (pEffect != nullptr) ? pEffect->Begine(*this, pPasses, Flag, subsetNum) : E_FAIL; };
	HRESULT BeginPass(Effect* pEffect, UINT pass);// { return (pEffect != nullptr) ? pEffect->BeginePass(*this, pass) : E_FAIL; };

	//EffectLambert用
	HRESULT OnSetTechnique	(EffectLambert* pEffect, int techniqueNum,D3DXHANDLE& tecHandle);
	HRESULT OnBegin			(EffectLambert* pEffect, UINT*pPasses, DWORD Flag,int subsetNum);
	HRESULT OnBeginPass		(EffectLambert* pEffect, UINT pass) ;

	//EffectBasic用
	HRESULT OnSetTechnique(EffectBasic* pEffect, int techniqueNum, D3DXHANDLE& tecHandle);
	HRESULT OnBegin(EffectBasic* pEffect, UINT*pPasses, DWORD Flag, int subsetNum);
	HRESULT OnBeginPass(EffectBasic* pEffect, UINT pass);

};
