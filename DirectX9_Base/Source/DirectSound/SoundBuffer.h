#pragma once

#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")

#include <dsound.h>
#include <tchar.h>
#include <comdef.h>

class WaveFile;

class SoundBuffer
{
private :
	IDirectSoundBuffer8 *pSecondaryBuffer;//セカンダリバッファ

public :
	SoundBuffer();
	~SoundBuffer();

	bool Create(WaveFile& waveFile);

	void Play(bool Loop);
	void Stop();

	void Release();

};