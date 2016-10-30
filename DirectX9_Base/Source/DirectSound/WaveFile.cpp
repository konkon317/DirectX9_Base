#define _CRT_SECURE_NO_WARNINGS
//fopenでの警告をなくすため

#include "WaveFile.h"
#include <string>

using namespace std;

//コンストラクタ
WaveFile::WaveFile()
{
	//waveFormatを0で初期化
	memset(&waveFormat, 0, sizeof(WAVEFORMATEX));

	waveData = NULL;
	dataSize = 0;

}

//デストラクタ
WaveFile::~WaveFile()
{
	Release();
}


//開放処理
void WaveFile::Release()
{
	if (waveData != NULL)
	{
		delete[]waveData;

		waveData = NULL;
		memset(&waveFormat, 0, sizeof(WAVEFORMATEX));		
		dataSize = 0;
	}
}

//Waveファイルの読み込み
bool WaveFile::Load(TCHAR* FilePath)
{
	FILE* fp;

	//ファイルオープン
	if (!(fp = _tfopen(FilePath, _T("rb"))))
	{
		return false;
	}

	char chunkId[5] = {};
	char type[5] = {};
	unsigned int chunkSize = 0;

	
	//RFFヘッダーの構成
	//riff "RIFF"の4文字　							4byte
	//size 総ファイルサイズ-8（byte単位）			4byte
	//type "WAVE"の4文字(音声データの場合"WAVE")	4byte 

	//RIFFヘッダーチャンク読み込み
	fread(chunkId, sizeof(char) * 4, 1, fp);
	fread(&chunkSize, sizeof(unsigned int), 1, fp);
	fread(type, sizeof(char) * 4, 1, fp);

	string sChunkId(chunkId);
	string sType(type);
	//riff形式 タイプが
	if (!(sChunkId == "RIFF" && sType == "WAVE"))
	{
		//Waveファイルではない
		return false;
	}

	//子チャンクの読みこみ
	bool fmtChunk = false;
	bool dataChunk = false;

	while (true)
	{
		fread(chunkId, sizeof(char) * 4, 1, fp);
		fread(&chunkSize, sizeof(unsigned int), 1, fp);

		sChunkId = chunkId;

		//フォーマットチャンクの読み込み
		if (sChunkId == "fmt ")
		{
			//フォーマットチャンクの構成
			//id			4byte	チャンク識別子
			//size			4byte	idとsizeを除くチャンクサイズ
			//format		2byte	波形データのフォーマット
			//channels		2byte	使用チャンネル数
			//samplerate	4byte	サンプリング周波数
			//bytepersec	4byte	平均ンデータ割合
			//blockalign	2byte	データのブロックサイズ
			//bitswidth		2byte	1サンプル当たりのビット数
			//extended_size	2byte	拡張データのサイズ
			//extended		nbyte	拡張データ

			
			if (chunkSize >= sizeof(WAVEFORMATEX))
			{
				//WAVEFORMATEXのおおきさ分読み込み
				fread(&waveFormat, sizeof(WAVEFORMATEX), 1, fp);		

				//チャンクの残り領域のおおきさを計算
				int diff = chunkSize - sizeof(WAVEFORMATEX);

				//次のチャンクの先頭までシーク
				fseek(fp, diff, SEEK_CUR);
			
			}
			else
			{			
				//チャンクの終わりまで読み込む
				fread(&waveFormat, chunkSize, 1, fp);
			}

			fmtChunk = true;//フォーマットチャンク読み込み済みを示す
		}
		else if(sChunkId =="data")//データチャンクの読み込み
		{
			//データチャンク
			//id				4byte	チャンク識別子
			//size				4byte	idとsizeを除くチャンクサイズ(波形データのサイズ)
			//waveformatData	nbyte	波形データ

			//データサイズ書くウホ
			dataSize = chunkSize;
			waveData = new byte[chunkSize];
		
			//データ読み込み
			unsigned long readCount = fread(waveData, sizeof(byte), chunkSize, fp);
			if (readCount!= chunkSize)
			{
				fclose(fp);

				Release();
				return false;
			}
			dataChunk = true;


		}
		else
		{
			fseek(fp, chunkSize, SEEK_CUR);
		}

		//フォーマットチャンクとデータチャンクの両方が読み込み済みならループを抜ける
		if (fmtChunk && dataChunk)
		{
			break;
		}

	}
	fclose(fp);

	return true;
}