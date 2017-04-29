#include "texture.h"

//コンストラクタ
Texture::Texture()
{
	pTexture = nullptr;

	divU = 1;
	divV = 1;
	numU = 0;
	numV = 0;
}

//デストラクタ
Texture::~Texture()
{
	//読み込まれていたら破棄
	if (pTexture != nullptr)
	{
		pTexture->Release();
	}
}

bool Texture::Load(TCHAR* FileName)
{
	Direct3D& d3d =Direct3D::GetInstance();

	return d3d.LoadTexture(*this, FileName);//読み込み
}

//テクスチャを縦横何分割して利用するかを設定
void Texture::SetDivide(unsigned int DivU, unsigned int DivV)
{
	if (DivU <= 0 || DivV <= 0)
	{
		return;
	}

	divU = DivU;
	divV = DivV;
}

//分割したテクスチャの縦横何番目を使うかを指定
void Texture::SetUVNum(unsigned int NumU, unsigned int NumV)
{
	if (NumU >= divU)return;
	if (NumV >= divV)return;

	numU = NumU;
	numV = NumV;
}