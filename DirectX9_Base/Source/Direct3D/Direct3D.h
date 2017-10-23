#pragma once 

#define D3D_DEBUG_INFO	//Direct3Dデバッグフラグ

#define D3DFVF_LINE_VERTEX D3DFVF_XYZ|D3DFVF_DIFFUSE

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dxof.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//#include <memory>

#include <comdef.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <mmsystem.h>

#include "../Template/Singleton.h"

enum RENDERSTATE
{
	RENDER_DEFAULT,		//デフォルト
	RENDER_ALPHATEST,	//αテスト
	RENDER_ALPHABLEND,
	RENDER_HALFADD,		//半加算合成
	RENDER_ADD,		//加算合成

	RENDER_MESH,
	RENDER_SHADOW_MAP,
};



struct LINE_VERTEX
{
	D3DXVECTOR3 v;
	DWORD color;
};

class Texture;
class Sprite;
class MeshX;
class TriangleList;
class Effect;
class ShadowMapTexture;

//-------------------------------
class Direct3D : public Singleton<Direct3D>
{
	friend class Singleton<Direct3D>;

public :
	typedef void(*FuncPointer)();//関数ポインタの型定義

public:	

	const IDirect3DDevice9* GetDevice()const
	{
		return pDevice3D;
	}
	
	//デバイス作成を試みる
	bool TryCreate(HWND hWmd);

	static void Render();
	static void SetRenderState(RENDERSTATE RenderrState);

	bool LoadTexture(Texture& texture, TCHAR* FileName);
	void LoadNormalTextures(LPDIRECT3DTEXTURE9& pDestTarget, TCHAR* filepath_HeightMap);
	
	void  SetViewMatrix(D3DXMATRIXA16& mat);
	void SetProjectionMatrix(D3DXMATRIXA16& mat);
	void SetupRrojectionMatrix();

	void LoadMeshX(MeshX& mesh,TCHAR* path);
	void DrawMeshX(MeshX& mesh, D3DXMATRIXA16& worldMat);
	void DrawMeshX(MeshX& mesh, D3DXMATRIXA16& worldMat, Effect* pEffect);

	void DrawTriangleList(TriangleList& triangleList, D3DXMATRIXA16& worldMat);

	void CreateVertexDecle(D3DVERTEXELEMENT9* elements, IDirect3DVertexDeclaration9** ppVertexDelc_Dest);

	void CloneMesh(LPD3DXMESH& formMesh, LPD3DXMESH& destMesh, D3DVERTEXELEMENT9* delcArray);

	void Clear(DWORD count, const D3DRECT* pRect, DWORD Flag, D3DCOLOR clearColor, float z, DWORD stencil);

	//スプライトの表示
	static void DrawSprite(Sprite& sprite, Texture& texture , bool isTurn = false);
	static void DrawSprite(Sprite& sprite, bool isTurn = false);
	void SetDrawFunc(FuncPointer pointer);

	void DrawLine(LINE_VERTEX* pVertex, int count);

	HRESULT CreateEffectFromFile(Effect& refEffect, std::string filepath);

	HRESULT GetTransForm(_D3DTRANSFORMSTATETYPE type, D3DXMATRIXA16& mat) {	return (pDevice3D) ? pDevice3D->GetTransform(type, &mat) : E_FAIL;}
	HRESULT GetRenderState(D3DRENDERSTATETYPE type,DWORD* d) {return  (pDevice3D) ? pDevice3D->GetRenderState(type,d): E_FAIL;}
	
	void ChangeRenderTarget_Default();
	void ChangeDepthStencilSurfac_Default();
	void ChangeViewPort_Default();

	void ChangeRenderTarget(LPDIRECT3DSURFACE9 pTarget);
	void ChangeDepthStencilSurface(LPDIRECT3DSURFACE9 pZbuffer);
	void ChangeViewPort(D3DVIEWPORT9& pViewPort);


	HRESULT CallCreateShadowMap(ShadowMapTexture& tex);

	bool  UseMeshMaterial() { return useMeshMaterial; }

	void SetLight(DWORD index, D3DLIGHT9& light);
	void LightEnable(DWORD index, BOOL enable);



	void Test();
private:
	HWND hWnd;

	IDirect3D9*	pD3D9 = nullptr;				//Direct3Dデバイス生成用オブジェクト
	IDirect3DDevice9* pDevice3D = nullptr;		//Direct3Dのデバイス　スプライトの表示やテクスチャのロード時に必要
	
	bool useMeshMaterial;

	LPDIRECT3DSURFACE9 pDefaultBackBuffer;
	LPDIRECT3DSURFACE9 pDefaultZBuffer;
	D3DVIEWPORT9 DefaultViewPort;

	static RENDERSTATE currentState;

	//デバイス作成
	bool Create(HWND hWmd);

	bool isDeviceCreated ;
	bool IsDeviceCreated(){ return isDeviceCreated; }

	//コンストラクタ　デストラクタ
	Direct3D();
	~Direct3D();

	FuncPointer DrawFunc;//関数ポインタ
	void TryCallDrawFunc();
	

	void ReleaseDevice();

	

	
};