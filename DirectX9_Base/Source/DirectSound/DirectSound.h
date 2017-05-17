#pragma once

#define DIRECTSOUND_VERSION 0x800

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")

#include <comdef.h>
#include <dsound.h>

#include <tchar.h>

#include "../Template/Singleton.h"


class DirectSound : public Singleton<DirectSound>
{
	friend class Singleton<DirectSound>;

private :
	IDirectSound8* pDirectSound8;//サウンドデバイス

	DirectSound();
	~DirectSound();

	void Release();

public :
	bool Create(HWND hWnd);//デバイス作成

	const IDirectSound8* GetDevice(){ return pDirectSound8; }

	HRESULT CreateSoundBuffer(DSBUFFERDESC* pDesc, IDirectSoundBuffer** ppBuffer);

	
};