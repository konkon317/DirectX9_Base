#include "sprite.h"


//コンストラクタ
Sprite::Sprite()
{
	pos.x = pos.y = 0.0f;
	width = height = 0;

	rotate = 0.0f;
}

//デストラクタ

Sprite::~Sprite()
{

}

//------------------------

void Sprite::SetPos(float x, float y)
{
	pos.x = x;
	pos.y = y;
}


//------------------------

void Sprite::SetSize(int Width, int Height)
{
	width = Width;
	height = Height;
}


//------------------------



//回転を設定 ラジアン
void Sprite::SetRotate(float Rotate)
{
	rotate = Rotate;
}