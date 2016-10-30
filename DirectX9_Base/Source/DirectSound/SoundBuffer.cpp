#include "SoundBuffer.h"
#include "WaveFile.h"
#include "DirectSound.h"

//コンストラクタ
SoundBuffer::SoundBuffer()
{
	pSecondaryBuffer = NULL;

}
//デストラクタ
SoundBuffer::~SoundBuffer()
{
	Release();
}

//開放処理
void SoundBuffer::Release()
{
	if (pSecondaryBuffer != NULL)
	{
		Stop();//開放する際には再生停止する

		pSecondaryBuffer->Release();
		pSecondaryBuffer = NULL;
	}
}

//再生開始
void SoundBuffer::Play(bool isLoop)
{
	if (pSecondaryBuffer != NULL)
	{
		DWORD LoopFlag = (isLoop ? DSBPLAY_LOOPING : 0);
		pSecondaryBuffer->Play(0, 0, LoopFlag);
	}
}

//再生停止
void SoundBuffer::Stop()
{
	if (pSecondaryBuffer != NULL)
	{
		pSecondaryBuffer->Stop();
	}
}



//バッファ作成

bool SoundBuffer::Create(WaveFile& waveFile)
{
	//desc : description
	DSBUFFERDESC desc = {};//セカンダリバッファ作成用設定

	//チャンネル数での分岐
	//モノラルは1チャンネル
	//ステレオは2チャンネル

	if (waveFile.waveFormat.nChannels==1)
	{//モノラル
		desc.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_STATIC;

		desc.guid3DAlgorithm = DS3DALG_NO_VIRTUALIZATION;
	}
	else
	{//ステレオ
		desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY |
			DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_STATIC | DSBCAPS_CTRLPAN;
		// |DSBCAPS_CTRFX; エフェクトを追加するとDuplicateできない

		desc.guid3DAlgorithm = GUID_NULL;
	}

	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwBufferBytes = waveFile.dataSize;//音データサイズ
	desc.lpwfxFormat = &waveFile.waveFormat;//フォーマット指定

	//デバイス取得
	DirectSound & directSound = DirectSound::GetInstance();

	//サウンドバッファ
	IDirectSoundBuffer* pBuffer = NULL;

	//プライマリバッファ作製
	if (directSound.CreateSoundBuffer(&desc,&pBuffer))
	{
		//渡されたwaveデータに問題がある
		//初期化されていないか
		//ロード失敗
		//の可能性

		return false;
	}

	//サウンドバッファからセカンダリバッファ作成
	//IDirectSoundBufferから IDirectSoundBuffer8に変換する
	pBuffer->QueryInterface(IID_IDirectSoundBuffer8,(void **)&pSecondaryBuffer);
	pBuffer->Release();//セカンダリバッファが作成出来ればバッファは破棄して構わない


	unsigned char *block1 = NULL;
	unsigned char *block2 = NULL;
	unsigned long blockSize1 = 0;
	unsigned long blockSize2 = 0;

	//セカンダリバッファをロックしてデータを書き込み
	pSecondaryBuffer->Lock(0, waveFile.dataSize, (void**)&block1, &blockSize1, (void**)&block2, &blockSize2, DSBLOCK_ENTIREBUFFER);

	//セカンダリバッファに音データをコピー
	memcpy(block1, waveFile.waveData, waveFile.dataSize);

	//セカンダリバッファのロック解除
	pSecondaryBuffer->Unlock(block1, blockSize1, block2, 0);

	return true;
}