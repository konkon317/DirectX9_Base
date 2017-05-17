#pragma once


#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"d3dxof.lib")
#pragma comment(lib,"dxguid.lib")

#include <dsound.h>
#include <tchar.h>
#include <comdef.h>



class WaveFile
{
	//soundbuffer空は変数に自由にアクセスできるようにする
	friend class SoundBuffer;

private:
	WAVEFORMATEX waveFormat;
	byte*		 waveData;//波形データ
	int			 dataSize;//波形データのサイズ



public:
	//コンストラクタ
	WaveFile();
	~WaveFile();

	//ロード
	bool Load(TCHAR* FilePath);

	void Release();

};