#pragma once

#include "Direct3D.h"

class Light
{
	D3DXVECTOR4 direction;
	D3DLIGHT9 light;
	int lightNum;

public :
	Light();
	void Init(D3DXVECTOR4 dir,int num);

	D3DXVECTOR4 GetDir() { return direction; }

};