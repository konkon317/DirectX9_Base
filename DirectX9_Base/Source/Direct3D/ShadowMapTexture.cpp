#include "ShadowMapTexture.h"

//�ÓI
HRESULT ShadowMapTexture::Create(IDirect3DDevice9* pDevice, ShadowMapTexture& texture)
{
	if (texture.Created() || !(texture.size > 0))
	{
		return E_FAIL;
	}

	HRESULT hr;
	hr = pDevice->CreateTexture(texture.size, texture.size, 1
		, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, &texture.shadowTex, NULL);
	if (FAILED(hr))return hr;

	hr = texture.shadowTex->GetSurfaceLevel(0, &texture.shadowSurf);
	if (FAILED(hr))return hr;

	hr = pDevice->CreateDepthStencilSurface(texture.size, texture.size,
		D3DFMT_D16, D3DMULTISAMPLE_NONE,
		0, TRUE, &texture.shadowTexZ, NULL);

	return hr;

}

//�R���X�g���N�^

ShadowMapTexture::ShadowMapTexture()
{
	shadowTex=nullptr;
	shadowSurf=nullptr;
	shadowTexZ=nullptr;

	size = 0;
}

//�f�X�g���N�^

ShadowMapTexture::~ShadowMapTexture()
{
	this->Release();
}

//�ʏ�̃����o�֐�

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
			return hr;
		}
	}	
	else
	{
		return E_FAIL;
	}
}