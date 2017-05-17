#pragma once

#include "Direct3d.h"

class Texture
{
	friend class Direct3D;
private:
	IDirect3DTexture9* pTexture;

public:
	
	//コンストラクタ デストラクタ
	Texture();
	~Texture();

	//UVの分割数
	unsigned int divU;
	unsigned int divV;
	//UVの番号
	unsigned int numU;
	unsigned int numV;

	void SetDivide(unsigned int DivU, unsigned int DivV);
	void SetUVNum(unsigned int NumU, unsigned int NumV);

	bool Load(TCHAR* FileName);


};