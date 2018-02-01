#include "ShadowMapTexture.h"

//静的
HRESULT ShadowMapTexture::Create(IDirect3DDevice9* pDevice, ShadowMapTexture& texture)
{
	if (texture.Created() || !(texture.size > 0))
	{
		return E_FAIL;
	}

	HRESULT hr;
	hr = pDevice->CreateTexture(texture.size, texture.size, 1
		, D3DUSAGE_RENDERTARGET, D3DFMT_R32F/*D3DFMT_A8R8G8B8*/,
		D3DPOOL_DEFAULT, &texture.shadowTex, NULL);
	if (FAILED(hr))return hr;

	hr = texture.shadowTex->GetSurfaceLevel(0, &texture.shadowSurf);
	if (FAILED(hr))return hr;

	hr = pDevice->CreateDepthStencilSurface(texture.size, texture.size,
		D3DFMT_D16, D3DMULTISAMPLE_NONE,
		0, TRUE, &texture.shadowTexZ, NULL);

	return hr;

}

//コンストラクタ

ShadowMapTexture::ShadowMapTexture()
{
	shadowTex=nullptr;
	shadowSurf=nullptr;
	shadowTexZ=nullptr;

	size = 0;
}

//デストラクタ

ShadowMapTexture::~ShadowMapTexture()
{
	this->Release();
}

//通常のメンバ関数

void ShadowMapTexture::Release()
{
	if (shadowTex != nullptr)
	{
		shadowTex->Release();
		shadowTex = nullptr;
	}

	if (shadowSurf != nullptr)
	{
		shadowSurf->Release();
		shadowSurf = nullptr;
	}

	if (shadowTexZ != nullptr)
	{
		shadowTexZ->Release();
		shadowTexZ = nullptr;
	}
	size = 0;
}

HRESULT ShadowMapTexture::Create(int size)
{
	if(size>0)
	{ 
		this->size = size;
		HRESULT hr=Direct3D::GetInstance().CallCreateShadowMap(*this);

		if (FAILED(hr))
		{
			this->Release();
			
		}
		return hr;
	}	
	else
	{
		return E_FAIL;
	}
}

void ShadowMapTexture::SetRenderTarget()
{
	if (Created())
	{
		Direct3D&d3d = Direct3D::GetInstance();
		d3d.ChangeRenderTarget(shadowSurf);
		d3d.ChangeDepthStencilSurface(shadowTexZ);
		D3DVIEWPORT9 viewport = { 0, 0, size, size, 0.0f, 1.0f };

		d3d.ChangeViewPort(viewport);

		d3d.Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, /*0x00000000*/0xffffffff, 1.0f, 0L);
			
	}
}