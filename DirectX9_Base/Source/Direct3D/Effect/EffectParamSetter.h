#pragma once

#include "Effect.h"

class Effect;
class EffectLambert;
class EffectBasic;
class EffectPhong;
class EffectPhongAndNormal;
class EffectProjectedTextureShadow;
class EffectPriorityBufferShadow;

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
	
	//EffectLambert用
	HRESULT OnSetTechnique	(EffectLambert* pEffect, int techniqueNum,D3DXHANDLE& tecHandle);
	HRESULT OnBegin			(EffectLambert* pEffect, UINT*pPasses, DWORD Flag, unsigned int subsetNum);
	HRESULT OnBeginPass		(EffectLambert* pEffect, UINT pass) ;
	//EffectPhong用
	HRESULT OnSetTechnique(EffectPhong* pEffect, int techniqueNum, D3DXHANDLE& tecHandle);
	HRESULT OnBegin(EffectPhong* pEffect, UINT*pPasses, DWORD Flag, unsigned int subsetNum);
	HRESULT OnBeginPass(EffectPhong* pEffect, UINT pass);
	//EffectPhongAndNormal用
	HRESULT OnSetTechnique(EffectPhongAndNormal* pEffect, int techniqueNum, D3DXHANDLE& tecHandle);
	HRESULT OnBegin(EffectPhongAndNormal* pEffect, UINT*pPasses, DWORD Flag, unsigned int subsetNum);
	HRESULT OnBeginPass(EffectPhongAndNormal* pEffect, UINT pass);
	//EffectProjectedTextureShadow用
	HRESULT OnSetTechnique(EffectProjectedTextureShadow* pEffect, int techniqueNum, D3DXHANDLE& tecHandle);
	HRESULT OnBegin(EffectProjectedTextureShadow* pEffect, UINT*pPasses, DWORD Flag, unsigned int subsetNum);
	HRESULT OnBeginPass(EffectProjectedTextureShadow* pEffect, UINT pass);

	//EffectPriorityBufferShadow用
	HRESULT OnSetTechnique(EffectPriorityBufferShadow* pEffect, int techniqueNum, D3DXHANDLE& tecHandle);
	HRESULT OnBegin(EffectPriorityBufferShadow* pEffect, UINT*pPasses, DWORD Flag, unsigned int subsetNum);
	HRESULT OnBeginPass(EffectPriorityBufferShadow* pEffect, UINT pass);

	//EffectBasic用
	HRESULT OnSetTechnique(EffectBasic* pEffect, int techniqueNum, D3DXHANDLE& tecHandle);
	HRESULT OnBegin(EffectBasic* pEffect, UINT*pPasses, DWORD Flag, unsigned int subsetNum);
	HRESULT OnBeginPass(EffectBasic* pEffect, UINT pass);

};
