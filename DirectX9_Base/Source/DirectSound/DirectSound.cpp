#include "DirectSound.h"

//コンストラクタ
DirectSound::DirectSound()
{
	pDirectSound8 = nullptr;
}

//デストラクタ
DirectSound::~DirectSound()
{
	Release();
}

//開放
void DirectSound::Release()
{
	if (pDirectSound8 != nullptr)
	{
		pDirectSound8->Release();
		pDirectSound8 = nullptr;
	}
}

//サウンドデバイスの作成と協調レベルの設定
bool DirectSound::Create(HWND hWnd)
{ 
	//デバイス作成
	if(FAILED(DirectSoundCreate8(NULL, &pDirectSound8, NULL)))
	{
		//作製失敗
		Release();
		return false;
	}

	//協調レベルの設定　
	//また　音を鳴らすウィンドウの設定
	if (FAILED(pDirectSound8->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
		Release();
		return false;

	}

	return true;

}

HRESULT DirectSound::CreateSoundBuffer(DSBUFFERDESC* pDesc, IDirectSoundBuffer** ppBuffer)
{
	if (pDirectSound8 == nullptr)
	{
		//デバイスが作成されていないため失敗
		return S_FALSE;
	}
	HRESULT hr=pDirectSound8->CreateSoundBuffer(pDesc, ppBuffer,NULL);
	
	return hr;

}
